// Badger
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include <cmath>

using namespace std;

typedef unsigned char u8;

typedef struct{
	u8* pixels;
	int width;
	int height;
}bitmap;
int get_size(bitmap bmp){
	return bmp.width*bmp.height;
}

short int get_len(string a1){
	return a1.length();
}
string get_reverse(const string a1){
	string ans="";
	for(int a=get_len(a1)-1;a>=0;a--){
		ans+=a1[a];
	}
	return ans;
}
string get_prefix(int base) {
	switch (base) {
		case 2: return "0b";
		case 8: return "0";
		case 16: return "0x";
		default: return "";
	}
}
unsigned int get_digits(const unsigned int num,const unsigned int base=2){
	unsigned int n1=num;
	unsigned int digits=1;
	while(n1>0){
		n1/=base;
		digits++;
	}
	return digits;
}
string get_base(unsigned int num,unsigned int base=2,unsigned int digits=NULL){
	unsigned int min_digits=get_digits(num,base);
	if(digits<min_digits){digits=min_digits;}
	string hex_digits = "0123456789abcdef";
	string base_num = "";
	int j = 0;
	while(j<digits){
		base_num = hex_digits[num % base] + base_num;
		num /= base;
		j++;
	}
	return base_num;
}
int get_base10(string num, int base) {
	int base10 = 0;
	int j = 0;
	for (int i = num.length() - 1; i >= 0; i--) {
		if (num[i] >= '0' && num[i] <= '9') {
			base10 += (num[i] - '0') * pow(base, j);
		}
		else if (num[i] >= 'A' && num[i] <= 'F') {
			base10 += (num[i] - 'A' + 10) * pow(base, j);
		}
		j++;
	}
	return base10;
}
string get_num(string num, int base, int newBase, unsigned short int digits=NULL, bool prefix=true) {
	int base10 = get_base10(num, base);
	return (prefix?get_prefix(newBase):"") + get_base(base10, newBase, digits);
}

char* bitmap_to_charmap(bitmap bmp){
	char* charmap=new char[8];
	for(int i=0;i<8;i++){
		charmap[i]=0;
		for(int j=0;j<8;j++){
			bool is_black=bmp.pixels[i*8+j]<=0x40;
			charmap[i]|=is_black<<7-j;
		}
	}
	return charmap;
}

bitmap remove_white_boarders(const bitmap& input){
	bitmap output;

	int minX = input.width;
	int minY = input.height;
	int maxX = 0;
	int maxY = 0;

	for (int y = 0; y < input.height; ++y) {
		for (int x = 0; x < input.width; ++x) {
			if (input.pixels[y * input.width + x] < 0x88) {
				if(x<minX){minX=x;}
				if(y<minY){minY=y;}

				if(x>maxX){maxX=x;}
				if(y>maxY){maxY=y;}
			}
		}
	}

	output.width = maxX - minX + 1;
	output.height = maxY - minY + 1;
	output.pixels = new uint8_t[output.width * output.height];

	for (int y = minY; y <= maxY; ++y) {
		for (int x = minX; x <= maxX; ++x) {
			output.pixels[(y - minY) * output.width + (x - minX)] = input.pixels[y * input.width + x];
		}
	}

	return output;
}

bitmap resize_bitmap(const bitmap& bmp, int new_width, int new_height) {
	bitmap resized_bmp;

	resized_bmp.pixels = new uint8_t[new_width * new_height];
	resized_bmp.width = new_width;
	resized_bmp.height = new_height;

	for (int y = 0; y < new_height; ++y) {
		for (int x = 0; x < new_width; ++x) {
			int src_x = (x * bmp.width) / new_width;
			int src_y = (y * bmp.height) / new_height;

			int src_index = src_y * bmp.width + src_x;
			int dst_index = y * new_width + x;
			resized_bmp.pixels[dst_index] = bmp.pixels[src_index];
		}
	}

	return resized_bmp;
}

void save_bitmap_to_file(bitmap* bmp, string file_name) {
	FILE* file = fopen(file_name.c_str(), "wb");
	if (file) {
		fprintf(file, "P5\n%d %d\n255\n", bmp->width, bmp->height);

		fwrite(bmp->pixels, bmp->width * bmp->height, 1, file);

		fclose(file);
	}
}

int get_file_size(const string filename) {
	ifstream file(filename, ios::binary | ios::ate);
	if (!file.is_open()) {
		cerr << "Error opening file: " << filename << endl;
		return -1;
	}

	int file_size = file.tellg();
	file.close();
	return file_size;
}

string get_filename(const string filename) {
	string file_name = filename;
	size_t last_index = file_name.find_last_of(".");
	file_name = file_name.substr(0, last_index);
	return file_name;
}

bitmap read_bitmap_from_file(string file_name) {
	bitmap bmp;

	FILE* file = fopen(file_name.c_str(), "rb");
	if (file) {
		char header[256];
		fgets(header, 256, file);

		fscanf(file, "%d %d\n", &bmp.width, &bmp.height);

		fgets(header, 256, file);

		bmp.pixels = new uint8_t[bmp.width * bmp.height];
		fread(bmp.pixels, bmp.width * bmp.height, 1, file);

		fclose(file);
	}

	return bmp;
}

int main(int argc,char** argv){
	/*if(argc < 2){
		cout << "Usage: " << argv[0] << " <bitmap file>" << endl;
		return 0;
	}
	ifstream bmpfile(argv[1],ios::binary);
	if(!bmpfile.is_open()){
		cout << "Error: cannot open file " << argv[1] << endl;
		return 0;
	}*/
	/*bmpfile.seekg(0,bmpfile.end);
	int length = bmpfile.tellg();
	bmpfile.seekg(0,bmpfile.beg);
	u8* buffer = new u8[length];
	bmpfile.read((char*)buffer,length);
	bmpfile.close();
	bitmap bmp;
	bmp.width = *(int*)&buffer[18];
	bmp.height = *(int*)&buffer[22];
	bmp.pixels = new u8[get_size(bmp)];*/
	//bitmap bmp = read_bitmap_from_file(argv[1]);
	/**
	 * 50 50 : 2 : 7654B
	 * 65 73 : 1 : 14362B
	 * 58 66 : 2
	 * 50 58 : 2
	 * 35 43 : 3
	 * 20 28 : 0
	 * 10 10 : 2 : 374B
	*/
	// FIXME: fix setting bitmap pixels from input bitmap image file
	/*cout<<bmp.width<<"x"<<bmp.height<<" dimensions : "<<abs(bmp.width-bmp.height)<<endl;
	cout<<get_file_size(argv[1])<<"B storage"<<endl;
	cout<<get_file_size(argv[1])/3<<" vs. "<<bmp.width*bmp.height<<" pixels : ";
	int padding_bytes = (get_file_size(argv[1])/3)-(bmp.width*bmp.height);
	cout<<padding_bytes<<endl;
	cout<<get_file_size(argv[1])%padding_bytes<<" padding bytes"<<endl;*/
	/*int multiplier = 1;
	//int multiplier = get_file_size(argv[1])%3;
	int line=0;
	for(int i=0;i<get_size(bmp);i++){
		bmp.pixels[i]=buffer[(i*3)+(padding_bytes+(line*multiplier))];
		if(i%bmp.width==0){line++;}
	}
	for(int i=0;i<bmp.height/2;i++){
		for(int j=0;j<bmp.width;j++){
			u8 temp = bmp.pixels[i*bmp.width+j];
			bmp.pixels[i*bmp.width+j] = bmp.pixels[(bmp.height-i-1)*bmp.width+j];
			bmp.pixels[(bmp.height-i-1)*bmp.width+j] = temp;
		}
	}*/
	string code="";
	for(char a='A';a<='Z';a++){
		string filename="./bmp/"+string(1,a)+".bmp";
		bitmap bmp = read_bitmap_from_file(filename);
		save_bitmap_to_file(&bmp,"./bmp2/bmp_not_resized.bmp");
		bitmap bmp_no_white = remove_white_boarders(bmp);
		save_bitmap_to_file(&bmp_no_white,"./bmp2/bmp_no_white.bmp");
		// TODO: not just 8x8 but any size (get to work w/ ASCII converter as well; dynamically change const value 8 which determines line #)
		bitmap bmp8x8 = resize_bitmap(bmp_no_white,8,8);
		save_bitmap_to_file(&bmp8x8,"./bmp2/bmp_resized.bmp");
		for(int a=0;a<get_size(bmp8x8);a++){
			if(a%bmp8x8.width==0){cout<<endl;}
			string clr=bmp8x8.pixels[a]<=0x40?"\033[0;31m":"\033[0;37m";
			cout<<clr<<(int)bmp8x8.pixels[a]<<"\033[0m\t";
		}
		char* charmap=bitmap_to_charmap(bmp8x8);
		string hex="{";
		for(int a=0;a<=get_size(bmp8x8);a++){
			if(a!=0&&a%bmp8x8.width==0){
				int line=(a/bmp8x8.width)-1;
				string end=(a==bmp8x8.width*bmp8x8.height?"":",");
				hex+=get_num(to_string((int)charmap[line]),10,16,2)+end;
				cout<<":"<<get_num(to_string((int)charmap[line]),10,2,8)<<endl;
			}
			if(a!=get_size(bmp8x8)){cout<<(bmp8x8.pixels[a]<=0x40?"#":" ");}
		}
		string end=a=='Z'?"":",";
		hex+="}";
		code+="\t"+hex+end+"\t\t//U+"+get_num(to_string((int)a),10,16,4,false)+" ("+string(1,a)+")\n";
		cout<<hex<<"\t//U+"<<get_num(to_string((int)a),10,16,4,false)<<" ("<<a<<")"<<endl;
	}
	cout<<endl<<"char ascii_map8x8[26][8]={"<<endl;
	cout<<code;
	cout<<"}"<<endl;

	return 0;
}
//https://stackoverflow.com/questions/61360044/reading-data-from-bmp-file-in-c
//https://stackoverflow.com/questions/2750988/c-reading-and-editing-pixels-of-a-bitmap-image
//TODO: provide code for charmap(w/ given dimensions such as 8x8) from given folder where bitmap char images are stored with char as name, then display as "ascii_map8x8=..." code