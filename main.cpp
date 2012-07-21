//c4tler
//find first 10 digit prime in consecutive digits in e
//using zz datatype

#include <iostream>
#include <NTL/ZZ.h>
#include <NTL/RR.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sstream>

#define OUTPUTPRECISION 1000	//the length, in digits, of e to be output to screen and or used in firther calculations
#define PRECISION 100000	//the digit precision of e in bits; a value of 64 here results in a value of that is 64 bits long
				//value limited to machine word length
				//PRECISION limits value of OUTPUTPRECISION, so must be at least as high as OUTPUTPRECISION (bit-to-int) or rounding occurs
				//higher values substantially increase computation time of program
#define NUMROUNDS 1000		//number of rounds to calculate e. More rounds means a more accurate value of e. 
				//After 1000 rounds, a 1000 digit long value of e has the same value as after 1001 rounds

NTL_CLIENT

void getE(RR &e); 			//approximates a value for eulers number (e); accuracy and precision depends on parameters defined above
					//passed by reference for memory efficiency
ZZ getFactorial(const ZZ &index);	//gets the result of n! where n can be of any size

bool testprime(ZZ prime);
int lehmanns(const ZZ &prime);
void gcd(ZZ &n1, ZZ &n2, ZZ &g);
ZZ calcFastExp(const ZZ& a, const ZZ& e, const ZZ& n);


int main(){

	cout << "Finding the first 10 digit prime in consecutive digits of e" << endl;

	RR e;
	cout << "Calculating e..." << endl;
	getE(e);
	
	//cout << "e = " << e << endl << endl << endl;

	string cE;
	
	stringstream ssinput;
	ssinput << e;
	ssinput >> cE;
	//cout << "cE : " << cE << endl;
	
	//cout << "Revised cE: " << cE[0] << cE[2] << cE[3] << cE[4] << cE[5] << cE[6] << cE[7] << cE[8] << cE[9] << cE[10] << endl;
	string tempce;
	tempce += cE[0];
	//tempce += cE[2];
	//cout << "tempce: " << tempce << endl;
	for (int i = 2; i < 11; i++){
		tempce += cE[i];
	}
	//cout << "1\tPrime to test: " << tempce << endl;
	
	//cycle through the rest of the values starting from [2] to [2+10] untill PRECISION
	
	int i = 2;
	int j = i+10;
	
	
	
	
	while (j < OUTPUTPRECISION){
		tempce = "";
		for(int k = 0; k < 10; k++){
			//cout << "k:" << k << " i:" << i << " k+1:" << (k+i) << endl;  
			tempce += cE[k+i];		
		}
		//cout << i << "\tPrime to test: " << tempce << endl;
		
		//convert tempce to a zz here
		ssinput.clear();
		ssinput << tempce;
		ZZ zztempce;
		ssinput >> zztempce; 
		
		
		
		if (testprime(zztempce)==true){
			cout << "The first prime is: " << zztempce << endl;
			return 0; 
		}
		//else{
		//	cout << zztempce << " is not a prime. " << endl;
		//}
		i++;
		j++;
	}
	
	
	//if (testprime(to_ZZ(5))==true) cout << "5 is a prime" << endl;
	//if (testprime(to_ZZ(4))==false) cout << "4 is not a prime" << endl;
	
	


	return 0;
}

//get e

void getE(RR &e){

	
	RR::SetPrecision(PRECISION); //sets the precision of the RR datatype to 64 bits
	RR::SetOutputPrecision(OUTPUTPRECISION); //sets the number of decimal digits of precision that are used when printing RR
	ZZ index = to_ZZ(1);
	e = to_RR(1);
	RR tempE = e;
	//cout << "1\te = " << e << endl;
	index++;
	ZZ denominator = to_ZZ(1);
	RR fraction = to_RR(1);
	//i=number of rounds
	for (int i = 0; i < NUMROUNDS; i++){
		//cout << i << " ";
		denominator = getFactorial(index);
		//cout << "numerator=" << index << endl;
		//cout << "\tdenominator=" << denominator << endl;
		fraction = to_RR(to_RR(index) / to_RR(denominator));
		//cout << "\tnum/denom=" << fraction << endl;
		
		tempE = e + fraction;
		//cout << endl << "\te = " << e << " + " << index << "/" << getFactorial(index) << endl; 
		e = tempE;
		//cout << e << endl;
		index++;
	}

}


ZZ getFactorial(const ZZ &index){

	ZZ denominator = to_ZZ(1);
	//cout << "\t";
	for (ZZ i = index; i != 0; i--){
		denominator = to_ZZ(denominator * i);
	}
	
	//cout << index << "! = " << denominator << endl;
	return denominator;
}

bool testprime(ZZ prime){

	if((prime%2)==0) return false;
	//ZZ p1 = (prime-1)/2;
	//if((lehmanns(p1)==-1) || (lehmanns(prime)==-1)) return false;
	if((lehmanns(prime)==-1)) return false;
	return true;
}

/*

void primeGen(){	//generate a safe prime
	ZZ prime;
	long length = 100;
	prime = RandomLen_ZZ(length);
	cout << "The prime to test is: " << prime << endl;
	if ((prime%2)==0){
		cout << prime << " is even. Add one to make odd. " << endl;
		prime++;
	}
	ZZ p1;
	p1 = to_ZZ(0);
	while((lehmanns(p1)==-1) || (lehmanns(prime)==-1)){ //while p1 is not a prime
		prime+=2;	//add two to keep it odd
		p1 = ((prime-1)/2);	//recalculate this for every new value of prime until prime and p1 are both prime
	}
	cout << prime << " is a safe prime." << endl;
	return;
}
*/

int lehmanns(const ZZ &prime){ //checks if the value passed in is a prime
	//initial checks
	if (prime == 0) return -1; //not a prime
	if (prime == 1) return 1; //is a prime
	if (prime == 2) return 1; //is a prime
	
	if ((prime%2)==0) return -1; //if prime is divisible by 2, its not a prime
	ZZ tempPrime, num, g;
	tempPrime = prime;
	num = 3;
	
	gcd(tempPrime, num, g); //find the gcd of the prime to be tested, and any arbitrary small odd number 
	//cout << "The prime to be tested is" << prime << endl;
	//cout << "gcd(" << prime << ",3) is: " << g << endl;
	
	if (g!=1){
		//cout << prime << " is not a prime" << endl;
		return -1;
	}//else if(g==1){
		//cout << prime << " might be a prime. Testing..." << endl;
	//}
	
	ZZ a, x, e;
	for (int i = 0; i < 20; i++){
		a = RandomBnd(prime);//where a is a random number
		if (a==to_ZZ(0)) a++;
		
		//x = (a^((n-1)/2))modn;
		e = (prime-1)/2;
		x = calcFastExp(a, e, prime);
		//cout << i << " ";
		if(!(x==1 || x==(to_ZZ(-1)%prime))){
			return -1;
		}//else{
			//cout << prime << " failed the lehmanns test. It is not a prime. " << endl;
		//	return -1;
		//}
	
	}
	return 1;
	
}

void gcd(ZZ &n1, ZZ &n2, ZZ &g){
	ZZ oN1 = n1, oN2 = n2;
	ZZ q, r;
	ZZ a1 = to_ZZ(1), a2 = to_ZZ(0), b1 = to_ZZ(0), b2 = to_ZZ(1);
	
	while(n2!=0){
		q = n1/n2;	//integer division discards everything after the decimal place
		r = n1%n2;	//holds everything discarded by the integer division
		if (r==0) break;
		n1 = n2;
		n2 = r;
		ZZ temp = a2;
		a2 = a1-q*a2;
		a1 = temp;
		temp = b2;
		b2 = b1-q*b2;
		b1 = temp;
	}
	g = n2;
	n1 = a2;
	//cout << "n1:" << n1 << endl;
	n2 = b2;
	//cout << "gcd(" << oN1 << "," << oN2 << ") = " << g << endl;
	//if (g!=1){
	//	cout << "No multiplicative inverse exists." << endl;
	//	exit(1);
	//}else{
	//	cout << "A multiplicative inverse exists." << endl;
	//}
	return;
}

ZZ calcFastExp(const ZZ& a, const ZZ& e, const ZZ& n){	//passing by reference so local copy wont be made to save on memory usage
							//but dont want to change the values so pass as consts
	if (e==0) return to_ZZ(1);	//anything raised to zero is 1, cant have mod zero
	long numBits = NumBits(e);	//function declared in ntl library, gets number of bits in the exponent
	ZZ result;
	result = 1;
	
	for (long i = numBits-1; i > -1; i--){
		if (bit(e, i)==1){
			//ZZ pwr;
			//pwr = to_ZZ(2);
			result*=PowerMod(a, (power(to_ZZ(2), i)), n);
		}
	}
	return (result%n);
}



