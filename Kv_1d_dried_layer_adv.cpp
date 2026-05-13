#include <math.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "H2O_thps.h"
#define pi acos(-1)

using namespace std;

int main (int argc, char *argv[])
    {
    const char* input_file  = (argc > 1) ? argv[1] : "Kv_1d_dried_layer_data.dat";
    const char* output_file = (argc > 2) ? argv[2] : "Kv_1d_dried_layer_results.dat";

	double Nv, di, de, rows, rowl, mfi, mff, vfi, vff, Ap, Av, Fh,  Fhi, Fhf, Tf, h, Pc, Pv, Pcpa, Tv, dhv, err, dy, tt, Fhreg, Fhregs, dtr, Ca, Cb, Tb, dq_dt, sbr, dt, d, Kv, C1, C2, C3, dos;
	double R0, A1, A2, a, b, Rs, Pvn, Hd, Rp, resta, rata, diff, xs, Pvpa, Pspa, Pmpa, Psn, Pmn, Pmn2, Ps, Pm, Pm2, maxdiff, Am, Rm, Am2, Rm2, Kvv, Kvt, Kvf, Kvf2, C1t, C2t, C3t, C1f, C2f, C3f;
	double Pm2pa, C1f2, C2f2, C3f2;

	int i, N;

    {
        FILE * tempe1;
		tempe1=fopen(input_file,"r");
		if (tempe1 == NULL) { fprintf(stderr, "ERROR: Cannot open input file: %s\n", input_file); return 1; }
		fscanf (tempe1, "%*[^\n]\n");
		//Experimental testing data
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &dos);//Dosing in mass[kg] (0) of volume [l] (1)
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &mfi);//Initial filling weight [kg]
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &xs);//fraction of solute
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &mff);//Final filling weight [kg]
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &vfi);//Initial filling volume [l] (only if dosing is in volume)
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &vff);//Final filling volume [l] (only if dosing is in volume)
		//Vial data
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &Nv);//Quantity of vials
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &di);//internal diameter vials [m]
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &de);//external diameter vials [m]
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &C1);//vial heat transfer parameter 1 [W/m2K]
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &C2);//vial heat transfer parameter 2 [W/m2KPa]
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &C3);//vial heat transfer paramenter 3 [1/Pa]
		//Stopper data
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &a);//a[kg/s Pa] (stopper resistance coeficient 1)
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &b);//b[kg/s Pa^2] (stopper resistance coeficient 2)
		//Tray data
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &C1t);//vial heat transfer parameter 1 [W/m2K]
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &C2t);//vial heat transfer parameter 2 [W/m2KPa]
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &C3t);//vial heat transfer paramenter 3 [1/Pa]
		//Protective foil data
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &Rm);//Rm[m/s] (protective foil resistance 1 (inner))
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &Am);//Am[m2] (protective foil area 1 (inner))
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &C1f);//vial heat transfer parameter 1 [W/m2K]
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &C2f);//vial heat transfer parameter 2 [W/m2KPa]
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &C3f);//vial heat transfer paramenter 3 [1/Pa]
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &Rm2);//Rm[m/s] (protective foil resistance 2 (outer))
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &Am2);//Am[m2] (protective foil area 2 (outer))
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &C1f2);//vial heat transfer parameter 1 [W/m2K]
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &C2f2);//vial heat transfer parameter 2 [W/m2KPa]
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &C3f2);//vial heat transfer paramenter 3 [1/Pa]
		//Product data
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &R0);//R0[m/s] (product resistance coeficient 1)
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &A1);//A1[1/s] (product resistance coeficient 2)
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &A2);//A2[1/m] (product resistance coeficient 3)
		//A2[1/m] (product resistance coeficient 3)
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &Tf);//Shelves fluid temperature [ºC]
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &Pc);//Chamber pressure [mbar]
		//Constant thermophisical properties
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &rows);//Water density in solid state [kg/m3]
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &h);//ice conductivity [W/Km]
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &rowl);//Water density in liquid state [kg/m3]
		//Numerical parameters
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%d\n", &N);//Number of nodes at frozen product height
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &diff);//Allowed difference between the experimental time and the calculated time [s]
		//Output file parameters
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%*[^\n]\n");
		fscanf (tempe1, "%lf\n", &Fhregs);//Height step to print results[m]
		fclose(tempe1);
        
        
			
		//First calculations
		Pcpa=Pc*100; //Chamber pressure [Pa]
		Pmpa=Pc*100; 
		Pm2pa=Pc*100; 
		Ap=Nv*pow(di,2)*pi/4; //Total heat transfer surface [m2]
		Av=Nv*pow(de,2)*pi/4;
		
		Kvv=C1+(C2*Pmpa/(1+C3*Pmpa));//Initial guess heat transfer coeficient [W/m2ºC]
		Kvt=C1t+(C2t*Pmpa/(1+C3t*Pmpa));//Initial guess heat transfer coeficient [W/m2ºC]
		Kvf=C1f+(C2f*Pm2pa/(1+C3f*Pm2pa));//Initial guess heat transfer coeficient [W/m2ºC]
		Kvf2=C1f2+(C2f2*Pcpa/(1+C3f2*Pcpa));//Initial guess heat transfer coeficient [W/m2ºC]
		Kv=1/((1/Kvv+1/Kvt+1/Kvf+1/Kvf2));//Initial guess heat transfer coeficient [W/m2ºC]
		
		Rs=1/(a+b*(Pcpa+Pspa)/2)*Ap/Nv;
		
		if(dos==1){//if inlet data is in volume[l]
		Fhi=(vfi/1000)*(rowl/rows)/Ap; //initial frozen height [m]
       	Fhf=(vff/1000)*(rowl/rows)/Ap; //Final frozen height [m]
		dy=(Fhi-Fhf)/N; //node height [m]
		}
		
		if(dos==0){//if inlet data is in mass[l]
		Fhi=mfi*(1-xs)/rows/Ap; //initial frozen height [m]
       	Fhf=(mff-mfi*xs)/rows/Ap; //Final frozen height [m]
		dy=(Fhi-Fhf)/N; //node height [m]
		}
		
		tt=0;
		Fhreg=Fhregs;
		dtr=0;
		Fh=Fhi;
		Hd=Fhi-Fh;
		
		
		FILE * tempe2;
		tempe2=fopen(output_file,"w+");
		if (tempe2 == NULL) { fprintf(stderr, "ERROR: Cannot open output file: %s\n", output_file); return 1; }
		fprintf(tempe2,"Inlet data\n");
		fprintf(tempe2,"Experimental testing data\n");
		fprintf(tempe2,"Initial filling weight [kg];");
  		fprintf(tempe2,"%f\n",mfi);
  		fprintf(tempe2,"Final filling weight [kg];");
  		fprintf(tempe2,"%f\n",mff);
  		fprintf(tempe2,"Fraction of solute [w/w];");
  		fprintf(tempe2,"%f\n",xs);
  		fprintf(tempe2,"Vial data\n");
  		fprintf(tempe2,"Quantity of vials;");
  		fprintf(tempe2,"%f\n",Nv);
  		fprintf(tempe2,"Vials internal diameter [m];");
  		fprintf(tempe2,"%f\n",di);
  		fprintf(tempe2,"Vials external diameter [m];");
  		fprintf(tempe2,"%f\n",de);
  		fprintf(tempe2,"vial heat transfer parameter 1;");
  		fprintf(tempe2,"%f\n",C1);
  		fprintf(tempe2,"vial heat transfer parameter 2;");
  		fprintf(tempe2,"%f\n",C2);
  		fprintf(tempe2,"vial heat transfer parameter 3;");
  		fprintf(tempe2,"%f\n",C3);
  		fprintf(tempe2,"Stopper resistance coeficient 1 [kg/s Pa];");
  		fprintf(tempe2,"%.20f\n",a);
  		fprintf(tempe2,"Stopper resistance coeficient 2 [kg/s Pa^2];");
  		fprintf(tempe2,"%.20f\n",b);
  		fprintf(tempe2,"Tray data\n");
  		fprintf(tempe2,"Tray transfer parameter 1;");
  		fprintf(tempe2,"%f\n",C1t);
  		fprintf(tempe2,"Tray heat transfer parameter 2;");
  		fprintf(tempe2,"%f\n",C2t);
  		fprintf(tempe2,"Tray heat transfer parameter 3;");
  		fprintf(tempe2,"%f\n",C3t);
  		fprintf(tempe2,"Protective foil data\n");
  		fprintf(tempe2,"Protective foil resistance 1 (inner) [m/s];");
  		fprintf(tempe2,"%f\n",Rm);
  		fprintf(tempe2,"Protective foil resistance surface 1 (inner) [m2];");
  		fprintf(tempe2,"%f\n",Am);
  		fprintf(tempe2,"Protective foil 1 (inner) heat transfer parameter 1;");
  		fprintf(tempe2,"%f\n",C1f);
  		fprintf(tempe2,"Protective foil 1 (inner) heat transfer parameter 2;");
  		fprintf(tempe2,"%f\n",C2f);
  		fprintf(tempe2,"Protective foil 1 (inner) heat transfer parameter 3;");
  		fprintf(tempe2,"%f\n",C3f);
  		fprintf(tempe2,"Protective foil resistance 2 (outer) [m/s];");
  		fprintf(tempe2,"%f\n",Rm2);
  		fprintf(tempe2,"Protective foil resistance surface 2 (outer) [m2];");
  		fprintf(tempe2,"%f\n",Am2);
  		fprintf(tempe2,"Protective foil 2 (outer) heat transfer parameter 1;");
  		fprintf(tempe2,"%f\n",C1f2);
  		fprintf(tempe2,"Protective foil 2 (outer) heat transfer parameter 2;");
  		fprintf(tempe2,"%f\n",C2f2);
  		fprintf(tempe2,"Protective foil 2 (outer) heat transfer parameter 3;");
  		fprintf(tempe2,"%f\n",C3f2);
  		fprintf(tempe2,"Product data\n");
  		fprintf(tempe2,"R0[m/s] (product resistance coeficient 1);");
  		fprintf(tempe2,"%f\n",R0);
  		fprintf(tempe2,"A1[1/s] (product resistance coeficient 2);");
  		fprintf(tempe2,"%f\n",A1);
  		fprintf(tempe2,"A2[1/m] (product resistance coeficient 3);");
  		fprintf(tempe2,"%f\n",A2);
  		fprintf(tempe2,"Freeze dryer settings\n");
  		fprintf(tempe2,"Shelves fluid temperature [ºC];");
  		fprintf(tempe2,"%f\n",Tf);
  		fprintf(tempe2,"Chamber pressure [mbar];");
  		fprintf(tempe2,"%f\n",Pc);
  		fprintf(tempe2,"Constant thermophisical properties\n");
  		fprintf(tempe2,"Water density in solid state [kg/m3];");
  		fprintf(tempe2,"%f\n",rows);
  		fprintf(tempe2,"Ice conductivity [W/Km];");
  		fprintf(tempe2,"%f\n",h);
  		fprintf(tempe2,"Water density in liquid state [kg/m3];");
  		fprintf(tempe2,"%f\n",rowl);
  		fprintf(tempe2,"Numerical parameters\n");
  		fprintf(tempe2,"Number of nodes at frozen product height;");
  		fprintf(tempe2,"%d\n",N);
  		fprintf(tempe2,"Output file parameters\n");
  		fprintf(tempe2,"Height step to print results[m];");
  		fprintf(tempe2,"%f\n",Fhregs);
  		fprintf(tempe2,"\n");
  		fprintf(tempe2,"First calculations\n");
  		fprintf(tempe2,"Total heat transfer area [m2];");
  		fprintf(tempe2,"%f\n",Av);
  		fprintf(tempe2,"Total sublimation area [m2];");
  		fprintf(tempe2,"%f\n",Ap);
  		fprintf(tempe2,"Total protective foil area 1 (inner) [m2];");
  		fprintf(tempe2,"%f\n",Am);
  		fprintf(tempe2,"Total protective foil area 2 (outer) [m2];");
  		fprintf(tempe2,"%f\n",Am2);
  		fprintf(tempe2,"Initial frozen height [m];");
  		fprintf(tempe2,"%f\n",Fhi);
  		fprintf(tempe2,"Final frozen height [m];");
  		fprintf(tempe2,"%f\n",Fhf);
  		fprintf(tempe2,"Height differential [m];");
  		fprintf(tempe2,"%.20f\n",dy);
  		fprintf(tempe2,"\n");
  		fprintf(tempe2,"Results\n");
		fprintf(tempe2,"Fh[m];Fd[m];tt[h];dtr[h];Pm2[mbar];Pm[mbar];Ps[mbar];Pv[mbar];Tv[ºC];Tb[ºC];sbr[kg/sm2];");
		fprintf(tempe2," ;");
		fprintf(tempe2,"dPm2[mbar];dPm[mbar];dPs[mbar];dPp[mbar];");
		fprintf(tempe2," ;");
		fprintf(tempe2,"dPtot[mbar];");
		fprintf(tempe2," ;");
		fprintf(tempe2,"dPm[];dPs[];dPp[]\n");
		
		
		Ps=Pc; //Guessed pressure before stopper [mbar]
		Pm=Pc, //Guessed pressure membrane [mbar]
		Pvn=Pc; //Guessed Vapour pressure at the sublimation front [mbar]
		Pv=Pc; //Vapour pressure at the sublimation front [mbar]
		Pspa=Ps*100;//Initial guess pressure before stopper [Pa]
		Pmpa=Pm*100;//Initial guess pressure before membrane [Pa]
		
		Rp=R0+A1*Hd/(1+A2*Hd);//First value of product resistance
		sbr=0;
		Pv=(Pc*100+(Rp+Rs+Rm*(Ap/Am))*sbr)/100;//First value of pressure at sublimation front
		
		
		//loop for sublimating all nodes under study
		for(i=1; i<=N; i++){
		do{//loop for adjusting Pv
		//Thermophisical properties from correlations
		Tv=H2O_thps(1,Pv,2); //Vapour temperature at the sublimation front [ºC]
		dhv=H2O_thps(1,Pv,5)*1000; //sublimation enthalpy at the sublimation front [J/kg]
		//Conduction analitical solution
		Ca=(Tv-Tf)/(Fh+h/(Kv*Av/Ap));
		Cb=(h*Tv+(Kv*Av/Ap)*Tf*Fh)/((Kv*Av/Ap)*Fh+h);
		Tb=Ca*0+Cb;
		//Time for sublimating one node
		dq_dt=(Kv*Av/Ap)*(Tf-Tb);
		sbr=dq_dt/dhv;
		//Verifying guessed Pv
		Pvn=(Pc*100+(Rp+Rs+Rm*(Ap/Am)+Rm2*(Ap/Am2))*sbr)/100;
		Pmn=(Pc*100+(Rm*(Ap/Am)+Rm2*(Ap/Am2))*sbr)/100;
		Pmn2=(Pc*100+(Rm2*(Ap/Am2))*sbr)/100;
		Psn=(Pc*100+(Rs+Rm*(Ap/Am)+Rm2*(Ap/Am2))*sbr)/100;
		
		resta=Pvn-Pv;
		cout.precision(5);
		//cout<<Pvn<<" "<<Pv<<" "<<resta<<" "<<diff<<endl;
		if (resta<=0){resta=-resta;}
		
		maxdiff=resta;
		
		resta=Psn-Ps;
		cout.precision(5);
		//cout<<Pvn<<" "<<Pv<<" "<<resta<<" "<<diff<<endl;
		if (resta<=0){resta=-resta;}
		
		if(maxdiff<resta){maxdiff=resta;}
		
		resta=Pmn-Pm;
		cout.precision(5);
		//cout<<Pvn<<" "<<Pv<<" "<<resta<<" "<<diff<<endl;
		if (resta<=0){resta=-resta;}
		
		if(maxdiff<resta){maxdiff=resta;}
		
		resta=Pmn2-Pm2;
		cout.precision(5);
		//cout<<Pvn<<" "<<Pv<<" "<<resta<<" "<<diff<<endl;
		if (resta<=0){resta=-resta;}
		
		if(maxdiff<resta){maxdiff=resta;}
		
		
		//if(Fh<=0.003119){cout<<"Fh[m]"<<Fh<<" tt[h]"<<tt/3600<<" dtr[h]"<<dtr/3600<<" Pv[mbar]"<<Pv<<" Tv[C]"<<Tv<<" Tb[C]"<<Tb<<" dq_dt[w/m2]"<<dq_dt<<" sbr[kg/m2]"<<sbr<<endl;}
		if(Tv>=Tb){cout<<Tv<<" "<<Tb<<" "<<Pv<<endl;}
		
		Pv=(Pv*0.75+Pvn*0.25);//new value of Pvn for next iteration (first value for next portion)
		Ps=(Ps*0.75+Psn*0.25);//new value of Psn for next iteration (first value for next portion)
		Pm=(Pm*0.75+Pmn*0.25);//new value of Pmn for next iteration (first value for next portion)
		Pm2=(Pm2*0.75+Pmn2*0.25);//new value of Pmn for next iteration (first value for next portion)
		//if(Fh<=0.00326){cout<<resta<<endl;}
		Pvpa=Pv*100;
		Pmpa=Pm*100;
		Pm2pa=Pm2*100;
		Pspa=Ps*100;
		
		Rs=1/(a+b*(Pvpa+Pspa)/2)*Ap/Nv;
		
		Kvv=C1+(C2*Pmpa/(1+C3*Pmpa));//Initial guess heat transfer coeficient [W/m2ºC]
		Kvt=C1t+(C2t*Pmpa/(1+C3t*Pmpa));//Initial guess heat transfer coeficient [W/m2ºC]
		Kvf=C1f+(C2f*Pm2pa/(1+C3f*Pm2pa));//Initial guess heat transfer coeficient [W/m2ºC]
		Kvf2=C1f2+(C2f2*Pcpa/(1+C3f2*Pcpa));//Initial guess heat transfer coeficient [W/m2ºC]
		Kv=1/((1/Kvv+1/Kvt+1/Kvf+1/Kvf2));//Initial guess heat transfer coeficient [W/m2ºC]
		
		
		}while (maxdiff>=diff);//loop for adjusting Pv
		
		
		dt=dy*(1-xs)*rows/sbr;
		//if(Fh<=0.00326){
		//cout<<"Fh[m]"<<Fh<<" tt[h]"<<tt/3600<<" dtr[h]"<<dtr/3600<<" Pv[mbar]"<<Pv<<" Tv[C]"<<Tv<<" Tb[C]"<<Tb<<" dq_dt[w/m2]"<<dq_dt<<" sbr[kg/m2]"<<sbr<<endl;
		//}
		cout.precision(10);
		if(Fhreg>=Fhregs){
		fprintf(tempe2,"%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%.10f",Fh,(Fhi-Fh), tt/3600, dtr/3600, Pm2, Pm,Ps, Pv, Tv, Tb, sbr);
		fprintf(tempe2,"; ;");
		fprintf(tempe2,"%f;%f;%f;%f",(Pm2-Pc),(Pm-Pm2),(Ps-Pm),(Pv-Ps));
		fprintf(tempe2,"; ;");
		fprintf(tempe2,"%f",(Pv-Pc));
		fprintf(tempe2,"; ;");
		fprintf(tempe2,"%f;%f;%f;%f\n",((Pm2-Pc)/(Pv-Pc)),((Pm-Pm2)/(Pv-Pc)),((Ps-Pm)/(Pv-Pc)),((Pv-Ps)/(Pv-Pc)));
		
		
		
		cout<<"Fh[m]"<<Fh<<" tt[h]"<<tt/3600<<" dtr[h]"<<dtr/3600<<" Ps[mbar]"<<Ps<<" Pv[mbar]"<<Pv<<" Tv[C]"<<Tv<<" Tb[C]"<<Tb<<" dq_dt[w/m2]"<<dq_dt<<" sbr[kg/m2]"<<sbr<<endl;
		Fhreg=0;
		dtr=0;
		}
		//Acumulated time
		tt=tt+dt;
		//Preparing for next node
		Fh=Fh-dy;
		Hd=Fhi-Fh;
		//New Rp value
		Rp=R0+A1*Hd/(1+A2*Hd);
		//Acumulated height for register
		Fhreg=Fhreg+dy;
		//Acumulated time for register
		dtr=dtr+dt;
		}
		cout.precision(5);
		cout<<"Fh[m]"<<Fh<<" tt[h]"<<tt/3600<<" dtr[h]"<<dtr/3600<<" Ps[mbar]"<<Ps<<" Pv[mbar]"<<Pv<<" Tv[C]"<<Tv<<" Tb[C]"<<Tb<<" dq_dt[w/m2]"<<dq_dt<<" sbr[kg/m2]"<<sbr<<endl;
		fprintf(tempe2,"%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%.10f",Fh,(Fhi-Fh), tt/3600, dtr/3600,Pm2,Pm,Ps, Pv, Tv, Tb, sbr);
		fprintf(tempe2,"; ;");
		fprintf(tempe2,"%f;%f;%f;%f",(Pm2-Pc),(Pm-Pm2),(Ps-Pm),(Pv-Ps));
		fprintf(tempe2,"; ;");
		fprintf(tempe2,"%f",(Pv-Pc));
		fprintf(tempe2,"; ;");
		fprintf(tempe2,"%f;%f;%f;%f\n",((Pm2-Pc)/(Pv-Pc)),((Pm2-Pm)/(Pv-Pc)),((Ps-Pm)/(Pv-Pc)),((Pv-Ps)/(Pv-Pc)));
		
		fprintf(tempe2,"\n");
		fprintf(tempe2,"Cycle end\n");
		fprintf(tempe2,"Time[h];");
  		fprintf(tempe2,"%f \n",tt/3600);
  		fprintf(tempe2,"Time[s];");
  		fprintf(tempe2,"%f \n",tt);
  		fprintf(tempe2,"Tb[ºC];");
  		fprintf(tempe2,"%f \n",Tb);
  		fprintf(tempe2,"Sublimation rate[kg/sm2];");
  		fprintf(tempe2,"%.10f \n",sbr);
  		fprintf(tempe2,"Sublimation rate[kg/hm2];");
  		fprintf(tempe2,"%.5f \n",sbr*3600);
  		fprintf(tempe2,"Heat transfer coeficient vial [W/m2];");
  		fprintf(tempe2,"%f\n",Kvv);
  		fprintf(tempe2,"Heat transfer coeficient tray [W/m2];");
  		fprintf(tempe2,"%f\n",Kvt);
  		fprintf(tempe2,"Heat transfer coeficient foil 1 (inner) [W/m2];");
  		fprintf(tempe2,"%f\n",Kvf);
  		fprintf(tempe2,"Heat transfer coeficient foil 2 (outer) [W/m2];");
  		fprintf(tempe2,"%f\n",Kvf2);
  		fprintf(tempe2,"Total heat transfer coeficient [W/m2];");
  		fprintf(tempe2,"%f\n",Kv);
       	fclose(tempe2);
	 
    
  
    }
    return 0;
}
