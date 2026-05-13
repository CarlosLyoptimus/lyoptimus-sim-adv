#include <math.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

double H2O_thps(double itpi, double itp, double otpi);

double H2O_thps(double itpi, double itp, double otpi)
{

	double A0, A1, A2, A3, A4, A5, A6, H2O, b, c;
	
	if (itpi==1){
	
	if (otpi==1){H2O=itp;}
	
	if (otpi==2){
	A6 = 0.000011969445371;
    A5 = -0.00019568133707;
    A4 = 0.0011423191814;
    A3 = 0.056983195839;
    A2 = 0.5968472781;
    A1 = 10.424405094;
    A0 = -20.657977888;
	H2O=A6*pow(log(itp),6)+A5*pow(log(itp),5)+A4*pow(log(itp),4)+A3*pow(log(itp),3)+A2*pow(log(itp),2)+A1*pow(log(itp),1)+A0*pow(log(itp),0);
	}
	
	if (otpi==3){
	b = 0.0008420095995;
	c = 0.95393224594;
	H2O=1/(b*pow(itp,c));
	}
	
	if (otpi==5){
	if(itp<=6.11){
	A6 = -0.0000231150443;
    A5 = -0.00100758716;
    A4 = -0.00880001796;
    A3 = -0.0076513192;
    A2 = 0.0338891008;
    A1 = -2.59206784;
    A0 = 2836.70265;
	}
	if(itp>6.11){
	A6 = -0.00263001357;
    A5 = 0.0655878421;
    A4 = -0.699138707;
    A3 = 3.76344141;
    A2 = -13.1297484;
    A1 = -7.49682229;
    A0 = 2542.03137;
	}
	H2O=A6*pow(log(itp),6)+A5*pow(log(itp),5)+A4*pow(log(itp),4)+A3*pow(log(itp),3)+A2*pow(log(itp),2)+A1*pow(log(itp),1)+A0*pow(log(itp),0);
	}
	
	}

	if (itpi==2){
	
	if (otpi==2){H2O=itp;}
	
	if (otpi==1){
	A6 = -1.007213106E-12;
    A5 = 2.8524338777E-11;
    A4 = 1.2188804148E-08;
    A3 = 1.0598569493E-06;
    A2 = -0.00041900070591;
    A1 = 0.077590038281;
    A0 = 1.7710969984;
	H2O=exp(A6*pow(itp,6)+A5*pow(itp,5)+A4*pow(itp,4)+A3*pow(itp,3)+A2*pow(itp,2)+A1*pow(itp,1)+A0*pow(itp,0));
	}
	
	if (otpi==3){
	A6 = -1.1143239515E-12;
    A5 = 3.9619437539E-11;
    A4 = 1.2970908979E-08;
    A3 = 9.6431628761E-07;
    A2 = -0.00041242169848;
    A1 = 0.074099615012;
    A0 = -5.3690466983;
	H2O=1/(exp(A6*pow(itp,6)+A5*pow(itp,5)+A4*pow(itp,4)+A3*pow(itp,3)+A2*pow(itp,2)+A1*pow(itp,1)+A0*pow(itp,0)));
	}
	
	if (otpi==5){
	if(itp<=0){
	A6 = 0;
    A5 = 0;
    A4 = 0;
    A3 = 0.0000257233251;
    A2 = 0.00327823464;
    A1 = -0.154376296;
    A0 = 2832.23471;
	}
	if(itp>0){
	A6 = 0;
    A5 = 0;
    A4 = 0;
    A3 = -0.0000134938688;
    A2 = 0.000553510283;
    A1 = -2.36699295;
    A0 = 2501.5521;
	}
	H2O=A6*pow(itp,6)+A5*pow(itp,5)+A4*pow(itp,4)+A3*pow(itp,3)+A2*pow(itp,2)+A1*pow(itp,1)+A0*pow(itp,0);
	}
	
	}
     
     
	return H2O;
	

}
