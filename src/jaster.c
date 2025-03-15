#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "jaster.h"

/* al the values parsed form the json */
char parsed[MAX_LENGTH][MAX_LENGTH] = {0};

/* a schema from the json to use to auto generate struct */
char jen_str[MAX_LENGTH][5] = {0};
static int nested_object_in_array = 0;
static int nested_object_in_object = 0;
static int inside_array = 0;
/*static functions prototype*/
static int parse_object(char **json, char parsed[][MAX_LENGTH], int *step);
static int parse_string(char **json, char parsed[][MAX_LENGTH], int *step);
static int parse_array(char **json, char parsed[][MAX_LENGTH], int *step);
static int parse_literal(char **json, char parsed[][MAX_LENGTH], int *step);
static int parse_number(char **json, char parsed[][MAX_LENGTH], int *step);
static int is_float(char *number);
static FILE *init_header();

int jenerator(int steps, char parsed[][MAX_LENGTH],char jen_str[][5])
{
	FILE *fp = init_header();
	if(!fp){
		fprintf(stderr,"cannot generate code.\n");
		return -1;
	}

	int counter = 0;
	for(int i= 0; i < steps; i++)
		if(strncmp(jen_str[i],OBJk,2) == 0)
			counter++;


	while(counter > 0){
		for(int i = 0; i < steps;i++){
			if(strncmp(jen_str[i],OBJk,2) == 0 ){
				fputs(CODE_STRCT,fp);
				parsed[i][0] = toupper(parsed[i][0]);
				fputs(parsed[i],fp);
				fputs(" {\n",fp);		
				strncpy(jen_str[i],"\0",5);
				for(int j = i, k = i + 1; (jen_str[j][0] != END && jen_str[k][0] != '\0');j++,k++){
					if(jen_str[k][0] == 'v'){
						fputs("\tchar ",fp);
						fputs(parsed[j],fp);
						fputs("[MAX_STRING_LENGTH];\n",fp);
						continue;
					}

					if(jen_str[k][0] == 'l'){
						if(strncmp(parsed[k],"null",4) == 0){
							fputs("\tchar ",fp);
							fputs(parsed[j],fp);
							fputs("[MAX_STRING_LENGTH];\n",fp);
							continue;
						}
						fputs("\tint ",fp);
						fputs(parsed[j],fp);
						fputs(";\n",fp);
						continue;
					}

					if(jen_str[k][0] == 'n'){
						if(is_float(parsed[k])){
							fputs("\tdouble ",fp);
							fputs(parsed[j],fp);
							fputs(";\n",fp);
							continue;
						}		

						fputs("\tlong ",fp);
						fputs(parsed[j],fp);
						fputs(";\n",fp);
						continue;
					}

					if(strncmp(jen_str[j],OBJk,2) == 0){
						fputs("\t",fp);
						fputs(CODE_STRCT,fp);
						parsed[j][0] =toupper(parsed[j][0]);/*convert to CAPITAL*/
						fputs(parsed[j],fp);
						fputs(" ",fp);
						parsed[j][0] = tolower(parsed[j][0]);/*convert to small*/
						fputs(parsed[j],fp);
						fputs(";\n",fp);
						while(jen_str[j][0] != END){
							j++,k++;
						}

						i = j;
					}
				}
				fputs("};\n\n",fp);

			}

		}
		counter--;	
	}

	fputs(CODE_STRCT,fp);
	fputs("Object",fp);
	fputs(" {\n",fp);		

	for(int i = 0, k = i+1; i < steps;i++, k++){
		
		if(jen_str[k][0] == 'v'){
			fputs("\tchar ",fp);
			fputs(parsed[k-1],fp);
			fputs("[MAX_STRING_LENGTH];\n",fp);
			continue;
		}

		if(jen_str[k][0] == 'l'){
			if(strncmp(parsed[k],"null",4) == 0){
				fputs("\tchar ",fp);
				fputs(parsed[k-1],fp);
				fputs("[MAX_STRING_LENGTH];\n",fp);
				continue;
			}
			fputs("\tint ",fp);
			fputs(parsed[k-1],fp);
			fputs(";\n",fp);
			continue;
		}

		if(jen_str[k][0] == 'n'){
			if(is_float(parsed[k])){
				fputs("\tdouble ",fp);
				fputs(parsed[k-1],fp);
				fputs(";\n",fp);
				continue;
			}		

			fputs("\tlong ",fp);
			fputs(parsed[i],fp);
			fputs(";\n",fp);
			continue;
		}

		if(strncmp(jen_str[i],ARRk,2) == 0){
			int j = i+1;
			int count = 0, el = 0, l = 0, n = 0, fl = 0, o = 0 ;
			while(jen_str[j][0] == EL || jen_str[j][0] == LITERAL || jen_str[j][0] == NUM
					|| strstr(jen_str[j],"k:") != NULL
					|| strstr(jen_str[j],"v:") != NULL ){

				if(jen_str[j][0] == EL)
					el++;

				if(jen_str[j][0] == LITERAL)
					l++;

				if(jen_str[j][0] == NUM){
					n++;
					if(is_float(parsed[k-1]))
						fl++;
				}
			
				if(strstr(jen_str[j],"k:") != NULL ){
					if(jen_str[j][3] == '0' && o == 0)
						o++;
	
				}
				count++;
				j++;
			}
			
			if(count == el){
				fputs("\tchar ",fp);
				fputs(parsed[k-1],fp);
				fputs("[",fp);
				char nu[3] = {0};
				if(snprintf(nu,3,"%d",el) < 0){
					fprintf(stderr,"snprintf() failed, %s:%d",__FILE__,__LINE__-1);
					return -1;
				}
				fputs(nu,fp);
				fputs("];\n",fp);
				continue;
			}

		
			if(count == l){
				fputs("\tint ",fp);
				fputs(parsed[k-1],fp);
				char nu[3] = {0};                                                                   
				fputs("[",fp);
				if(snprintf(nu,3,"%d",l) < 0){
					fprintf(stderr,"snprintf() failed, %s:%d",__FILE__,__LINE__-1);
					return -1;
				}
				fputs(nu,fp);
				fputs("];\n",fp);
				continue;

			}

			if(count == n){
				if(n == fl){
					fputs("\tdouble ",fp);
					fputs(parsed[k-1],fp);
					char nu[3] = {0};                                                                   
					fputs("[",fp);
					if(snprintf(nu,3,"%d",fl) < 0){
						fprintf(stderr,"snprintf() failed, %s:%d",__FILE__,__LINE__-1);
						return -1;
					}
					fputs(nu,fp);
					fputs("];\n",fp);
					continue;	
				}

				fputs("\tlong ",fp);
                                fputs(parsed[k-1],fp);
                                char nu[3] = {0};
                                fputs("[",fp);
                                if(snprintf(nu,3,"%d",n) < 0){
                                        fprintf(stderr,"snprintf() failed, %s:%d",__FILE__,__LINE__-1);
                                        return -1;
                                }
				fputs(nu,fp);
                                fputs("];\n",fp);
				continue;
			}

			if(count == o){
				fputs("\t",fp);
				fputs(CODE_STRCT,fp);
				fputs("obj objs",fp);
				char nu[3] = {0};
                                fputs("[",fp);
                                if(snprintf(nu,3,"%d",o) < 0){
                                        fprintf(stderr,"snprintf() failed, %s:%d",__FILE__,__LINE__-1);
                                        return -1;
                                }
				fputs(nu,fp);
                                fputs("];\n",fp);
			}
			
		}

		if(jen_str[i][0] == '\0'){
			fputs("\t",fp);
			fputs(CODE_STRCT,fp);
			parsed[k-1][0] =toupper(parsed[k-1][0]);/*convert to CAPITAL*/
			fputs(parsed[k-1],fp);
			fputs(" ",fp);
			parsed[k-1][0] = tolower(parsed[k-1][0]);/*convert to small*/
			fputs(parsed[k-1],fp);
			fputs(";\n",fp);
			while(jen_str[i][0] != END){
				i++, k++;
			}
		}

	}
	
	return 0;
}



int parse_json(char *json, char parsed[][MAX_LENGTH])
{
	int step =  0;
	while(*json != '\0') {
		if(*json == ',' || *json == ' ' || *json == '\n' ||
				*json == '\t' || *json == '\r' || *json == ':'){
			json++;
			continue;
		}


		switch(*json){
		case '{':
			if(step > 0){
				if(jen_str[step-1][0] == KEY){
					jen_str[step-1][1] = OBJ;
				} else{
					jen_str[step][0] = OBJ;
				}
			} 
			parse_object(&json,parsed,&step);
			parsed[step][0] = END;
			jen_str[step][0] = END;
			step++;
			break;
		case '"':
			/*string*/
			if(step > 0){
                                if(strncmp(jen_str[step -1],OBJk,2) == 0){
                                        jen_str[step][0] = KEY;
                                }else if(jen_str[step - 1][0] == KEY) {
                                        jen_str[step][0] = VALUE;
                                }else{
                                        jen_str[step][0] = KEY;
                                }
                        }else{
                                jen_str[step][0] = KEY;
                        }

			parse_string(&json,parsed,&step);
			break;
		case '[':
			/*array */
			if(step > 0){
				if(jen_str[step -1][0] == KEY){
					jen_str[step - 1][1] = ARRAY;
				}
			}
			parse_array(&json, parsed,&step);	
			nested_object_in_array = 0;
			inside_array = 0;
			break;
		default:
			if(isalpha(*json)){
				jen_str[step][0] = LITERAL;
				parse_literal(&json,parsed,&step);
			}else if(isdigit(*json) || *json == '-'){
				jen_str[step][0] = NUM;
				parse_number(&json,parsed,&step);
			}


			break;
		}

		json++;	
	
	}
	return step;
}

static int parse_object(char **json, char parsed[][MAX_LENGTH], int *step)
{
	(*json)++;
	while(**json != '}' && **json != '\0'){
		if(**json == ',' || **json == ' ' || **json == '\n' ||
			       	**json == '\t' || **json == '\r' || **json == ':'){
			(*json)++;
			continue;
		}


		switch(**json){
		case '{':
			if(*step > 0){
				if(jen_str[*step-1][0] == KEY){
					jen_str[*step-1][1] = OBJ;
				} else{
					jen_str[*step][0] = OBJ;
				}
			} else {
				jen_str[*step][0] = OBJ;
			}	

			parse_object(json,parsed,step);
			parsed[*step][0] = END;
			jen_str[*step][0] = END;
			(*step)++;
			break;
		case '"':
			/*string*/
			if(*step > 0){
				if(strncmp(jen_str[*step -1],OBJk,2) == 0){
					jen_str[*step][0] = KEY;
				}else if(strncmp(jen_str[*step -1],ARRk,2) == 0){
					if(snprintf(jen_str[*step],5,"%c:%d",KEY,nested_object_in_array) < 0){
						fprintf(stderr,"snprintf() failed %s:%d.",__FILE__,__LINE__-1);
						return -1;
					}
				}else if(strstr(jen_str[*step -1],"v:") != NULL){
					if(snprintf(jen_str[*step],5,"%c:%d",KEY,nested_object_in_array) < 0){
						fprintf(stderr,"snprintf() failed %s:%d.",__FILE__,__LINE__-1);
						return -1;
					}

				}else if(strstr(jen_str[*step -1],"k:") != NULL){
					if(snprintf(jen_str[*step],5,"%c:%d",VALUE,nested_object_in_array) < 0){
						fprintf(stderr,"snprintf() failed %s:%d.",__FILE__,__LINE__-1);
						return -1;
					}
				}else if(inside_array){
					if(snprintf(jen_str[*step],5,"%c:%d",KEY,nested_object_in_array) < 0){
						fprintf(stderr,"snprintf() failed %s:%d.",__FILE__,__LINE__-1);
						return -1;
					}		
				}else if(jen_str[*step - 1][0] == KEY) {
					jen_str[*step][0] = VALUE;
				}else{
					jen_str[*step][0] = KEY;
				}
			}else{
				jen_str[*step][0] = KEY;
			}

			parse_string(json,parsed,step);
			break;
		case '[':
			if(*step > 0){
				if(jen_str[*step -1][0] == KEY){
					jen_str[*step - 1][1] = ARRAY;
				}
			}

			/*array */
			parse_array(json,parsed,step);
			inside_array = 0;
			break;
		default:
			if(isalpha(**json)){
				jen_str[*step][0] = LITERAL;
				parse_literal(json,parsed,step);
			}else if(isdigit(**json) || **json == '-'){
				jen_str[*step][0] = NUM;
				parse_number(json,parsed,step);
			}

			break;
		}

		if(**json != '}')		
			(*json)++;
	
	}

	return 0;
}


static int parse_string(char **json, char parsed[][MAX_LENGTH], int *step)
{
	(*json)++;
	int counter = 0;
	while(**json != '"'){
		parsed[*step][counter] = **json;
		counter++;
		(*json)++;
	}

	(*step)++;
	return 0;
}

static int parse_array(char **json, char parsed[][MAX_LENGTH], int *step)
{
	inside_array = 1;
	(*json)++;
	while(**json != ']') {
		if(**json == ',' || **json == ' '){
			(*json)++;
			continue;
		}

		switch(**json){
		case '{':
			parse_object(json,parsed,step);
			nested_object_in_array++;
			break;
		case '"':
			/*string*/
			jen_str[*step][0] = EL;

			parse_string(json,parsed,step);
			break;
		case '[':
			if(*step > 0){
				if(jen_str[*step -1][0] == KEY){
					jen_str[*step - 1][1] = ARRAY;
				}
			}
			/*array */
			parse_array(json,parsed,step);
			break;
		default:
			if(isalpha(**json)){
				jen_str[*step][0] = LITERAL;
				parse_literal(json,parsed,step);
			}else if(isdigit(**json) || **json == '-'){
				jen_str[*step][0] = NUM;
				parse_number(json,parsed,step);
			}

			break;
		}

		(*json)++;
	}
	return 0;
}

static int parse_literal(char **json,  char parsed[][MAX_LENGTH], int *step)
{
	int counter = 0;
	while(**json != ',' && **json != ' ' && **json != '}'){
		parsed[*step][counter] = **json;
		counter++;
		(*json)++;
	}
	(*step)++;
	return 0;
}

static int parse_number(char **json,  char parsed[][MAX_LENGTH], int *step)
{
	int counter = 0;
	while(**json != ',' && **json != ' ' && **json != '}'){
		parsed[*step][counter] = **json;
		counter++;
		(*json)++;
	}
	(*step)++;
	return 0;
}

static FILE *init_header()
{
	FILE *fp = fopen("data.h","w+");
	if(!fp){
		fprintf(stderr,"failed to open %s.\n",DATA);
		return 0;
	}
	

	fputs(Head,fp);

	return fp;
}

static int is_float(char *number){
	for(; *number != '\0'; number++)
		if(*number == '.')
			return 1;

	return 0;
}
