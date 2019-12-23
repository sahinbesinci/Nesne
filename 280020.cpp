#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <iomanip>
#include<bits/stdc++.h> 
#include <sstream>  // for string streams 
#include <math.h>
using namespace std;

template<class T>
class Matrix
{
public:
	// Matrix sıfının kurucu fonksiyonudur. Defaultta 10x10 luk matris üretir ve 0 değeri atar.
	Matrix();
	// Matrix sınıfının kurucu fonksiyonudur. ilk iki parametresi satır x sütun sayılarını verir. 
	// Son parametresi ise, sayısal bir ifade ise o sayılarla matrix i doldurur.
	// Eğer sayısal değil ve r ise random (0 ile 255) sayı ile matrixi doldurur.
	// Eğer sayısal değil ve e ise birim matris oluşturur.
	Matrix(int,int,T);
	// Matrix sınıfının oluşturulan matrix boyutunu değiştirir. Mevcuttaki satır ve ya sütun sayısından büyük ve ya küçük girilmişse aradaki farklara 0 değeri atanır.
	void resize(int,int);
	// Matrix dizisinin ekrana yazdırılması için kullanılmaktadır.
	void print();
	// Matrix dizininin text editörüne yazılmasını sağlar.
	void print(string);
	// İki Matrix nesnelerinde bulunan Matrix dizilerinin toplamının sonucunu verir. toplama işleminin aşırı yüklemesidir. 
	Matrix<T>* operator+(const Matrix<T>&);
	// Atama yapmayı sağlayan aşırı yükleme fonksiyonudur. bir matris dizisini diğer matris dizisine atar.
	void operator=(const Matrix<T>*);
	// İki Matrix nesnelerinde bulunan Matrix dizilerinin çıkarma işleminin sonucunu verir. çıkarma işleminin aşırı yüklemesidir. 
	Matrix<int> operator-(const Matrix<int>&);
	// İki Matrix nesnelerinde bulunan Matrix matrislerinin çarpma işleminin sonucunu verir. çarpma işleminin aşırı yüklemesidir. 
	Matrix<T> operator*(const Matrix<T>&);
	
	// Skaler işlemler için kullanılan aşırı yüklenen fonksiyonlardır.
	// Toplama, Çıkarma, Çarpma, Bölme, Mod ve üs almaya yarar.
	Matrix<T> operator+(const T);
	Matrix<T> operator-(const T);
	Matrix<T> operator*(const T);
	Matrix<T> operator/(const T);
	Matrix<T> operator%(const T);
	Matrix<T> operator^(const T);
	
	// Matrix dizisinin Transpozesini alan fonksiyondur.
	void Tr();
	
	// Matrix dizisinin Transpozesini alan fonksiyondur.
	void emul(Matrix<T>*);
	
	// Matrix dizisinin Tersini alan fonksiyondur.
	Matrix<T> inv();
	T det();
	
	//for Table
	int getItem(int,int);
	void printTable(string*, string*);
private:
	T** matrix;
	int row;
	int col;
	bool createMatrix(int,int,T);
	// matris - skalar işlemleri
	Matrix<T> islem(char,T);
	void Transpoze();
	Matrix<T> inversion();
	T determinant(Matrix<T>*);
};

template<class T>
class Table
{
public:
	Table();
	Table(int,int,int);
	void print();
	int itemAt(int,int);
	int itemAt(string);
	int itemAt(string, string);
	bool setRowNames(string[], int);
	bool setColNames(string[], int);
private:
	Matrix<T>* matrix;
	int row;
	int col;
	string* c;
	string* r;
	string generateColumnName(int);
	int getItem(int, string);
};


template<class T>
Matrix<T>::Matrix(){
	createMatrix(10,10,0);
}

template<class T>
Matrix<T>::Matrix(int a,int b,T s){
	createMatrix(a,b,s);
}

template<class T>
bool Matrix<T>::createMatrix(int a,int b,T s){
	row = a;
	col = b;
	matrix = new T*[row];
	for(int i=0;i<row;i++){
		matrix[i] = new T[col];
	}
	
	// birim matris ise
	if(s == 'e'){
		if(a != b){
			cout << "Bu boyutta birim matris olamaz!"<<endl;
			return false;
		}else{
			for(int i=0;i<row;i++){
				for(int j=0;j<col;j++){
					if(i == j){
						matrix[i][j] = (T)(1);
						break;
					}
				}
			}
		}
	}
	// random değerleri olan matris ise
	else if(s == 'r'){
		for(int i=0;i<row;i++){
			for(int j=0;j<col;j++){
				matrix[i][j] = (T)(rand() % 255);
			}
		}
	}
	else{
		for(int i=0;i<row;i++){
			for(int j=0;j<col;j++){
				matrix[i][j] = (T) s;
			}
		}
	}
	return true;
}

template<class T>
void Matrix<T>::resize(int a,int b){
	Matrix<T>* tMatrix = new Matrix<T>(a,b,0);
	for(int i=0;i<a;i++){
		for(int j=0;j<b;j++){
			if(i<row && j<col){
				tMatrix->matrix[i][j] = matrix[i][j];
			}
		}
	}
	matrix = tMatrix->matrix;
	row = a;
	col = b;
	delete tMatrix;
}

template<class T>
void Matrix<T>::print(){
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			cout<<right<<setw(4)<<matrix[i][j];
		}
		cout<<endl;
	}
}

template<class T>
void Matrix<T>::print(string fileName){
    int n = fileName.length(); 
    char char_array[n + 1]; 
    strcpy(char_array, fileName.c_str()); 
	ofstream dosya(char_array);
	if ( dosya.is_open() ){
		for(int i=0;i<row;i++){
			for(int j=0;j<col;j++){
				dosya << matrix[i][j] <<" ";
			}
			dosya<<endl;
		}
		dosya.close();
	}
}

template<class T>
Matrix<T>* Matrix<T>::operator+(const Matrix<T>& obj){
	int _row = this->row > obj.row ? this->row : obj.row;
	int _col = this->col > obj.col ? this->col : obj.col;
	Matrix<T> *tMatrix = new Matrix<T>(_row,_col,0);
	for(int i = 0; i<_row; i++){
		for(int j = 0; j<_col; j++){
			int temp1 = 0;
			int temp2 = 0;
			if(this->row > i && this->col > j){
				temp1 = this->matrix[i][j];
			}
			if(obj.row > i && obj.col > j){
				temp2 = obj.matrix[i][j];
			}
			tMatrix->matrix[i][j] = temp1 + temp2;
		}
	}
	return tMatrix;
};

template<class T>
void Matrix<T>::operator=(const Matrix<T>* obj){
	Matrix<T> *tMatrix = new Matrix<T>(obj->row, obj->col, 0);
	*this->matrix = new T[obj->row];
	for(int i=0;i<obj->row;i++){
		this->matrix[i] = new T[obj->col];
	}
	this->row = obj->row;
	this->col = obj->col;
	for(int i = 0; i<obj->row; i++){
		for(int j = 0; j<obj->col; j++){
			this->matrix[i][j] = obj->matrix[i][j];
		}
	}
};

template<class T>
Matrix<int> Matrix<T>::operator-(const Matrix<int>& obj){
	int _row = row > obj.row ? row : obj.row;
	int _col = col > obj.col ? col : obj.col;
	Matrix<int> tMatrix(_row,_col,0);
	for(T i = 0; i<_row; i++){
		for(T j = 0; j<_col; j++){
			int temp1 = 0;
			int temp2 = 0;
			
			if(this->row > i && this->col > j){
				temp1 = this->matrix[i][j];
			}
			if(obj.row > i && obj.col > j){
				temp2 = obj.matrix[i][j];
			}
			tMatrix.matrix[i][j] = temp1 - temp2;
		}
	}
	return tMatrix;
};

template<class T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& obj){
	int _row1 = row;
	int _row2 = obj.row;
	int _col2 = obj.col;
	Matrix<T> tMatrix(_row1,_col2,0);
	for(int i = 0; i<_row1; i++){
		for(int j = 0; j<_col2; j++){
			for(int k = 0; k<_row2; k++){
				tMatrix.matrix[i][j] = tMatrix.matrix[i][j] + (matrix[i][k]*obj.matrix[k][j]);
				
			}
		}
	}
	return tMatrix;
};

template<class T>
Matrix<T> Matrix<T>::operator+(const T number){
	return islem('+', number);
};
template<class T>
Matrix<T> Matrix<T>::operator-(const T number){
	return islem('-', number);
};
template<class T>
Matrix<T> Matrix<T>::operator*(const T number){
	return islem('*', number);
};
template<class T>
Matrix<T> Matrix<T>::operator/(const T number){
	return islem('/', number);
};
template<class T>
Matrix<T> Matrix<T>::operator%(const T number){
	return islem('%', number);
};
template<class T>
Matrix<T> Matrix<T>::operator^(const T number){
	return islem('^', number);
};

template<class T>
Matrix<T> Matrix<T>::islem(char operation, T number){
	int _row = row;
	int _col = col;
	Matrix<T> tMatrix(_row,_col,0);
	for(int i = 0; i<_row; i++){
		for(int j = 0; j<_col; j++){
			if(operation == '+'){
				tMatrix.matrix[i][j] = matrix[i][j] + number;
			}
			else if(operation == '-'){
				tMatrix.matrix[i][j] = matrix[i][j] - number;
			}
			else if(operation == '*'){
				tMatrix.matrix[i][j] = matrix[i][j] * number;
			}
			else if(operation == '/'){
				tMatrix.matrix[i][j] = matrix[i][j] / number;
			}
			else if(operation == '%'){
				tMatrix.matrix[i][j] = (int)matrix[i][j] % (int)number;
			}
			else if(operation == '^'){
				int us = matrix[i][j];
				for(int k = 0; k < number-1; k++ ){
					matrix[i][j] = matrix[i][j] * us;
				}
				tMatrix.matrix[i][j] = matrix[i][j];
			}
		}
	}
	return tMatrix;
}

template<class T>
void Matrix<T>::Tr(){
	Transpoze();
}

template<class T>
void Matrix<T>::Transpoze(){
	Matrix<int>* tMatrix = new Matrix(col,row,0);
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			tMatrix->matrix[j][i] = matrix[i][j];
		}
	}
	matrix = tMatrix->matrix;
	int tmp = row;
	row = col;
	col = tmp;
	delete tMatrix;
}

// eleman düzeyinde çarpma -> matris boyutları aynı olmalı
template<class T>
void Matrix<T>::emul(Matrix<T>* obj){
	Matrix<T>* tMatrix = new Matrix(row,col,0);
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			tMatrix->matrix[i][j] = matrix[i][j] * obj->matrix[i][j];
		}
	}
	matrix = tMatrix->matrix;
}

template<class T>
Matrix<T> Matrix<T>::inv(){
	return inversion();
}

// matrisin tersi , sadece görüntülüyor ve matrisi birim matrise çevirerek sağlama yapıyor. birim matris ise matrisin tersi haline geliyor.
template<class T>
Matrix<T> Matrix<T>::inversion(){
	Matrix<T>* bM  = new Matrix<T>(row,col,'e');
	Matrix<T>* temp  = new Matrix<T>(row,col,0);
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			temp->matrix[i][j] = matrix[i][j];
		}
	}
	T k,l;
	for(int i=0; i<row; i++){
		k = temp->matrix[i][i];
		for(int j=0;j<col;j++){
			temp->matrix[i][j] = temp->matrix[i][j] / k;
			bM->matrix[i][j] = bM->matrix[i][j] / k;
		}
		for(int j=0;j<row;j++){
			if(j!=i){
				l = temp->matrix[j][i];
				for(int m=0;m<col;m++){
					temp->matrix[j][m] = temp->matrix[j][m] - (temp->matrix[i][m]*l);
					bM->matrix[j][m] = bM->matrix[j][m] - (bM->matrix[i][m]*l);
				}
			}
		}
	}
	return *bM;
}

template<class T>
T Matrix<T>::det(){
	return determinant(this);
}

template<class T>
T Matrix<T>::determinant(Matrix<T>* obj){
	int result = 0;
	int _row = obj->row;
	int _col = obj->col;
	if(_row == 1){
		return obj->matrix[0][0];
	}
	if(_row == 2){
		result += obj->matrix[0][0] * obj->matrix[1][1];
		result -= obj->matrix[0][1] * obj->matrix[1][0];
		return result;
	}
	for(int i=0;i<_col;i++){
		Matrix<T>* temp = new Matrix<T>(_row-1,_col-1,0);
		for(int j=1;j<_row;j++){
			for(int k=0; k<_col;k++){
				if(k < i){
					temp->matrix[j-1][k] = obj->matrix[j][k];
				}else if(k > i){
					temp->matrix[j-1][k-1] = obj->matrix[j][k];
				}
			}
		}
		result += obj->matrix[0][i] * pow(-1,i) * determinant(temp);
	}
	return result;
}
template<class T>
int Matrix<T>::getItem(int r, int c){
	return matrix[r][c];
}

template<class T>
void Matrix<T>::printTable(string* r, string* c){
	for(int i=-1; i<row; i++){
		for(int j=0; j<=col; j++){
			if((j==0) && (i==-1)){
				cout<<setw(8)<<"X";
			}else if(i==-1){
				cout<<setw(4)<<c[j-1];
			}else if(j==0){
				cout<<setw(8)<<r[i];
			}else{
				cout<<setw(4)<<matrix[i][j-1];
			}
		}
		cout<<endl;
	}
}
template class Matrix<int>;

template<class T>
Table<T>::Table(){
	row = 10;
	col = 10;
	c = new string[col];
	r = new string[row];
	for(int i=0;i<col;i++){
		c[i] = generateColumnName(i+1);
	}
	for(int i=0;i<row;i++){
		ostringstream str1;
		str1 << (i+1); 
		r[i] = str1.str();
	}
    matrix = new Matrix<int>(10,10,0);
}
template<class T>
Table<T>::Table(int row, int col, int d){
	this->row = row;
	this->col = col;
	this->c = new string[this->col];
	this->r = new string[this->row];
	for(int i=0;i<this->col;i++){
		c[i] = generateColumnName(i+1);
	}
	for(int i=0;i<this->row;i++){
		ostringstream str1;
		str1 << (i+1); 
		r[i] = str1.str();
	}
    matrix = new Matrix<int>(this->row,this->col,d);
}

template<class T>
string Table<T>::generateColumnName(int n) 
{ 
    string str; 
    int i = 0; 
    while (n>0) 
    { 
        int rem = n%26; 
        if (rem==0) 
        { 
            str.push_back('Z'); 
            n = (n/26)-1; 
        } 
        else
        { 
            str.push_back((rem-1) + 'A'); 
            n = n/26; 
        } 
    } 
	reverse(str.begin(),str.end());
	return str;
} 

template<class T>
void Table<T>::print(){
	matrix->printTable(r,c);
}

template<class T>
int Table<T>::itemAt(int r, int c){
	if(row <= r || col <= c)
	{
		cout<<"Boyut disinda bir deger verilmistir."<<endl;
		return 0;
	}
	return matrix->getItem(r, c);
}
template<class T>
int Table<T>::itemAt(string s){
	char alphabet[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
	
	string row="";
	string col="";
	int cnt = 0;
	bool cntrl = false;
	
	for(int i=0 ;i<s.length(); i++){
		for(int j=0; j<26; j++){
			if(alphabet[j] == s[i]){
				col += s[i];
				cntrl = true;
			}
			if(cntrl){
				break;
			}
		}
		if(!cntrl){
			for(int j=i ;j<s.length(); j++){
				row += s[j];
			}
			break;
		}
		else{
			cntrl = false;
		}
	}
	
	int iRow = 0;
	for (int i = 0; i < this->row; i++) {
        if (r[i] == row) {
            iRow = i;
			break;
        }
    }
	return getItem(iRow, col);
}

template<class T>
int Table<T>::getItem(int row, string col){
	int colNumber=0;
	bool cntrlColNumber = false;
	
	for(int i=0; i<this->col; i++){
		if(c[i] == col){
			colNumber = i;
			cntrlColNumber = true;
			break;
		}
	}
	if(cntrlColNumber == 0 
		|| (this->row < row
		&& 0 >= row)){
		cout<<"Boyut disinda bir deger verilmistir."<<endl;
		return 0;
	}else{
		return matrix->getItem(row, colNumber);
	}
}

template<class T>
int Table<T>::itemAt(string rs,string cs){
	string tRow;
	for (int i = 0; i < sizeof(r); i++) {
        if (r[i] == rs) {
			return getItem(i, cs);
            //tRow.push_back(i);
			//break;
        }
    }
	return getItem(atoi(tRow.c_str()), cs);
}

template<class T>
bool Table<T>::setRowNames(string s[], int n){
	for(int i=0; i < n; i++){
		if(sizeof(r) > i){
			r[i] = s[i];
		}
	}
}
template<class T>
bool Table<T>::setColNames(string s[], int n){
	for(int i=0; i < n; i++){
		if(sizeof(c) > i){
			c[i] = s[i];
		}
	}
}
template class Table<int>;

int main()
{
	
  int sec=10;
	switch(sec){
	case 1 :
		{
			Matrix<double> a(2,3,1.);
			a.print();
			Matrix<double> b(2,3,9.);
			b.print();
			a = b + a*1;
			a.print("file.txt"); // içinde 10 olan matris olmalı*/
		}
		break;
	case 2 :
		{
			Matrix<double> m1(4,4,'e');
			Matrix<double> m2(4,4,'r'); // (5,5) boyutu dene
			m2.print();
			m1 = m1 * m2;
			m1.print();
		}
	  break;
	case 3 :
		{
			// STL vector kullanan buradan puan alamaz
			Matrix<double> m3(5,4,1);
			m3.print();
			// aşağıdaki durumları ayrı ayrı dene
			//m3.resize(2,6);
			//m3.resize(10,6);
			m3.resize(10,3);
			m3.print();
		}
	  break;
	case 4 :
		{
			// lineer matris oluştur
			Matrix<double> m4(3,3,'r');
			//Matrix<double> m5 = m4 * m4.inv();
			//m5.print(); // birim matris olmalı

			//Matrix<double> m6(3,3,'L');
			//cout<<m6.det(); // sonuç 0 olmalı
		}
		break;
	/*case 5 :
	  break;
	case 6 :
	  Image im1("custom.bin");
	  im1.threshold(128);
	  im1.print("custom2.bin");
	  break;
	case 7 :
	  Image im2("imfile.bmp");
	  im2.print("imfile2.bmp");
	  break;
	case 8 :
	  Image im3("custom.bin");
	  im3.threshold(128);
	  im3.erosion();
	  im3.print("custom2.bin");
	  break;
	  */
	case 9 :
		{
			Table<int> t1(4,4,1);
			t1.print(); // satır ve sütun etiketleri de yazılmalı
			string s[] = {"Sat1","Sat2"};
			t1.setRowNames(s,2);
			t1.print();
		}	
		break;
	case 10 :
		{
			Table<int> t2(3,4,'r');
			t2.print();
			cout<<t2.itemAt(1,1)<<endl; // (1,1) deki eleman
			cout<<t2.itemAt("A2")<<endl;  // (2,0) deki eleman
			string s2[] = {"S1","S2","S3"};
			t2.setColNames(s2,3);
			cout<<t2.itemAt("1","S2")<<endl;  // (1,1) deki eleman
		}
		break;
	}
	
	
	/*
		Tamsayı değerleri olan Matrix sınıfı örnekleri

	
	// 0 değerli 10x10 matris
	Matrix<int> *m1 = new Matrix<int>();
	cout<<"0 değerli 10x10 matris"<<endl;
	m1->print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	// 9 değerli 3x4 matris
	//Matrix<int> *m2 = new Matrix<int>(3,2,1);
	Matrix<int> *m2 = new Matrix<int>(3,4,7);
	cout<<"9 değerli 3x4 matris"<<endl;
	m2->print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;

	// 5x5 birim matris
	Matrix<int> *m3 = new Matrix<int>(5,5,'e');
	cout<<"5x5 birim matris"<<endl;
	m3->print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	// 15x15 0-255 arası rasgele matris
	Matrix<int> *m4 = new Matrix<int>(15,15,'r');
	cout<<"15x15 0-255 arası rasgele matris"<<endl;
	m4->print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	// resize -> matris oluşturulduktan sonra boyutu değiştirilmelidir
	cout<<"matris oluşturulduktan sonra boyutu değiştirilmelidir"<<endl;
	cout<<"	önce:"<<endl;
	m2->print();
	cout<<endl;
	cout<<"	sonra:";
	m2->resize(3,3);
	cout<<endl;
	m2->print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;

	// matrisin dosyaya yazdırılması
	cout<<"matrisin dosyaya yazdırılması"<<endl;
	cout<<"m3->print('file.txt');"<<endl;
	m3->print("file.txt");
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	// matris -matris işlemleri 
	// ----------------------------
	Matrix<int> m5(2,4,2);
	Matrix<int> m6(4,3,'r');
	
	// (*m1) = (*m1) + (*m2);
	cout<<"matris -matris işlemleri"<<endl;
	cout<<"(*m3)"<<endl;
	m3->print();
	cout<<"(*m2)"<<endl;
	m2->print();
	cout<<"(*m1) = (*m3) + (*m2)"<<endl;
	(*m1) = (*m3) + (*m2);
	m1->print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	// m1 = m1 - m2;
	cout<<"m5"<<endl;
	m5.print();
	cout<<"m6"<<endl;
	m6.print();
	cout<<"m8"<<endl;
	cout<<"m5 = m5 - m6"<<endl;
	m5 = m5 - m6;
	m5.print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	// m1 = m1 * m2;
	Matrix<int> m7(3,3,'e');
	Matrix<int> m8(4,3,4);
	
	cout<<"m7"<<endl;
	m7.print();
	cout<<"m8"<<endl;
	m8.print();
	cout<<endl;
	cout<<"m7 = m7 * m8"<<endl;
	m7 = m7 * m8;
	m7.print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	// matris - skalar işlemleri 
	// ------------------------
	
	cout<<"matris - skalar işlemleri"<<endl;
	Matrix<int> m9(3,3,'e');
	cout<<"m9"<<endl;
	m9.print();
	cout<<endl;
	cout<<"m9 = m9 + 12"<<endl;
	m9 = m9 + 12;
	m9.print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	cout<<"m9"<<endl;
	m9.print();
	cout<<endl;
	cout<<"m9 = m9 - 20"<<endl;
	m9 = m9 - 20;
	m9.print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	cout<<"m9"<<endl;
	m9.print();
	cout<<endl;
	cout<<"m9 = m9 * 10"<<endl;
	m9 = m9 * 10;
	m9.print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	cout<<"m9"<<endl;
	m9.print();
	cout<<endl;
	cout<<"m9 = m9 / 10"<<endl;
	m9 = m9 / 10;
	m9.print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	cout<<"m9"<<endl;
	m9.print();
	cout<<endl;
	cout<<"m9 = m9 % 10"<<endl;
	m9 = m9 % 10;
	m9.print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	cout<<"m9"<<endl;
	m9.print();
	cout<<endl;
	cout<<"m9 = m9 ^ 2"<<endl;
	m9 = m9 ^ 2;
	m9.print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	// matris işlemleri 
	// ------------------------
	
	Matrix<int> *m10 = new Matrix<int>(3,4,7);
	Matrix<int> *m11 = new Matrix<int>(5,3,4);
	
	// m10->Tr()
	cout<<"matris işlemleri"<<endl;
	cout<<"m10"<<endl;
	m10->print();
	m10->Tr();
	cout<<"m10->Tr(); "<<endl;
	m10->print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	// m10->emul(m11)
	cout<<"m10"<<endl;
	m10->print();
	cout<<"m11"<<endl;
	m11->print();
	cout<<"m10->emul(m11)"<<endl;
	m10->emul(m11);
	m10->print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	// m10->inv()
	*m10 = new Matrix<int>(5,5,'r');
	cout<<"m10"<<endl;
	m10->print();
	cout<<"m10->inv()"<<endl;
	m10->inv();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	// m10->det()
	*m10 = new Matrix<int>(3,3,'r');
	cout<<"m10"<<endl;
	m10->print();
	cout<<"m10->det()"<<endl;
	m10->det();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
		*/
	
	/*	TABLE
		Matrix sınıfından türetilmiş Table sınıfı
	
	Table<int>* t = new Table<int>();
	
	cout<<"Table<int>() -> sıfırlara sahip 10x10'luk bir tablo"<<endl;
	t->print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	t = new Table<int>(5,5,3);

	cout<<"Table<int>(5,5,3) -> d değerine sahip rxc boyutlu bir tablo"<<endl;
	t->print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;

	t = new Table<int>(5,5,'r');
	
	cout<<"Table<int>(5,5,'r') -> 0-255 arasında rastgele değerlere sahip tablo"<<endl;
	t->print();
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	
	cout<<endl;
	cout<<"t->print()"<<endl;
	t->print();
	cout<<"itemAt(int r,int c) →   [r,c] indisindeki değeri geri döndür"<<endl;
	cout<<"t->itemAt(3,3): "<<t->itemAt(3,3)<<endl;
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	
	cout<<endl;
	cout<<"t->print()"<<endl;
	t->print();
	cout<<"itemAt(string s) → [2,0] indisindeki değeri geri döndür demektir"<<endl;
	cout<<"t->itemAt('A2'): "<<t->itemAt("A2")<<endl;
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	
	cout<<endl;
	cout<<"t->print()"<<endl;
	t->print();
	cout<<"itemAt(string rs,string cs)  → itemAt('1','A')"<<endl;
	cout<<"t->itemAt('1','A'): "<<t->itemAt("1","A")<<endl;
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	
	
	string s[] = {"0. satır","1. satır","2. satır"};
	cout<<endl;
	cout<<"t->print()"<<endl;
	t->print();
	cout<<endl;
	cout<<"setRowNames(string s[],int n)"<<endl;
	cout<<"string s[] = {'0. satır','1. satır','2. satır'}"<<endl;
	cout<<"t->setRowNames(s,3)"<<endl;
	cout<<endl;
	t->setRowNames(s,3);
	t->print();
	cout<<endl;
	cout<<"t->itemAt('0. satır','B'): "<<t->itemAt("0. satır","B")<<endl;
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	
	
	
	string r[] = {"cl1","cl2","cl3"};
	cout<<endl;
	cout<<"t->print()"<<endl;
	t->print();
	cout<<endl;
	cout<<"setColNames(string r[],int n)"<<endl;
	cout<<"string r[] = {'cl1','cl2','cl3'}"<<endl;
	cout<<"t->setColNames(r,3)"<<endl;
	cout<<endl;
	t->setColNames(r,3);
	t->print();
	cout<<endl;
	cout<<"t->itemAt('0. satır','cl2'): "<<t->itemAt("0. satır","cl2")<<endl;
	cout<<endl;
	cout<<"------------------------------------------------"<<endl;
	*/
	
	return 0;

}
