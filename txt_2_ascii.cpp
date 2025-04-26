// Badger
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <regex>
#include "../rgb.h"
#include "./font.h"

using namespace std;

string ascii_map[95]={
	"    \n    \n    \n    \n    \n",						//1.  =(char)32
	" _\n| |\n| |\n|_|\n(_)\n",							//2. !=(char)33
	" _ _\n( | )\n V V\n",								//3. "=(char)34
	"   _  _\n _| || |_\n|_  ..  _|\n|_      _|\n  |_||_|\n",			//4. #=(char)35
	"  _\n | |\n/ __)\n\\__ \\\n(   /\n |_|\n",					//5. $=(char)36
	" _  __\n(_)/ /\n  / /\n / /_\n/_/(_)\n",					//6. %=(char)37
	"  ___\n ( _ )\n / _ \\/\\\n| (_>  <\n \\___/\\/\n",				//7. &=(char)38
	" _\n( )\n|/\n",								//8. '=(char)39
	"  __\n / /\n| |\n| |\n| |\n \\_\\\n",						//9. (=(char)40
	"__\n\\ \\\n | |\n | |\n | |\n/_/\n",						//10. )=(char)41
	"__/\\__\n\\    /\n/_  _\\\n  \\/\n",						//11. *=(char)42
	"   _\n _| |_\n|_   _|\n  |_|\n",						//12. +=(char)43
	"   \n   \n   \n( )\n|/\n",							//13. ,=(char)44
	"       \n _____\n|_____|\n",							//14. -=(char)45
	"   \n   \n   \n -\n(_)\n",							//15. .=(char)46
	"    __\n   / /\n  / /\n / /\n/_/\n",						//16. /=(char)47
	"  ___\n / _ \\\n| | | |\n| |_| |\n \\___/\n",					//17. 0=(char)48
	" _\n/ |\n| |\n| |\n|_|\n",							//18. 1=(char)49
	" ____\n|___ \\\n  __) |\n / __/\n|_____|\n",					//19. 2=(char)50
	" _____\n|___ /\n  |_ \\\n ___) |\n|____/\n",					//20. 3=(char)51
	" _  _\n| || |\n| || |_\n|__   _|\n   |_|\n",					//21. 4=(char)52
	" ____\n| ___|\n|___ \\\n ___) |\n|____/\n",					//22. 5=(char)53
	"  __\n / /_\n| '_ \\\n| (_) |\n \\___/\n",					//23. 6=(char)54
	" _____\n|___  |\n   / /\n  / /\n /_/\n",					//24. 7=(char)55
	"  ___\n ( _ )\n / _ \\\n| (_) |\n \\___/\n",					//25. 8=(char)56
	"  ___\n / _ \\\n| (_) |\n \\__, |\n   /_/\n",					//26. 9=(char)57
	"   \n(_)\n _\n(_)\n",								//27. :=(char)58
	" _\n(_)\n _\n( )\n|/\n",							//28. ;=(char)59
	"  __\n / /\n/ /\n\\ \\\n \\_\\\n",						//29. <=(char)60
	"       \n _____\n|_____|\n|_____|\n",						//30. ==(char)61
	"__\n\\ \\\n \\ \\\n / /\n/_/\n",						//31. >=(char)62
	" ___\n|__ \\\n  / /\n |_|\n (_)\n",						//32. ?=(char)63
	"   ____\n  / __ \\\n / / _` |\n| | (_| |\n \\ \\__,_|\n  \\____/\n",		//33. @=(char)64
	"    _\n   / \\\n  / _ \\\n / ___ \\\n/_/   \\_\\\n",				//34. A=(char)65
	" ____\n| __ )\n|  _ \\\n| |_) |\n|____/\n",					//35. B=(char)66
	"  ____\n / ___|\n| |\n| |___\n \\____|\n",					//36. C=(char)67
	" ____\n|  _ \\\n| | | |\n| |_| |\n|____/\n",					//37. D=(char)68
	" _____\n| ____|\n|  _|\n| |___\n|_____|\n",					//38. E=(char)69
	" _____\n|  ___|\n| |_\n|  _|\n|_|\n",						//39. F=(char)70
	"  ____\n / ___|\n| |  _\n| |_| |\n \\____|\n",					//40. G=(char)71
	" _   _\n| | | |\n| |_| |\n|  _  |\n|_| |_|\n",					//41. H=(char)72
	" ___\n|_ _|\n | |\n | |\n|___|\n",						//42. I=(char)73
	"     _\n    | |\n _  | |\n| |_| |\n \\___/\n",					//43. J=(char)74
	" _  __\n| |/ /\n| ' /\n| . \\\n|_|\\_\\\n",					//44. K=(char)75
	" _\n| |\n| |\n| |___\n|_____|\n",						//45. L=(char)76
	" __  __\n|  \\/  |\n| |\\/| |\n| |  | |\n|_|  |_|\n",				//46. M=(char)77
	" _   _\n| \\ | |\n|  \\| |\n| |\\  |\n|_| \\_|\n",				//47. N=(char)78
	"  ___\n / _ \\\n| | | |\n| |_| |\n \\___/\n",					//48. O=(char)79
	" ____\n|  _ \\\n| |_) |\n|  __/\n|_|\n",					//49. P=(char)80
	"  ___\n / _ \\\n| | | |\n| |_| |\n \\__\\_\\\n",				//50. Q=(char)81
	" ____\n|  _ \\\n| |_) |\n|  _ <\n|_| \\_\\\n",					//51. R=(char)82
	" ____\n/ ___|\n\\___ \\\n ___) |\n|____/\n",					//52. S=(char)83
	" _____\n|_   _|\n  | |\n  | |\n  |_|\n",					//53. T=(char)84
	" _   _\n| | | |\n| | | |\n| |_| |\n \\___/\n",					//54. U=(char)85
	"__     __\n\\ \\   / /\n \\ \\ / /\n  \\ V /\n   \\_/\n",			//55. V=(char)86
	"__        __\n\\ \\      / /\n \\ \\ /\\ / /\n  \\ V  V /\n   \\_/\\_/\n",	//56. W=(char)87
	"__  __\n\\ \\/ /\n \\  /\n /  \\\n/_/\\_\\\n",					//57. X=(char)88
	"__   __\n\\ \\ / /\n \\ V /\n  | |\n  |_|\n",					//58. Y=(char)89
	" _____\n|__  /\n  / /\n / /_\n/____|\n",					//59. Z=(char)90
	" __\n| _|\n| |\n| |\n| |\n|__|\n",						//60. [=(char)91
	"    __\n   / /\n  / /\n / /\n/_/\n",						//61. \=(char)92
	" __\n|_ |\n | |\n | |\n | |\n|__|\n",						//62. ]=(char)93
	" /\\\n|/\\|\n",								//63. ^=(char)94
	"       \n       \n       \n _____\n|_____|\n",					//64. _=(char)95
	" _\n( )\n \\|\n",								//65. `=(char)96
	"  __ _\n / _` |\n| (_| |\n \\__,_|\n",						//66. a=(char)97
	" _\n| |__\n| '_ \\\n| |_) |\n|_.__/\n",					//67. b=(char)98
	"  ___\n / __|\n| (__\n \\___|\n",						//68. c=(char)99
	"     _\n  __| |\n / _` |\n| (_| |\n \\__,_|\n",				//69. d=(char)100
	"  ___\n / _ \\\n|  __/\n \\___|\n",						//70. e=(char)101
	"  __\n / _|\n| |_\n|  _|\n|_|\n",						//71. f=(char)102
	"  __ _\n / _` |\n| (_| |\n \\__, |\n |___/\n",		//72. g=(char)103
	" _\n| |__\n| '_ \\\n| | | |\n|_| |_|\n",					//73. h=(char)104
	" _\n(_)\n| |\n| |\n|_|\n",							//74. i=(char)105
	"  (_)\n  | |\n  | |\n _/ |\n|__/\n",						//75. j=(char)106
	" _\n| | __\n| |/ /\n|   <\n|_|\\_\\\n",					//76. k=(char)107
	" _\n| |\n| |\n| |\n|_|\n",							//77. l=(char)108
	" _ __ ___\n| '_ ` _ \\\n| | | | | |\n|_| |_| |_|\n",				//78. m=(char)109
	" _ __\n| '_ \\\n| | | |\n|_| |_|\n",						//79. n=(char)110
	"  ___\n / _ \\\n| (_) |\n \\___/\n",						//80. o=(char)111
	" _ __\n| '_ \\\n| |_) |\n| .__/\n|_|\n",					//81. p=(char)112
	"  __ _\n / _` |\n| (_| |\n \\__, |\n    |_|\n",				//82. q=(char)113
	" _ __\n| '__|\n| |\n|_|\n",							//83. r=(char)114
	" ___\n/ __|\n\\__ \\\n|___/\n",						//84. s=(char)115
	" _\n| |_\n| __|\n| |_\n \\__|\n",						//85. t=(char)116
	" _   _\n| | | |\n| |_| |\n \\__,_|\n",						//86. u=(char)117
	"__   __\n\\ \\ / /\n \\ V /\n  \\_/\n",					//87. v=(char)118
	"__      __\n\\ \\ /\\ / /\n \\ V  V /\n  \\_/\\_/\n",				//88. w=(char)119
	"__  __\n\\ \\/ /\n >  <\n/_/\\_\\\n",						//89. x=(char)120
	" _   _\n| | | |\n| |_| |\n \\__, |\n |___/\n",					//90. y=(char)121
	" ____\n|_  /\n / /\n/___|\n",							//91. z=(char)122
	"   __\n  / /\n | |\n< <\n | |\n  \\_\\\n",					//92. {=(char)123
	" _\n| |\n| |\n| |\n| |\n|_|\n",						//93. |=(char)124
	"__\n\\ \\\n | |\n  > >\n | |\n/_/\n",						//94. }=(char)125
	"       \n       \n   /\\/|\n|/\\/\n"						//95. ~=(char)126
};

vector<string> split_ascii(string ascii, char delim = '\n'){
	vector<string> lines;
	stringstream ss(ascii);
	string line;
	while (getline(ss, line, delim))
	{
		lines.push_back(line);
	}
	return lines;
}

vector<string> split_ascii(string ascii, string delim){
	vector<string> lines;
	string line;
	int pos=0;
	while((pos=ascii.find(delim))!=string::npos){
		line=ascii.substr(0,pos);
		lines.push_back(line);
		ascii.erase(0,pos+delim.length());
	}
	lines.push_back(ascii);
	return lines;
}

int get_max_num(vector<int> v){
	int max = v[0];
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i] > max)
		{
			max = v[i];
		}
	}
	return max;
}

string str_per(string txt, int per){
	string ans="";
	for(int i=0;i<per;i++){
		ans+=txt;
	}
	return ans;
}

string str_per(string txt, string per){
	string ans="";
	for(int a=0;a<txt.length();a++){
		ans+=per;
	}
	return ans;
}

string txt_to_ascii(string input,bool horizontal_reverse=false,bool vertical_reverse=false){
	string output = "";
	vector<vector<string>> chr(input.length(), vector<string>(8));
	vector<int> num_lines;
	vector<int> max_len_line(input.length());
	for (int i = 0; i < chr.size(); i++){
		vector<string> ascii_lines = split_ascii(ascii_map[input[i] - ' ']);
		num_lines.push_back(ascii_lines.size());
		for (int j = 0; j < ascii_lines.size(); j++) {
			chr[i][j]=ascii_lines[j];
		}
	}
	for (int i = 0; i < chr.size(); i++){
		for (int j = 0; j < chr[i].size(); j++){
			if (chr[i][j].length() > max_len_line[i]){
				max_len_line[i] = chr[i][j].length();
			}
		}
	}
	int size=get_max_num(num_lines);
	for(int j=(vertical_reverse?size-1:0);vertical_reverse?j>=0:j<size;j+=(vertical_reverse?-1:1)){
		for(int i=(horizontal_reverse?chr.size()-1:0);horizontal_reverse?i>=0:i<chr.size();i+=(horizontal_reverse?-1:1)){
			bool is_alpha=(input[i]>='a'&&input[i]<='z')||(input[i]>='A'&&input[i]<='Z');
			int index=is_alpha?j-(size-num_lines[i]):j;
			if(size-num_lines[i]>j&&is_alpha){output+=str_per(" ",max_len_line[i])+" ";continue;}
			string pad=(chr[i][index].length()<max_len_line[i]&&chr[i][index]!="")?str_per(" ",max_len_line[i]-chr[i][index].length()):"";
			pad+=(chr[i][index]=="")?str_per(" ",max_len_line[i])+(input[i]=='m'?"":" "):" ";
			if(input[i]=='m'&&chr[i][index]==""){pad+=" ";}
			output += chr[i][index]+pad;
		}
		output += "\n";
	}

	return output;
}

void write_to_file(string filename, string text){
	ofstream file;
	file.open(filename);
	file << text;
	file.close();
}
void set_file_text(string filename, string text){
	ofstream file;
	file.open(filename,ofstream::out|ofstream::trunc);
	file << text;
	file.close();
}

string ascii_to_txt(string ascii){
	string output="";
	vector<string> lines=split_ascii(ascii);
	for(int i=0;i<lines.size();i++){
		string line=lines[i];
		if(line==""){
			output+="\n";
		}
		else{
			output+=line+"\\n";
		}
	}
	return output;
}

string ascii_file_to_txt(string filename){
	ifstream file(filename);
	string line;
	string output="";
	while(getline(file,line)){
		if(line==""){
			output+="\n";
		}
		else{
			output+=line+"\\n";
		}
	}
	return output;
}

string get_file_content(string filename){
	ifstream file(filename);
	string line;
	string output="";
	while(getline(file,line)){
		output+=line+"\n";
	}
	return output;
}

// TODO: fix
/*string get_ascii(vector<string> txt){
	string output="";
	vector<vector<string>> chr(txt.size(),vector<string>(6));
	vector<int> num_lines;
	vector<int> max_len_line(txt.size());
	for(int i=0;i<chr.size();i++){
		vector<string> ascii_lines=split_ascii(txt[i],"\\n");
		num_lines.push_back(ascii_lines.size());
		for(int j=0;j<ascii_lines.size();j++){
			chr[i][j]=ascii_lines[j];
		}
	}
	cout<<"test"<<endl;
	for(int i=0;i<chr.size();i++){
		for(int j=0;j<chr[i].size();j++){
			if(chr[i][j].length()>max_len_line[i]){
				max_len_line[i]=chr[i][j].length();
			}
		}
	}
	int size=get_max_num(num_lines);
	vector<string> last_line(chr.size());
	for(int j=0;j<size;j++){
		for(int i=0;i<chr.size();i++){
			string pad=(chr[i][j].length()<max_len_line[i]&&chr[i][j]!="")?str_per(" ",max_len_line[i]-chr[i][j].length()):"";
			pad+=(chr[i][j]=="")?str_per(last_line[i]," ")+" ":" ";
			output+=chr[i][j]+pad;
			last_line[i]=chr[i][j];
		}
		output+="\n";
	}
	return output;
}*/

string get_ascii_chr(string txt){
	vector<string> chr=split_ascii(txt,'\n');
	string output="";
	for(int i=0;i<chr.size();i++){
		vector<string> lines=split_ascii(chr[i],"\\n");

		for(int a=0;a<lines.size();a++){
			output+=lines[a]+"\n";
		}
	}
	return output;
}

string get_ascii(string txt){
	vector<string> chr=split_ascii(txt,'\n');
	string output="";
	for(int i=0;i<chr.size();i++){
		//vector<string> lines=split_ascii(chr[i],"\\n");
		//string num=txt_to_ascii(to_string(i+1)+". ");
		//output+=get_ascii_chr(txt_to_ascii(to_string(i+1)))+chr[i]+"\n";
		//TODO: use get_ascii to get do ascii art for numbers and dot
			//-then the ascii art for the character would display next to it

		/*for(int a=0;a<lines.size();a++){
			output+=lines[a]+"\n";
		}*/
		output+=txt_to_ascii(to_string(i+1)+". "+(char)(' '+i))+"\n";
	}
	return output;
}

string get_replace(const string& txt,const string& find,const string& replace){
	string result=txt;
	if(find[0]!='\b'&&find[txt.length()-1]!='\b'){
		size_t pos=0;
		while((pos=result.find(find,pos))!=string::npos){
			result.replace(pos,find.length(),replace);
			pos+=replace.length();
		}
	}
	else{
		regex find_regex(find.substr(1,find.length()-2));
		smatch match;
		while(regex_search(result,match,find_regex)){
			result=match.prefix().str()+replace+match.suffix().str();
		}
	}
	return result;
}

string get_replace(const string& txt,const string& find,string (*replace)(string)){
	string result=txt;
	regex find_regex(find);
	smatch match;
	while(regex_search(result,match,find_regex)){
		result=match.prefix().str()+replace(get_replace(get_replace(match[0],"\b\\\\(.*?)\\{\b",""),"}",""))+match.suffix().str();
	}
	return result;
}

int get_len(const string str){
	int count=0;
	int pos=0;
	for(char c:str){
		if(c=='\t'){
			int spaces=8-(pos%8);
			count+=spaces;
			pos+=spaces;
		}
		else{
			count++;
			pos++;
		}

		if(c=='\n'){
			pos=0;
		}
	}
	return count;
}

string get_tabs2spaces(const string str,const int tab_size=8){
	string ans="";
	int pos=0;
	for(char c:str){
		if(c=='\t'){
			int spaces=tab_size-(pos%tab_size);
			ans+=str_per(" ",spaces);
			pos+=spaces;
		}
		else{
			ans+=c;
			pos++;
		}

		if(c=='\n'){
			pos=0;
		}
	}
	return ans;
}

string get_plain_txt(const string str){
	string ans="";
	bool is_clr=false;
	for(char c:str){
		if(c=='\033'){
			is_clr=true;
		}
		else if(is_clr){
			if(c=='m'){
				is_clr=false;
			}
		}
		else{
			ans+=c;
		}
	}
	return ans;
}

string get_code(vector<string> code, vector<string> comments){
	string output="";
	vector<string> code_plain=code;
	for(int i=0;i<code.size();i++){
		code_plain[i]=get_plain_txt(code[i]);
	}

	vector<int> max_line(code_plain.size());
	for(int i=0;i<code_plain.size();i++){
		if(code_plain[i].length()>max_line[i]){
			max_line[i]=get_len(code_plain[i]+"\t");
		}
	}
	int max=get_max_num(max_line);
	int line=0;
	for(int i=0;i<max_line.size();i++){
		if(max_line[i]==max){line=i;break;}
	}
	max=max+(get_len(code_plain[line]+"\t")-code_plain[line].length());
	for(int i=0;i<code_plain.size();i++){
		int tab=(max-max_line[i])/8;
		string tabs="";
		for(int j=0;j<tab;j++){
			tabs+="\t";
		}
		output+=code[i]+tabs+comments[i]+"\n";
	}
	return output;
}

string txt_to_code(const string txt, const char start=' '){
	string txt_fmt=get_replace(get_replace(txt,"\\","\\\\"),"\\\\n","\\n");
	vector<string> lines=split_ascii(txt_fmt);
	string output="";
	vector<string> comments;
	int i=0;
	for(i=0;i<lines.size();i++){
		string end=i==lines.size()-1?"":",";
		output+="\t"+(string)orange+"\""+lines[i]+"\""+"\033[0m"+end+"\n";
		comments.push_back((string)green+"//"+to_string(i+1)+". "+string(1,(char)(start+i))+"=(char)"+to_string((int)(start+i))+"\033[0m");
	}
	return (string)rgb8(0,128,128)+"string"+"\033[0m"+" "+(string)rgb8(125,125,125)+"ascii_map"+"\033[0m["+(string)rgb8(85,85,85)+to_string(i)+"\033[0m]={\n"+get_code(split_ascii(output),comments)+"};";
}

void set_ascii_map(string filename){
	string txt=ascii_file_to_txt(filename);
	string txt_fmt=txt;
	vector<string> lines=split_ascii(txt_fmt);
	for(int i=0;i<lines.size();i++){
		if(i>=95){throw("\"ascii_map["+to_string(i)+"]\" does not exist! Max size: 95 (index 94).");break;}
		ascii_map[i]=get_replace(lines[i],"\\n","\n");
	}
}

bool has_letter(string word,char letter){
	for (int i=0;i<word.length();i++){
		if(word[i]==letter){
			return true;
		}
	}
	return false;
}

/*string get_num(string txt){
	string ans="";
	for(int i=0;i<txt.length();i++){
		if(txt[i]>='0'&&txt[i]<='9'){
			ans+=txt[i];
		}
		else{
			throw(txt[i]);
		}
	}
	return ans;
}*/

string get_num(string txt, string after=". "){
	string ans="";
	for(int i=0;i<txt.length();i++){
		if(txt[i]>='0'&&txt[i]<='9'){
			ans+=txt[i];
		}
		else{
			if(after!=""){
				if(txt.substr(i,after.length())==after){
					return ans;
				}
			}
			else{
				return ans;
			}
		}
	}
	return ans;
}

vector<string> get_roman_numerals(string txt){
	vector<string> numerals;
	// TODO: fix regex to make sure spacings between roman numerals, but skip over because i still want newline and tabs to show
	regex roman_numeral("(?!\\s+|\\\\t+|\\\\n+)(?=[MDCLXVI])M*(C[MD]|D?C{0,3})(X[CL]|L?X{0,3})(I[XV]|V?I{0,3})(?<!\\s+|\\\\t+|\\\\n+)");
	smatch match;
	while(regex_search(txt,match,roman_numeral)){
		numerals.push_back(match[0]);
		txt=match.suffix().str();
	}
	return numerals;
}

int roman_to_num(string roman){
	int ans=0;
	vector<int> nums;
	for(int i=0;i<roman.length();i++){
		switch(roman[i]){
			case 'I':
				nums.push_back(1);
				break;
			case 'V':
				nums.push_back(5);
				break;
			case 'X':
				nums.push_back(10);
				break;
			case 'L':
				nums.push_back(50);
				break;
			case 'C':
				nums.push_back(100);
				break;
			case 'D':
				nums.push_back(500);
				break;
			case 'M':
				nums.push_back(1000);
				break;
			default:
				throw(roman[i]);
		}
	}
	for(int i=0;i<nums.size();i++){
		if(i+1<nums.size()){
			if(nums[i]<nums[i+1]){
				ans-=nums[i];
			}
			else{
				ans+=nums[i];
			}
		}
		else{
			ans+=nums[i];
		}
	}
	return ans;
}

string get_roman_numeral(string txt){
	vector<string> numerals=get_roman_numerals(txt);
	string ans=txt;
	for(int i=0;i<numerals.size();i++){
		ans=get_replace(ans,numerals[i],to_string(roman_to_num(numerals[i])));
	}
	return ans;
}

string get_cmd_out(const string cmd){
	char buffer[128];
	string result="";

	FILE* pipe=popen(cmd.c_str(),"r");

	if(pipe){
		while(!feof(pipe)){
			if(fgets(buffer,128,pipe)!=NULL){result+=buffer;}
		}

		pclose(pipe);
	}

	return result;
}

string get_date(){
	time_t now = time(0);
	tm *ltm = localtime(&now);
	string year=to_string(1900 + ltm->tm_year);
	string month=to_string(1 + ltm->tm_mon);
	string day=to_string(ltm->tm_mday);
	return year+"/"+month+"/"+day;
}

vector<string> get_storages(string txt){
	vector<string> storage;
	regex storage_regex("[0-9]+(\\.[0-9]+)?(\\s)?(b|kb|mb|gb|tb|pb|eb|zb|yb)");
	smatch match;
	while(regex_search(txt,match,storage_regex)){
		storage.push_back(match[0]);
		txt=match.suffix().str();
	}
	return storage;
}

int get_storage_convert(int num,string type,string type_to){
	vector<string> types={"b","kb","mb","gb","tb","pb","eb","zb","yb"};
	int index=0;
	for(int i=0;i<types.size();i++){
		if(type==types[i]){
			index=i;
			break;
		}
	}
	int index_to=0;
	for(int i=0;i<types.size();i++){
		if(type_to==types[i]){
			index_to=i;
			break;
		}
	}
	int ans=num;
	if(index<index_to){
		for(int i=index;i<index_to;i++){
			ans*=1024;
		}
	}
	else if(index>index_to){
		for(int i=index;i>index_to;i--){
			ans/=1024;
		}
	}
	return ans;
}

string get_storage(string txt){
	vector<string> storage=get_storages(txt);
	string ans=txt;
	for(int i=0;i<storage.size();i++){
		string num=get_num(storage[i]);
		string type=storage[i].substr(num.length());
		string type_to="b";
		if(type=="kb"){type_to="kb";}
		else if(type=="mb"){type_to="mb";}
		else if(type=="gb"){type_to="gb";}
		else if(type=="tb"){type_to="tb";}
		else if(type=="pb"){type_to="pb";}
		else if(type=="eb"){type_to="eb";}
		else if(type=="zb"){type_to="zb";}
		else if(type=="yb"){type_to="yb";}
		ans=get_replace(ans,storage[i],to_string(get_storage_convert(stoi(num),type,type_to)));
	}
	return ans;
}

string get_num_fmt(string num){
	string ans="";
	int count=0;
	for(int i=num.length()-1;i>=0;i--){
		if(count==3){
			ans=","+ans;
			count=0;
		}
		ans=num[i]+ans;
		count++;
	}
	return ans;
}
string get_num_fmt2(string num_str) {
	double num=stod(num_str);
	int exp=0;
	string suffixes[]={"","k","m","b","t"};

	while(num>=1000){
		num/=1000;
		exp++;
	}

	return to_string(num)+suffixes[exp];
}

string get_lower_case(string txt){
	string ans="";
	for(int i=0;i<txt.length();i++){
		if(txt[i]>='A'&&txt[i]<='Z'){
			ans+=txt[i]+32;
		}
		else{
			ans+=txt[i];
		}
	}
	return ans;
}
string get_upper_case(string txt){
	string ans="";
	for(int i=0;i<txt.length();i++){
		if(txt[i]>='a'&&txt[i]<='z'){
			ans+=txt[i]-32;
		}
		else{
			ans+=txt[i];
		}
	}
	return ans;
}
string get_title_case(string txt){
	string ans="";
	bool is_space=true;
	for(int i=0;i<txt.length();i++){
		if(is_space){
			if(txt[i]>='a'&&txt[i]<='z'){
				ans+=txt[i]-32;
			}
			else{
				ans+=txt[i];
			}
			is_space=false;
		}
		else{
			if(txt[i]==' '){
				is_space=true;
			}
			ans+=txt[i];
		}
	}
	return ans;
}

string chr_range(char start,char end,int step=1,int pos_pad=0,char pad='\0'){
	string ans="";
	for(int i=start;i<=end;i+=step){
		ans+=(char)i;
		if(pos_pad!=0&&i%pos_pad==0){
			ans+=pad;
		}
	}
	return ans;
}

int main(int argc, char** argv){
	string input=chr_range('a','z',1,5,'\n');
	string filename="";
	bool horizontal_reverse=false;
	bool vertical_reverse=false;
	for(int i=0;i<argc;i++){
		if(string(argv[i])=="-h"){
			cout<<txt_to_ascii("HELP:")<<endl<<endl;
			cout<<argv[0]<<" [options]"<<endl<<endl;
			cout<<"Options:"<<endl;
			cout<<"-t <text> : text to convert to ascii art"<<endl;
			cout<<"-w <filename=\"./txt_2_ascii.txt\"> : filename to write ascii art to"<<endl;
			cout<<"-i <filename> : filename to take as input text to be converted"<<endl;
			cout<<"-c <filename> : filename to convert ascii art font file to code"<<endl;
			cout<<"-r <reverse=\"h\"> : reverse the ascii art horizontally or vertically\n\t* h: horizontal\n\t* v: vertical\n\t* hv: horizontal & vertical"<<endl;
			cout<<"-f <filename=\"./std_font.txt\"> : load font style from file & use for ascii art"<<endl;
			cout<<"-cf <filename=\"./font.txt\",fill=\'#\',blank=\' \'> : create font style file"<<endl;
			cout<<"-sf <filename=\"./std_font.txt\"> : set font file to standard font"<<endl;
			return 0;
		}
		else if((string)argv[i]=="-t"){
			i++;
			input=argv[i];
		}
		else if(string(argv[i])=="-w"){
			i++;
			if(argc>i){
				filename=argv[i];
			}
			else{
				filename="./txt_2_ascii.txt";
			}
		}
		else if((string)argv[i]=="-i"){
			i++;
			if(argc>i){
				input=get_file_content(argv[i]);
			}
			else{
				cout<<"No file specified as input text for \"-i\" in command line arguments!";
				return 0;
			}
		}
		else if(string(argv[i])=="-c"){
			i++;
			if(argc>i){
				string txt=ascii_file_to_txt(argv[i]);
				cout<<txt_to_code(txt);
				cout<<endl<<endl;
				try{set_ascii_map(argv[i]);}
				catch(string err){cerr<<err<<endl;return 0;}
				cout<<get_ascii(txt);
				return 0;
			}
			else{
				cout<<"No file specified\n";
				return 0;
			}
		}
		else if(string(argv[i])=="-r"){
			i++;
			if(argc>i){
				string reverse=argv[i];
				horizontal_reverse=has_letter(reverse,'h');
				vertical_reverse=has_letter(reverse,'v');
			}
			else{
				horizontal_reverse=true;
			}
		}
		else if(string(argv[i])=="-f"){
			i++;
			if(argc>i){
				try{set_ascii_map(argv[i]);}
				catch(string err){cerr<<err<<endl;return 0;}
			}
			else{
				try{set_ascii_map("./std_font.txt");}
				catch(string err){cerr<<err<<endl;return 0;}
			}
		}
		else if(string(argv[i])=="-cf"){
			i++;
			vector<string> def={"./font.txt","#"," "};
			vector<string> split=argc>i?split_ascii(argv[i],','):def;
			string filename=split[0];
			char fill=split.size()>1?split[1][0]:def[1][0];
			char blank=split.size()>2?split[2][0]:def[2][0];
			createFontFile(filename,fill,blank);
		}
		else if(string(argv[i])=="-sf"){
			i++;
			string filename=argc>i?argv[i]:"./std_font.txt";
			set_file_text(filename,std_font);
		}
	}

	int counter=0;
	bool is_one=false;
	vector<string> num_split=split_ascii(input,"1. ");
	string input_fmt=num_split.empty()?input:num_split[0];
	for(int a=1;a<num_split.size();a++){
		if(!is_one){counter++;}
		else{counter=1;}
		string num=to_string(counter);
		input_fmt+=num+". "+num_split[a];

		string num2=get_num(num_split[a]);
		if(to_string(counter+1)==num2){counter++;}
		else if(num2!=""){is_one=true;}
		else{is_one=false;}
	}
	input_fmt=get_replace(input_fmt,"\\\\0\\{\\d+\\}",get_num_fmt);
	input_fmt=get_replace(input_fmt,"\\\\1\\{(\\d+|\\d{1,3}(,\\d{3})*)(\\.\\d+)?+\\}",get_num_fmt2);
	input_fmt=get_replace(input_fmt,"\\\\l\\{(.*?)\\}",get_lower_case);
	input_fmt=get_replace(input_fmt,"\\\\u\\{(.*?)\\}",get_upper_case);
	input_fmt=get_replace(input_fmt,"\\\\ul\\{(.*?)\\}",get_title_case);
	input_fmt=get_tabs2spaces(get_replace(input_fmt,"\\t","\t"),4);
	//input_fmt=get_roman_numeral(input_fmt);
	input_fmt=get_replace(input_fmt,"\\d",get_date());
	input_fmt=get_replace(input_fmt,"\\u",getenv("USER"));
	input_fmt=get_replace(input_fmt,"\\h",get_cmd_out("echo -n \"$(hostname)\""));
	input_fmt=get_replace(input_fmt,"\\p",getenv("PWD"));
	vector<string> txt=split_ascii(input_fmt,"\\n");
	string ascii_art="";
	for(int i=0;i<txt.size();i++){
		ascii_art+=i==txt.size()-1?txt_to_ascii(txt[i],horizontal_reverse,vertical_reverse):txt_to_ascii(txt[i],horizontal_reverse,vertical_reverse)+"\n";
	}
	cout<<ascii_art;

	if(filename!=""){
		write_to_file(filename,ascii_art);
	}

	return 0;
}