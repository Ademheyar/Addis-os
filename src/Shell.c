#include <Shell.h>

string readStr()
{
	//char buff;
	char buffstr[200];
	uint8 i = 0;
	uint8 reading = 1;
	while(reading)
	{
			if(inportb(0x64) & 0x1)                 
			{
					switch(inportb(0x60))
					{ 
		/*case 1:
							printch('(char)27);           Escape button
							buffstr[i] = (char)27;
							i++;
							break;*/
			case 2:
							printch('1');
							buffstr[i] = '1';
							i++;
							break;
			case 3:
							printch('2');
							buffstr[i] = '2';
							i++;
							break;
			case 4:
							printch('3');
							buffstr[i] = '3';
							i++;
							break;
			case 5:
							printch('4');
							buffstr[i] = '4';
							i++;
							break;
			case 6:
							printch('5');
							buffstr[i] = '5';
							i++;
							break;
			case 7:
							printch('6');
							buffstr[i] = '6';
							i++;
							break;
			case 8:
							printch('7');
							buffstr[i] = '7';
							i++;
							break;
			case 9:
							printch('8');
							buffstr[i] = '8';
							i++;
							break;
			case 10:
							printch('9');
							buffstr[i] = '9';
							i++;
							break;
			case 11:
							printch('0');
							buffstr[i] = '0';
							i++;
							break;
			case 12:
							printch('-');
							buffstr[i] = '-';
							i++;
							break;
			case 13:
							printch('=');
							buffstr[i] = '=';
							i++;
							break;
			case 14:
							printch('\b');
							i--;
							/*if(i<0)
							{
								i = 0;
							}*/
							buffstr[i+1] = 0;
							buffstr[i] = 0;
							break;
			/* case 15:
							printch('\t');          Tab button
							buffstr[i] = '\t';
							i++;
							break;*/
			case 16:
							printch('q');
							buffstr[i] = 'q';
							i++;
							break;
			case 17:
							printch('w');
							buffstr[i] = 'w';
							i++;
							break;
			case 18:
							printch('e');
							buffstr[i] = 'e';
							i++;
							break;
			case 19:
							printch('r');
							buffstr[i] = 'r';
							i++;
							break;
			case 20:
							printch('t');
							buffstr[i] = 't';
							i++;
							break;
			case 21:
							printch('y');
							buffstr[i] = 'y';
							i++;
							break;
			case 22:
							printch('u');
							buffstr[i] = 'u';
							i++;
							break;
			case 23:
							printch('i');
							buffstr[i] = 'i';
							i++;
							break;
			case 24:
							printch('o');
							buffstr[i] = 'o';
							i++;
							break;
			case 25:
							printch('p');
							buffstr[i] = 'p';
							i++;
							break;
			case 26:
							printch('[');
							buffstr[i] = '[';
							i++;
							break;
			case 27:
							printch(']');
							buffstr[i] = ']';
							i++;
							break;
			case 28:
							// printch('\n');
							// buffstr[i] = '\n';
								i++;
							reading = 0;
							break;
		/*  case 29:
							printch('q');           Left Control
							buffstr[i] = 'q';
							i++;
							break;*/
			case 30:
							printch('a');
							buffstr[i] = 'a';
							i++;
							break;
			case 31:
							printch('s');
							buffstr[i] = 's';
							i++;
							break;
			case 32:
							printch('d');
							buffstr[i] = 'd';
							i++;
							break;
			case 33:
							printch('f');
							buffstr[i] = 'f';
							i++;
							break;
			case 34:
							printch('g');
							buffstr[i] = 'g';
							i++;
							break;
			case 35:
							printch('h');
							buffstr[i] = 'h';
							i++;
							break;
			case 36:
							printch('j');
							buffstr[i] = 'j';
							i++;
							break;
			case 37:
							printch('k');
							buffstr[i] = 'k';
							i++;
							break;
			case 38:
							printch('l');
							buffstr[i] = 'l';
							i++;
							break;
			case 39:
							printch(';');
							buffstr[i] = ';';
							i++;
							break;
			case 40:
							printch((char)44);               //   Single quote (')
							buffstr[i] = (char)44;
							i++;
							break;
			case 41:
							printch((char)44);               // Back tick (`)
							buffstr[i] = (char)44;
							i++;
							break;
		/* case 42:                                 Left shift 
							printch('q');
							buffstr[i] = 'q';
							i++;
							break;
			case 43:                                 \ (< for somekeyboards)   
							printch((char)92);
							buffstr[i] = 'q';
							i++;
							break;*/
			case 44:
							printch('z');
							buffstr[i] = 'z';
							i++;
							break;
			case 45:
							printch('x');
							buffstr[i] = 'x';
							i++;
							break;
			case 46:
							printch('c');
							buffstr[i] = 'c';
							i++;
							break;
			case 47:
							printch('v');
							buffstr[i] = 'v';
							i++;
							break;                
			case 48:
							printch('b');
							buffstr[i] = 'b';
							i++;
							break;               
			case 49:
							printch('n');
							buffstr[i] = 'n';
							i++;
							break;                
			case 50:
							printch('m');
							buffstr[i] = 'm';
							i++;
							break;               
			case 51:
							printch(',');
							buffstr[i] = ',';
							i++;
							break;                
			case 52:
							printch('.');
							buffstr[i] = '.';
							i++;
							break;            
			case 53:
							printch('/');
							buffstr[i] = '/';
							i++;
							break;            
			case 54:
							printch('.');
							buffstr[i] = '.';
							i++;
							break;            
			case 55:
							printch('/');
							buffstr[i] = '/';
							i++;
							break;            
		/*case 56:
							printch(' ');           Right shift
							buffstr[i] = ' ';
							i++;
							break;*/           
			case 57:
							printch(' ');
							buffstr[i] = ' ';
							i++;
							break;
					}
			}
	}
	buffstr[i-1] = 0;                
	buffstr[i] = buffstr[i];
	return "";
}

void launch_shell(int n)
{
	string ch = (string) malloc(200); // util.h
	//int counter = 0;
	do
	{
			print("NIDOS (");
			print(int_to_string(n));
			print(")> ");
		    ch = readStr(); //memory_copy(readStr(), ch,100);
		    if(strEql(ch,"cmd"))
		    {
		            print("\nYou are allready in cmd. A new recursive shell is opened\n");
					launch_shell(n+1);
		    }
		    else if(strEql(ch,"clear"))
		    {
		            clearScreen();
		    }
		    else if(strEql(ch,"sum"))
		    {
		    	sum();
		    }
		    else if(strEql(ch,"exit"))
		    {
		    	print("\nGood Bye!\n");
		    }
		    else if(strEql(ch,"echo"))
		    {
		    	echo();
		    }
		    else if(strEql(ch,"sort"))
		    {
		    	sort();
		    }
		    else if(strEql(ch,"fibonaci"))
		    {
		    	fibonaci();
		    }
		    else if(strEql(ch,"gcd"))
		    {
		    	gcd();
		    }
		    else if(strEql(ch,"help"))
		    {
		    	help();
		    }
		    else if(strEql(ch,"color"))
		    {
		    	set_background_color();
		    }
		    else if(strEql(ch,"multiply"))
		    {
		    	multiply();
		    }
		    
		    
		    else
		    {
		            print("\nBad command!\n");
		            print("NIDOS> ");
		    } 
	} while (!strEql(ch,"exit"));
}



void sum()
{
	print("\nHow many numbers: ");
	int n = str_to_int(readStr());
	//int i =0;
	print("\n");
	int arr[n];
	fill_array(arr,n);
	int s = sum_array(arr,n);
	print("Result: ");
	print(int_to_string(s));
	print("\n");
}
void echo()
{
	print("\n");
	string str = readStr();
	print("\n");
	print(str);
	print("\n");
}
void sort()
{
	int arr[100];
	print("\nArray size: ");
	int n = str_to_int(readStr());
	print("\n");
	fill_array(arr,n);
	print("Before sorting:\n");
	print_array(arr,n);
	print("\nOrdre: (1 for increassing/ 0 for decreassing): ");
	int ordre = str_to_int(readStr());
	insertion_sort(arr,n,ordre);
	print("\nAfter sorting:\n");
	print_array(arr,n);
}

void fill_array(int arr[],int n)
{
	int i = 0;
	for (i = 0;i<n;i++)
	{
		print("ARR[");
		print(int_to_string(i));
		print("]: ");
		arr[i] = str_to_int(readStr());
		print("\n");
	}
}
void print_array(int arr[],int n)
{
	int i = 0;
	for (i = 0;i<n;i++)
	{
		/*print("ARR[");
		print(int_to_string(i));
		print("]: ");*/
		print(int_to_string(arr[i]));
		print("   ");
	}
	print("\n");
}
void insertion_sort(int arr[],int n,int ordre) //1 is increassing, 0 is descreassing
{
	int i = 0;
	for (i = 1;i<n;i++)
	{
		int aux = arr[i];
		int j = i;
		while((j > 0) && ((aux < arr[j-1]) && ordre ))
		{
			arr[j] = arr[j-1];
			j = j -1;
		}
		arr[j] = aux;
	}
}
int sum_array(int arr[],int n)
{
	int i = 0;
	int s = 0;
	for (i = 0;i<n;i++)
	{
		s += arr[i];
	}
	return s;
}
void fibonaci()
{
	print("\nHow many Elements: ");
	int n = str_to_int(readStr()); 
	print("\n");
	int i = 0;
	for(i =0;i<n;i++)
	{
		print("Fibo ");
		print(int_to_string(i));
		print(" : ");
		print(int_to_string(fibo(i)));
		print("\n");
	}
	
}
int fibo(int n)
{
	if(n <2)
		return 1;
	else 
		return fibo(n-1) + fibo(n-2);
}
int gcd_couple(int a,int b)
{
	if(b == 0)
		return 1;
	if(a % b ==0) 
		return b;
	else 
		return gcd_couple(b,a % b);
}
void gcd()
{
	print("\nHow many numbers: ");
	int n = str_to_int(readStr());
	int i =0;
	print("\n");
	int arr[n];
	int matrix[n][n];
	fill_array(arr,n);
	for (i = 0;i<n;i++)
	{
		matrix[0][i] = arr[i];
	}
	int j = 0;
	for (i =1;i<n;i++)
	{
		for (j=0;j<n-1;j++)
		{
			matrix[i][j] = gcd_couple(matrix[i-1][j] , matrix[i-1][j+1]);
		}
	}
	print("Result: ");
	print(int_to_string(matrix[n-1][0]));
	print("\n");
}
void print_matrix(int matrix[][100],int rows,int cols)
{
	int i =0;
	int j = 0;
	for (i = 0;i<rows;i++)
	{
		for(j =0;j<cols;j++)
		{
			print(int_to_string(matrix[i][j]));
			print("   ");
		}
		print("\n");
	}
}
void set_background_color()
{
	print("\nColor codes : ");
	print("\n0 : black");
	print_colored("\n1 : blue",1,0);   // screen.h
	print_colored("\n2 : green",2,0);
	print_colored("\n3 : cyan",3,0);
	print_colored("\n4 : red",4,0);
	print_colored("\n5 : purple",5,0);
	print_colored("\n6 : orange",6,0);
	print_colored("\n7 : grey",7,0);
	print_colored("\n8 : dark grey",8,0);
	print_colored("\n9 : blue light",9,0);
	print_colored("\n10 : green light",10,0);
	print_colored("\n11 : blue lighter",11,0);
	print_colored("\n12 : red light",12,0);
	print_colored("\n13 : rose",13,0);
	print_colored("\n14 : yellow",14,0);
	print_colored("\n15 : white",15,0);
	
	print("\n\n Text color ? : ");
	int text_color = str_to_int(readStr());
	print("\n\n Background color ? : ");
	int bg_color = str_to_int(readStr());
	set_screen_color(text_color,bg_color);
	clearScreen();
}

void multiply()
{
	print("\nNum 1 :");
	int num1 = str_to_int(readStr());
	print("\nNum 2 :");
	int num2 = str_to_int(readStr());
	print("\nResult : ");
	print(int_to_string(num1*num2)); // util.h
	print("\n");
}

void help()
{
	print("\ncmd       : Launch a new recursive Shell");
	print("\nclear     : Clears the screen");
	print("\nsum       : Computes the sum of n numbers");
	print("\necho      : Reprint a given text");
	print("\nsort      : Sorts a given n numbers");
	print("\nfibonaci  : Prints the first n numbers of fibonaci");
	print("\ngcd       : Computes the grand common divider of n given numbers");
	print("\nexit      : Quits the current shell");
	print("\ncolor     : Changes the colors of the terminal");
	
	print("\n\n");
}

