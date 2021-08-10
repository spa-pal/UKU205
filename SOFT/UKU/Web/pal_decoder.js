var palDict = {
	A : "А",
	B : "Б",
	C : "Ц",
	D : "Д",
	E : "Е",
	F : "Ф",
	G : "Г",
	H : "Х",
	I : "И",
	J : "Й",
	K : "К",
	L : "Л",
	M : "М",
	N : "Н",
	O : "О",
	P : "П",
	Q : "Я",
	R : "Р",
	S : "С",
	T : "Т",
	U : "У",
	V : "Ю",
	W : "В",
	XA : "Ш",
	Y : "Ч",
	Z : "З",
	XE : "Ё",
	XC : "Ж",
	XD : "Щ",
	XB : "Ъ",
	XF : "Ы",
	XG : "Ь",
	XH : "Э",
	a : "а",
	b : "б",
	c : "ц",
	d : "д",
	e : "е",
	f : "ф",
	g : "г",
	h : "х",
	i : "и",
	j : "й",
	k : "к",
	l : "л",
	m : "м",
	n : "н",
	o : "о",
	p : "п",
	q : "я",
	r : "р",
	s : "с",
	t : "т",
	u : "у",
	v : "ю",
	w : "в",
	Xa : "ш",
	y : "ч",
	z : "з",
	Xe : "ё",
	Xc : "ж",
	Xd : "щ",
	Xb : "ъ",
	Xf : "ы",
	Xg : "ь",
	Xh : "э",
	Xi : "°",
	Xj : "№"
};

function palDecoder(input) {
	var output = "",i=0;
	while (input[i]){
		if(input[i]=='^'){
			if(input[++i]=='X'){
				output+=palDict[input[i]+input[i+1]];
				i++;
				i++;
			}
			else {
				output += palDict[input[i]];
				i++;
			}
		}
		else {
			output +=input[i++];
		}
	}
	return output;
}


function pal_cyr_coder(inp)
{
var output = "",i=0,ii=0;

/*output = pal_cyr_coder_output;*/

while(inp[i])
	{
	if(inp[i]=='А')
		{
		output+='^';
		output+='A';
		i++;
		}
	else if(inp[i]=='Б')
		{
		output+='^';
		output+='B';
		i++;
		}
	else if(inp[i]=='В')
		{
		output+='^';
		output+='W';
		i++;
		}
	else if(inp[i]=='Г')
		{
		output+='^';
		output+='G';
		i++;
		}
	else if(inp[i]=='Д')
		{
		output+='^';
		output+='D';
		i++;
		}
	else if(inp[i]=='Е')
		{
		output+='^';
		output+='E';
		i++;
		}
	else if(inp[i]=='Ё')
		{
		output+='^';
		output+='X';
		output+='E';
		i++;
		}
	else if(inp[i]=='Ж')
		{
		output+='^';
		output+='X';
		output+='C';
		i++;
		}
	else if(inp[i]=='З')
		{
		output+='^';
		output+='Z';
		i++;
		}
	else if(inp[i]=='И')
		{
		output+='^';
		output+='I';
		i++;
		}
	else if(inp[i]=='Й')
		{
		output+='^';
		output+='J';
		i++;
		}
	else if(inp[i]=='К')
		{
		output+='^';
		output+='K';
		i++;
		}
	else if(inp[i]=='Л')
		{
		output+='^';
		output+='L';
		i++;
		}
	else if(inp[i]=='М')
		{
		output+='^';
		output+='M';
		i++;
		}
	else if(inp[i]=='Н')
		{
		output+='^';
		output+='N';
		i++;
		}
	else if(inp[i]=='О')
		{
		output+='^';
		output+='O';
		i++;
		}
	else if(inp[i]=='П')
		{
		output+='^';
		output+='P';
		i++;
		}
	else if(inp[i]=='Р')
		{
		output+='^';
		output+='R';
		i++;
		}
	else if(inp[i]=='С')
		{
		output+='^';
		output+='S';
		i++;
		}
	else if(inp[i]=='Т')
		{
		output+='^';
		output+='T';
		i++;
		}
	else if(inp[i]=='У')
		{
		output+='^';
		output+='U';
		i++;
		}
	else if(inp[i]=='Ф')
		{
		output+='^';
		output+='F';
		i++;
		}
	else if(inp[i]=='Х')
		{
		output+='^';
		output+='H';
		i++;
		}
	else if(inp[i]=='Ц')
		{
		output+='^';
		output+='C';
		i++;
		}
	else if(inp[i]=='Ч')
		{
		output+='^';
		output+='Y';
		i++;
		}
	else if(inp[i]=='Ш')
		{
		output+='^';
		output+='X';
		output+='A';
		i++;
		}
	else if(inp[i]=='Щ')
		{
		output+='^';
		output+='X';
		output+='D';
		i++;
		}
	else if(inp[i]=='Ъ')
		{
		output+='^';
		output+='X';
		output+='B';
		i++;
		}
	else if(inp[i]=='Ы')
		{
		output+='^';
		output+='X';
		output+='F';
		i++;
		}
	else if(inp[i]=='Ь')
		{
		output+='^';
		output+='X';
		output+='G';
		i++;
		}
	else if(inp[i]=='Э')
		{
		output+='^';
		output+='X';
		output+='H';
		i++;
		}
	else if(inp[i]=='Ю')
		{
		output+='^';
		output+='V';
		i++;
		}
	else if(inp[i]=='Я')
		{
		output+='^';
		output+='Q';
		i++;
		}
	else if(inp[i]=='а')
		{
		output+='^';
		output+='a';
		i++;
		}
	else if(inp[i]=='б')
		{
		output+='^';
		output+='b';
		i++;
		}
	else if(inp[i]=='в')
		{
		output+='^';
		output+='w';
		i++;
		}
	else if(inp[i]=='г')
		{
		output+='^';
		output+='g';
		i++;
		}
	else if(inp[i]=='д')
		{
		output+='^';
		output+='d';
		i++;
		}
	else if(inp[i]=='е')
		{
		output+='^';
		output+='e';
		i++;
		}
	else if(inp[i]=='ё')
		{
		output+='^';
		output+='X';
		output+='e';
		i++;
		}
	else if(inp[i]=='ж')
		{
		output+='^';
		output+='X';
		output+='c';
		i++;
		}
	else if(inp[i]=='з')
		{
		output+='^';
		output+='z';
		i++;
		}
	else if(inp[i]=='и')
		{
		output+='^';
		output+='i';
		i++;
		}
	else if(inp[i]=='й')
		{
		output+='^';
		output+='j';
		i++;
		}
	else if(inp[i]=='к')
		{
		output+='^';
		output+='k';
		i++;
		}
	else if(inp[i]=='л')
		{
		output+='^';
		output+='l';
		i++;
		}
	else if(inp[i]=='м')
		{
		output+='^';
		output+='m';
		i++;
		}
	else if(inp[i]=='н')
		{
		output+='^';
		output+='n';
		i++;
		}
	else if(inp[i]=='о')
		{
		output+='^';
		output+='o';
		i++;
		}
	else if(inp[i]=='п')
		{
		output+='^';
		output+='p';
		i++;
		}
	else if(inp[i]=='р')
		{
		output+='^';
		output+='r';
		i++;
		}
	else if(inp[i]=='с')
		{
		output+='^';
		output+='s';
		i++;
		}
	else if(inp[i]=='т')
		{
		output+='^';
		output+='t';
		i++;
		}
	else if(inp[i]=='у')
		{
		output+='^';
		output+='u';
		i++;
		}
	else if(inp[i]=='ф')
		{
		output+='^';
		output+='f';
		i++;
		}
	else if(inp[i]=='х')
		{
		output+='^';
		output+='h';
		i++;
		}
	else if(inp[i]=='ц')
		{
		output+='^';
		output+='c';
		i++;
		}
	else if(inp[i]=='ч')
		{
		output+='^';
		output+='y';
		i++;
		}
	else if(inp[i]=='ш')
		{
		output+='^';
		output+='X';
		output+='a';
		i++;
		}
	else if(inp[i]=='щ')
		{
		output+='^';
		output+='X';
		output+='d';
		i++;
		}
	else if(inp[i]=='ъ')
		{
		output+='^';
		output+='X';
		output+='b';
		i++;
		}
	else if(inp[i]=='ы')
		{
		output+='^';
		output+='X';
		output+='f';
		i++;
		}
	else if(inp[i]=='ь')
		{
		output+='^';
		output+='X';
		output+='g';
		i++;
		}
	else if(inp[i]=='э')
		{
		output+='^';
		output+='X';
		output+='h';
		i++;
		}
	else if(inp[i]=='ю')
		{
		output+='^';
		output+='v';
		i++;
		}
	else if(inp[i]=='я')
		{
		output+='^';
		output+='q';
		i++;
		}
	else if(inp[i]=='°')
		{
		output+='^';
		output+='X';
		output+='i';
		i++;
		}
	else if(inp[i]=='№')
		{
		output+='^';
		output+='X';
		output+='j';
		i++;
		}
	else
		{
		output+=inp[i++];
		}
	}

/*while(in[i])
	{
	output+=in[i++];
	}*/

//output+=0;	
/*
for(i=0;i<4;i++)
	{
	output+=in[i++];
	}  */
return output;
}



console.log("pal_decoder.js загружен");