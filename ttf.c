// Badger
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define READ_BE16(mem) ((((u8*)(mem))[0] << 8) | (((u8*)(mem))[1]))
#define READ_BE32(mem) ((((u8*)(mem))[0] << 24) | (((u8*)(mem))[1] << 16) | (((u8*)(mem))[2] << 8) | (((u8*)(mem))[3]))
#define P_MOVE(mem, a) ((mem) += (a))

#define READ_BE16_MOVE(mem) (READ_BE16((mem))); (P_MOVE((mem), 2))
#define READ_BE32_MOVE(mem) (READ_BE32((mem))); (P_MOVE((mem), 4))

#define FONT_TAG(a, b, c, d) (a<<24|b<<16|c<<8|d)
#define GLYF_TAG FONT_TAG('g', 'l', 'y', 'f')
#define LOCA_TAG FONT_TAG('l', 'o', 'c', 'a')
#define HEAD_TAG FONT_TAG('h', 'e', 'a', 'd')
#define CMAP_TAG FONT_TAG('c', 'm', 'a', 'p')

char* read_file(char *file_name, int* file_size) { 
	if(strlen(file_name) > 0) {
		FILE* file = fopen(file_name, "rb");
		if(file) {
			fseek(file, 0, SEEK_END);
			int size = ftell(file);
			fseek(file, 0, SEEK_SET);

			if(file_size) { *file_size = size; }
			char *file_content = (char*)malloc(size+1);
			int read_amount = fread(file_content, size, 1, file);
			file_content[size] = '\0';
			if(read_amount) {
				fclose(file);
				return file_content;
			}
			free(file_content);
			fclose(file);
			return NULL;
		}
	}
	return NULL;
}

typedef unsigned char u8;
typedef char i8;

typedef unsigned short u16;
typedef short i16;

typedef unsigned int u32;
typedef int i32;

typedef struct {
	u32	scaler_type;
	u16	numTables;
	u16	searchRange;
	u16	entrySelector;
	u16	rangeShift;
} offset_subtable;

typedef struct {
	u16 platformID;
	u16 platformSpecificID;
	u32 offset;
} cmap_encoding_subtable;

typedef struct {
	u16 version;
	u16 numberSubtables;
	cmap_encoding_subtable* subtables;
} cmap;

typedef struct {
	u16  format;
 	u16  length;
 	u16  language;
 	u16  segCountX2;
 	u16  searchRange;
 	u16  entrySelector;
 	u16  rangeShift;
	u16  reservedPad;
	u16  *endCode;
	u16  *startCode;
	u16  *idDelta;
	u16  *idRangeOffset;
	u16  *glyphIdArray;
} format4;

typedef struct {
	union { 
		char tag_c[4];
		u32	tag;
	};
	u32	checkSum;
	u32	offset;
	u32	length;
} table_directory;

typedef struct  {
	offset_subtable off_sub;
	table_directory* tbl_dir;
	format4* f4;
	cmap* cmap;
	char* glyf;
	char* loca;
	char* head;
} font_directory; 

typedef union {
	struct {
		u8 on_curve: 1;
		
		u8 x_short: 1;
		u8 y_short: 1;
		
		u8 repeat: 1;
		
		u8 x_short_pos: 1;
		u8 y_short_pos: 1;
		
		u8 reserved1: 1;
		u8 reserved2: 1;
	};
	u8 flag;
} glyph_flag;


typedef struct {
	u16 numberOfContours;
	i16 xMin;
	i16 yMin;
	i16 xMax;
	i16 yMax;
	u16 instructionLength;
	u8* instructions;
	glyph_flag* flags;
	i16* xCoordinates;
	i16* yCoordinates;
	u16* endPtsOfContours;
} glyph_outline;

typedef struct {
	u8* pixels;
	int width;
	int height;
} bitmap;

void read_offset_subtable(char** mem, offset_subtable* off_sub) {
	char* m = *mem;
	off_sub->scaler_type = READ_BE32_MOVE(m);
	off_sub->numTables = READ_BE16_MOVE(m);
	off_sub->searchRange = READ_BE16_MOVE(m);
	off_sub->entrySelector = READ_BE16_MOVE(m);
	off_sub->rangeShift = READ_BE16_MOVE(m);

	*mem = m;
}

void read_cmap(char* mem, cmap* c) {
	char *m = mem;
	c->version = READ_BE16_MOVE(m);
	c->numberSubtables = READ_BE16_MOVE(m);

	c->subtables = (cmap_encoding_subtable*) calloc(1, sizeof(cmap_encoding_subtable)*c->numberSubtables);
	for(int i = 0; i < c->numberSubtables; ++i) {
		cmap_encoding_subtable* est = c->subtables + i;
		est->platformID = READ_BE16_MOVE(m);
		est->platformSpecificID = READ_BE16_MOVE(m);
		est->offset = READ_BE32_MOVE(m);
	}

}

void print_cmap(cmap* c) {
	printf("#)\tpId\tpsID\toffset\ttype\n");
	for(int i = 0; i < c->numberSubtables; ++i) {
		cmap_encoding_subtable* cet = c->subtables + i;
		printf("%d)\t%d\t%d\t%d\t", i+1, cet->platformID, cet->platformSpecificID, cet->offset);
		switch(cet->platformID) {
			case 0: printf("Unicode"); break;
			case 1: printf("Mac"); break;
			case 2: printf("Not Supported"); break;
			case 3: printf("Microsoft"); break;
		}
		printf("\n");
	}
}

void read_format4(char* mem, format4** format) {
	char* m = mem;

	u16 length = READ_BE16(m + 2);

	format4* f = NULL;

	f = (format4*) calloc(1, length + sizeof(u16*)*5);
	f->format = READ_BE16_MOVE(m);
	f->length = READ_BE16_MOVE(m);
	f->language = READ_BE16_MOVE(m);
	f->segCountX2 = READ_BE16_MOVE(m);
	f->searchRange = READ_BE16_MOVE(m);
	f->entrySelector = READ_BE16_MOVE(m);
	f->rangeShift = READ_BE16_MOVE(m);

	f->endCode = (u16*) ((u8*)f  + sizeof(format4));
	f->startCode = f->endCode + f->segCountX2/2;
	f->idDelta = f->startCode + f->segCountX2/2;
	f->idRangeOffset = f->idDelta + f->segCountX2/2;
	f->glyphIdArray = f->idRangeOffset + f->segCountX2/2;

	char* start_code_start = m + f->segCountX2 + 2;
	char* id_delta_start = m + f->segCountX2*2 + 2;
	char* id_range_start = m + f->segCountX2*3 + 2;

	for(int i = 0; i < f->segCountX2/2; ++i) {
		f->endCode[i] = READ_BE16(m + i*2);
		f->startCode[i] = READ_BE16(start_code_start + i*2);
		f->idDelta[i] = READ_BE16(id_delta_start + i*2);
		f->idRangeOffset[i] = READ_BE16(id_range_start + i*2);
	}

	P_MOVE(m, f->segCountX2*4 + 2);	

	int remaining_bytes = f->length - (m - mem);
	for(int i = 0; i < remaining_bytes/2; ++i) {
		f->glyphIdArray[i] = READ_BE16_MOVE(m);
	}

	*format = f;
}

void print_format4(format4 *f4) {
	printf("Format: %d, Length: %d, Language: %d, Segment Count: %d\n", f4->format, f4->length, f4->language, f4->segCountX2/2);
	printf("Search Params: (searchRange: %d, entrySelector: %d, rangeShift: %d)\n",
			f4->searchRange, f4->entrySelector, f4->rangeShift);
	printf("Segment Ranges:\tstartCode\tendCode\tidDelta\tidRangeOffset\n");
	for(int i = 0; i < f4->segCountX2/2; ++i) {
		printf("--------------:\t% 9d\t% 7d\t% 7d\t% 12d\n", f4->startCode[i], f4->endCode[i], f4->idDelta[i], f4->idRangeOffset[i]);
	}
}

void read_table_directory(char* file_start, char** mem, font_directory* ft) {
	char* m = *mem;
	ft->tbl_dir = (table_directory*)calloc(1, sizeof(table_directory)*ft->off_sub.numTables);

	for(int i = 0; i < ft->off_sub.numTables; ++i) {
		table_directory* t = ft->tbl_dir + i;
		t->tag = READ_BE32_MOVE(m);
		t->checkSum = READ_BE32_MOVE(m);
		t->offset = READ_BE32_MOVE(m);
		t->length = READ_BE32_MOVE(m);

		switch(t->tag) {
			case GLYF_TAG: ft->glyf = t->offset + file_start; break;
			case LOCA_TAG: ft->loca = t->offset + file_start; break;
			case HEAD_TAG: ft->head = t->offset + file_start; break;
			case CMAP_TAG: {
				ft->cmap = (cmap*) calloc(1, sizeof(cmap));
				read_cmap(file_start + t->offset, ft->cmap);
				read_format4(file_start + t->offset + ft->cmap->subtables[0].offset, &ft->f4);
			} break;
		}
	}

	*mem = m;
}

void print_table_directory(table_directory* tbl_dir, int tbl_size) {
	printf("#)\ttag\tlen\toffset\n");
	for(int i = 0; i < tbl_size; ++i) {
		table_directory* t = tbl_dir + i;
		printf("%d)\t%c%c%c%c\t%d\t%d\n", i+1,
				t->tag_c[3], t->tag_c[2],
				t->tag_c[1], t->tag_c[0],
				t->length, t->offset);
	}
}

void read_font_directory(char* file_start, char** mem, font_directory* ft) {
	read_offset_subtable(mem, &ft->off_sub); 
	read_table_directory(file_start, mem, ft);
}

int get_glyph_index(font_directory* ft, u16 code_point) {
	format4 *f = ft->f4;
	int index = -1;
	u16 *ptr = NULL;
	for(int i = 0; i < f->segCountX2/2; i++) {
		if(f->endCode[i] > code_point) {index = i; break;};
	}
	
	if(index == -1) return 0;

	if(f->startCode[index] < code_point) {
		if(f->idRangeOffset[index] != 0) {
			ptr = f->idRangeOffset + index + f->idRangeOffset[index]/2;
			ptr += code_point - f->startCode[index];
			if(*ptr == 0) return 0;
			return *ptr + f->idDelta[index];
		} else {
			return code_point + f->idDelta[index];
		}
	}

	return 0;
}

int read_loca_type(font_directory* ft) {
	return READ_BE16(ft->head + 50);
}

u32 get_glyph_offset(font_directory *ft, u32 glyph_index) {
	u32 offset = 0;
	if(read_loca_type(ft)) {
		//32 bit
		offset = READ_BE32((u32*)ft->loca + glyph_index);
	} else {
		offset =  READ_BE16((u16*)ft->loca + glyph_index)*2;
	}
	return offset;
}

// TODO: many fonts not supported such as: /usr/share/fonts/truetype/freefont/FreeSans.ttf
glyph_outline get_glyph_outline(font_directory* ft, u32 glyph_index) {
	u32 offset = get_glyph_offset(ft, glyph_index);
	unsigned char* glyph_start = (unsigned char*)ft->glyf + offset;
	glyph_outline outline = {0};
	outline.numberOfContours = READ_BE16_MOVE(glyph_start);
	outline.xMin = READ_BE16_MOVE(glyph_start);
	outline.yMin = READ_BE16_MOVE(glyph_start);
	outline.xMax = READ_BE16_MOVE(glyph_start);
	outline.yMax = READ_BE16_MOVE(glyph_start);

	outline.endPtsOfContours = (u16*) calloc(1, outline.numberOfContours*sizeof(u16));
	for(int i = 0; i < outline.numberOfContours; ++i) {
		outline.endPtsOfContours[i] = READ_BE16_MOVE(glyph_start);
	}

	outline.instructionLength = READ_BE16_MOVE(glyph_start);
	outline.instructions = (u8*)calloc(1, outline.instructionLength);
	memcpy(outline.instructions, glyph_start, outline.instructionLength);
	P_MOVE(glyph_start, outline.instructionLength);

	int last_index = outline.endPtsOfContours[outline.numberOfContours-1];
	outline.flags = (glyph_flag*) calloc(1, last_index + 1);

	for(int i = 0; i < (last_index + 1); ++i) {
		outline.flags[i].flag = *glyph_start;
		glyph_start++;
		if(outline.flags[i].repeat) {
			u8 repeat_count = *glyph_start;
			while(repeat_count-- > 0) {
				i++;
				outline.flags[i] = outline.flags[i-1];
			}
			glyph_start++;
		}
	}


	outline.xCoordinates = (i16*) calloc(1, (last_index+1)*2);
	i16 prev_coordinate = 0;
	i16 current_coordinate = 0;
	for(int i = 0; i < (last_index+1); ++i) {
		int flag_combined = outline.flags[i].x_short << 1 | outline.flags[i].x_short_pos;
		switch(flag_combined) {
			case 0: {
				current_coordinate = READ_BE16_MOVE(glyph_start);
			} break;
			case 1: { current_coordinate = 0; }break;
			case 2: { current_coordinate = (*glyph_start++)*-1; }break;
			case 3: { current_coordinate = (*glyph_start++); } break;
		}

		outline.xCoordinates[i] = current_coordinate + prev_coordinate;
		prev_coordinate = outline.xCoordinates[i];
	}

	outline.yCoordinates = (i16*) calloc(1, (last_index+1)*2);
	current_coordinate = 0;
	prev_coordinate = 0;
	for(int i = 0; i < (last_index+1); ++i) {
		int flag_combined = outline.flags[i].y_short << 1 | outline.flags[i].y_short_pos;
		switch(flag_combined) {
			case 0: {
				current_coordinate = READ_BE16_MOVE(glyph_start);
			} break;
			case 1: { current_coordinate = 0; }break;
			case 2: { current_coordinate = (*glyph_start++)*-1; }break;
			case 3: { current_coordinate = (*glyph_start++); } break;
		}

		outline.yCoordinates[i] = current_coordinate + prev_coordinate;
		prev_coordinate = outline.yCoordinates[i];
	}

	return outline;
}

void print_glyph_outline(glyph_outline *outline) {
	printf("#contours\t(xMin,yMin)\t(xMax,yMax)\tinst_length\n");
	printf("%9d\t(%d,%d)\t\t(%d,%d)\t%d\n", outline->numberOfContours,
			outline->xMin, outline->yMin,
			outline->xMax, outline->yMax,
			outline->instructionLength);

	printf("#)\t(  x  ,  y  )\n");
	int last_index = outline->endPtsOfContours[outline->numberOfContours-1];
	for(int i = 0; i <= last_index; ++i) {
		printf("%d)\t(%5d,%5d)\n", i, outline->xCoordinates[i], outline->yCoordinates[i]);
	}
	printf("instructions:\n");
	for(int i = 0; i < outline->instructionLength; ++i) {
		char end= i == outline->instructionLength-1 ? '\n' : ' ';
		printf("%d%c", outline->instructions[i], end);
	}
}

int* getPos(glyph_outline* outline, int x, int y){
	int* result = malloc(2 * sizeof(int));
	if (result != NULL) {
		result[0] = x - outline->xMin;
		result[1] = (outline->yMax - outline->yMin) - (y - outline->yMin);
	}
	return result;
}

void drawPixel(bitmap bmp, glyph_outline* outline, int x, int y, u8 color){
	int* pos=getPos(outline,x,y);
	if(pos[1] >= 0 && pos[0] >= 0 && pos[0] <= bmp.width && pos[1] <= bmp.height){
		bmp.pixels[(pos[1]*bmp.width)+pos[0]]=color;
	}
}

void drawPoint(bitmap bmp, glyph_outline* outline, int x, int y, int size, u8 color){
	int startx=x-size/2;
	int endx=x+size/2;
	int starty=y-size/2;
	int endy=y+size/2;
	for(int i=startx;i<=endx;i++){
		for(int j=starty;j<=endy;j++){
			drawPixel(bmp,outline,i,j,color);
		}
	}
}

void drawLine(bitmap bmp, glyph_outline* outline, int x1, int y1, int x2, int y2) {
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = x1 < x2 ? 1 : -1;
	int sy = y1 < y2 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2;
	int e2;

	while(1) {
		drawPixel(bmp, outline, x1, y1, 255);
		if(x1 == x2 && y1 == y2) break;
		e2 = err;
		if(e2 > -dx) { err -= dy; x1 += sx; }
		if(e2 < dy) { err += dx; y1 += sy; }
	}
}

int getShortDelta(u8 byte, u8 bitPos) {
	int mask = 1 << bitPos;
	int bitValue = (byte & mask) ? 1 : 0;
	return bitValue ? -(mask << 1) : mask;
}

int getSByte(u8 byte) {
	return (byte & 0x80) ? -(byte & 0x7F) : byte;
}

int getPt(int n1 ,int n2 ,float perc){
	int diff=n2-n1;

	return n1+(diff*perc);
}

//draw bezier curve function for when point is on curve
void drawBezier(bitmap bmp, glyph_outline* outline, int x1, int y1, int x2, int y2, int x3, int y3){
	float t=0;
	while(t<=1){
		int x=getPt(getPt(x1,x2,t),getPt(x2,x3,t),t);
		int y=getPt(getPt(y1,y2,t),getPt(y2,y3,t),t);
		drawPixel(bmp,outline,x,y,255);
		t+=0.01;
	}
}

//horizontal line test from given point that'll return how many times it intersects with the outline, how many white pixels it comes across
int hlt(bitmap bmp,int x,int y){
	int count=0;
	int hit=0;
	for(int i=x;i<bmp.width;i++){
		if(bmp.pixels[(y*bmp.width)+i]==255){
			if(hit==0){count++;}
			hit=1;
		}
		else{
			hit=0;
		}
	}
	return count;
}

//flood fill function
// TODO: neg x fill results in segfault error.
char tmp_chr;
void floodFill(bitmap bmp, int x, int y, u8 color){
	// for(int i=0;i<20;i++){
	// 	for(int j=0;j<20;j++){
	// 		bmp.pixels[((y+i)*bmp.width)+(x+j)]=0x80;
	// 	}
	// }
	int cont_neg_x=1;
	int cont_neg_y=1;
	int cont_pos_x=1;
	int cont_pos_y=1;
	if(x-1<0||bmp.pixels[(y*bmp.width)+(x-1)]==color){cont_neg_x=0;}
	if(y-1<0||bmp.pixels[((y-1)*bmp.width)+x]==color){cont_neg_y=0;}
	if(x+1>bmp.width||bmp.pixels[(y*bmp.width)+(x+1)]==color){cont_pos_x=0;}
	if(y+1>bmp.height||bmp.pixels[((y+1)*bmp.width)+x]==color){cont_pos_y=0;}
	if(x>=0&&bmp.pixels[(y*bmp.width)+x]==0){
		bmp.pixels[(y*bmp.width)+x]=color;
		if(cont_neg_x==1){printf("%c: (%d,%d)\n",tmp_chr,x-1,y);floodFill(bmp,x-1,y,color);}
		if(cont_neg_y==1){floodFill(bmp,x,y-1,color);}
		if(cont_pos_x==1){floodFill(bmp,x+1,y,color);}
		if(cont_pos_y==1){floodFill(bmp,x,y+1,color);}
	}
}

void invert(bitmap bmp){
	for(int i=0;i<bmp.width*bmp.height;i++){
		bmp.pixels[i]=255-bmp.pixels[i];
	}
}

//function to loop through each line of the bitmap and return which line has where black pixel shows up at least x value
int getLine(bitmap bmp,int x){
	int low_x=bmp.width;
	int low_y=0;
	for(int i=1;i<bmp.height;i++){
		for(int j=0;j<bmp.width;j++){
			if(bmp.pixels[(i*bmp.width)+j]==255){
				if(j<low_x){low_x=j;low_y=i;}
			}
		}
	}
	return low_y;
}

//function to fill in the bitmap based off of given points on the outline
//fill in area between points and the edge of the bitmap
//don't fill if there is gap in character. (i.e. don't fill in the middle of an O)
//use flood fill algorithm & do horizontal line test to check for gaps (if odd from where point started, then can fill in)
void drawFill(bitmap bmp, glyph_outline* outline){
	int x=0;
	//int y=bmp.height/2;
	int y=getLine(bmp,x);
	printf("y: %d\n",y);
	int count=hlt(bmp,x,y);
	printf("%d\n",count);
	while(count%2==0&&x<=bmp.width/2){
		x++;
		count=hlt(bmp,x,y);
		//printf("trying HLT again: %d\n",count);
	}
	printf("(%d,%d)\n",x,y);
	if(count%2==1){floodFill(bmp,x,y,255);}
}

bitmap get_bitmap_from_glyph_outline(glyph_outline* outline) {
	assert(outline != NULL); // Check for null pointer

	// Calculate bitmap dimensions
	int bitmapWidth = (outline->xMax-outline->xMin)+1;
	int bitmapHeight = (outline->yMax-outline->yMin)+1;

	// Allocate memory for the bitmap
	bitmap result;
	result.pixels = malloc(bitmapWidth * bitmapHeight);
	result.width = bitmapWidth;
	result.height = bitmapHeight;

	// Initialize all pixels to off (0)
	memset(result.pixels, 0, bitmapWidth * bitmapHeight);

	// Process each contour
	for(int i=0;i<outline->numberOfContours;i++){
		int start_index = i == 0 ? 0 : outline->endPtsOfContours[i-1] + 1;
		int end_index = outline->endPtsOfContours[i];
		for(int j=start_index;j<=end_index;j++){
			int x = outline->xCoordinates[j];
			int y = outline->yCoordinates[j];
			int x2 = outline->xCoordinates[j+1];
			int y2 = outline->yCoordinates[j+1];
			if(j==end_index){
				x=outline->xCoordinates[start_index];
				y=outline->yCoordinates[start_index];
				x2=outline->xCoordinates[end_index];
				y2=outline->yCoordinates[end_index];
			}
			if(outline->flags[j].on_curve){
				//drawPoint(result, outline, x, y, 10, 0x88);
				//if(last_index>0){drawBezier(result, outline, outline->xCoordinates[last_index], outline->yCoordinates[last_index], outline->xCoordinates[last_index2], outline->yCoordinates[last_index2], x, y);}
				drawLine(result, outline, x, y, x2, y2);
			} else {
				//drawPoint(result, outline, x, y, 10, 0x88);
				drawLine(result, outline, x, y, x2, y2);
			}
		}
	}
	drawFill(result, outline);
	invert(result);

	return result;
}

/*void save_bitmap_to_file(bitmap* bmp, char* file_name) {
	FILE* file = fopen(file_name, "wb");
	if(file) {
		fprintf(file, "P6\n%d %d\n255\n", bmp->width, bmp->height);
		for(int i = 0; i < bmp->width*bmp->height; ++i) {
			u8 pixel = bmp->pixels[i];
			fwrite(&pixel, 1, 1, file);
			fwrite(&pixel, 1, 1, file);
			fwrite(&pixel, 1, 1, file);
		}
		fclose(file);
	}
}*/
void save_bitmap_to_file(bitmap* bmp, const char* file_name) {
	FILE* file = fopen(file_name, "wb");
	if (file) {
		fprintf(file, "P5\n%d %d\n255\n", bmp->width, bmp->height);

		fwrite(bmp->pixels, bmp->width * bmp->height, 1, file);

		fclose(file);
	}
}

int main(int argc, char** argv) {
	int file_size = 0;
	char* filename="./envy.ttf";
	for(int a=0;a<argc;a++){
		if(strcmp(argv[a],"-h")==0){
			printf("Usage: %s [options]\n", argv[0]);
			printf("Options:\n");
			printf("\t-h\t\tShow this help message\n");
			printf("\t-f <filename>\tSpecify the font file to use\n");
			return 0;
		}
		else if(strcmp(argv[a],"-f")==0){
			a++;
			filename = argc>a ? argv[a] : filename;
		}
	}
	char* file = read_file(filename, &file_size);
	char* mem_ptr = file;

	font_directory ft = {0};
	read_font_directory(file, &mem_ptr, &ft);

	print_cmap(ft.cmap);
	print_format4(ft.f4);
	print_table_directory(ft.tbl_dir, ft.off_sub.numTables);

	for(int a=(int)'A';a<=(int)'Z';a++){
		glyph_outline chr=get_glyph_outline(&ft,get_glyph_index(&ft,(char)a));
		print_glyph_outline(&chr);
		tmp_chr=(char)a;
		bitmap chr_bmp=get_bitmap_from_glyph_outline(&chr);
		save_bitmap_to_file(&chr_bmp,(char[]){'.','/','b','m','p','/',(char)a,'.','b','m','p','\0'});
	}
	return 0;
}
//https://github.com/dluco/ttf
//https://github.com/mooman219/fontdue
//https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6.html
//https://handmade.network/forums/articles/t/7330-implementing_a_font_reader_and_rasterizer_from_scratch%252C_part_1__ttf_font_reader.
//https://learn.microsoft.com/en-us/typography/opentype/spec/glyf
//https://en.wikipedia.org/wiki/TrueType#Basic
//https://learn.microsoft.com/en-us/typography/truetype/
//https://github.com/freetype/freetype/tree/master/include/freetype