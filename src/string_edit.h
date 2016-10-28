void Eliminate(char *str, char ch) {
	    for (; *str != '\0'; str++)//종료 문자를 만날 때까지 반복
	    {
	        if (*str == ch)//ch와 같은 문자일 때
	        {
	            strcpy(str, str + 1);
	            str--;
	        }
	    }
}
