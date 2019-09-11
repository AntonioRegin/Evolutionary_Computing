//
// Created by mithr on 9/11/19.
//
#include <math.h>
double  ToX(int pj[]){
    double x=0;
    for(int i=1;i<100;i++){
        if(i%2==1) {
            if(pj[i] ==1)
                x+=1;
            x *= 2;
        }
    }
	for(int i=1;i<100;i++){
        if(i%2==0) {
                x+=1;            
    }

    x/=2;
    if(pj[0] == 1)
        x*=-1;
}
double fun(double x){
    return (60 - (60*exp(-.5*pow( ( (x-377) /100),2) )) + (100*exp(-.5 * pow(((x-377)/0.1),2))) );
}
double eval(int pj[]){
    return  fun(ToX(pj));
}
