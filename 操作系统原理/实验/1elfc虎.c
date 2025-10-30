R()
{
	P(mutex);
   	readcount ++;       if (readcount==1)
        P (w);
   	V(mutex);
	读
    P(mutex);
    readcount --;
    if (readcount==0)
        V(w);
    V(mutex);
 };
W() 
{
	P(w);
	写
	V(w);

};

/// @brief ////////////////
R(){
		P(S2);
		P(S3);
		if  (RC==0) then 	P(S1);
		RC++;
		V(S3);
		V(S2);
		Reading;
		P(S3);
		RC--;
		IF (RC==0) THEN V(S1);
		V(S3);
}	
W(){
	 	P(S2);			
		P(S1);			
		writing;
        V(S1);
        V(S2);
}
/// @brief //////////////////
W(){
     P(K);
    If  (WC==0) then P(S2);
    WC++;
   V(K);
   P(S1);
   WRITING;
   V(S1);
   P(K);
   WC--;
   If (wc==0) then  V(S2);
   V(k);
}
R(){
    P(S2);
    P(S3);
    if  (RC==0) then P(S1);
    RC++;
   V(S3);
   V(S2);
   Reading;
   P(S3);
   RC--;
   if  (RC==0) THEN V(S1);
   V(S3);
}
//////////////
int   fork[5]={1,1,1,1,1}; /*信号灯，表示某支筷子是否可用*/
int    count=4;/*信号灯，表示允许拿筷子的哲学家的个数*/
main () 
{
   cobegin 
		PH0();PH1();PH2();PH3();PH4();
  coend;
}
void   Phi()
{
		while(1) {
			think;
			P(count);
			P(fork[i]);	P(fork[(i+1) mod 5]);
			eat;
			V(fork[i]);	V(fork[(i+1) mod 5]);
			V(count);
		}
}
//////////////////////
#define  N  5
#define  THINKING  0
#define  HUNGRY    1
#define  EATING    2
int state[N];/*哲学家的状态*/
semaphore mutex=1;/*互斥信号灯*/
semaphore s[N];/*是否可以吃，初值为0*/
初值：state[ i ] = THINKING   s[ i ] = 0
void test(int i)
{
      if ((state[ i ] == HUNGRY) 
		&& (state [ (i-1) % 5] != EATING) 
          && (state [ (i+1) % 5] != EATING)
          {
               state[ i ] = EATING;
               V(s[ i ]);
           }
 }
 void  philosopher(int i) 
{   while (true) {
      思考;
      P(mutex);
      state[i] = HUNGRY;
      test(i);
      V(mutex);
      P(s[i]);
      拿左筷子；
      拿右筷子；
      进食；
      放左筷子；
      放右筷子；
      P(mutex)
      state[ i ] =  THINKING；
      test([i-1] % 5);
      test([i+1] % 5);
      V(mutex);
	}
}

////////////////////////
main()
{ 
    int n=5;//空位数
    int s=1； //互斥信号灯
    cobegin
        p1(); 
        p2(); 
        ……
        pn();
    coend
}  
pi()
{
    p(s);
    if (n==0) {
        v(s);
        离开;
    }
    else {
        n--;
        v(s);
        在长凳上休息;
        p(s);
        n++;
        v(s);
        离开长凳;
}


