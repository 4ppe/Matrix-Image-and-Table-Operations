/*
    Alper Babadağ  365256
	
	Email:	365256@ogr.ktu.edu.tr
	
	Bu projede Matrix Image ve Table şablon sınıfları oluşturuldu.
	Image ve Table sınıfları Matrixten miras alınarak türetildi.
	Matrix sınıfında matris işlemleri yapabilecek fonksiyonlar
	yazıldı.
	Image sınıfında "bin" formatındaki dosyaları okuma
	yazma, renkli görüntüyü griye, gri görüntüyü ikili görüntüye
	çevirme ve görüntü işlemedeki bazı morfolojik işlemleri yapan
	fonksiyonlar yazıldı.
	Table sınıfında ise satır ve sütün isimlendirmeleri ve
	girdi/çıktı manipülatörleri ile kullanıcı açısından rahat 
	farkedilebilir bir tablo oluşturuldu ve bu tabloda satır ismi
	güncelleme, sütün ismi güncelleme, verilen indisdeki değeri alma  
	ve konsola yazdırma fonksiyonlarını yazıldı.

*/

#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include <iomanip>
#include <fstream>
#include <math.h>
#include <string>

using namespace std;


template<typename TT>
class Matrix {

    protected:       
        TT **array2D;   // store matrix values 
        int ROW;		// store row count
        int COLUMN;  	// store column count

    public:
        Matrix();
        Matrix(int ,int ,TT);
        Matrix(int ,int, char);
		//virtual ~Matrix();
		void resize(int, int);
        void print() const;
		void print(string) const;
		Matrix<TT> operator+(Matrix<TT>);
		Matrix<TT> operator-(Matrix<TT>);	
		Matrix<TT> operator*(Matrix<TT>);
		Matrix<TT> operator+(int);
		Matrix<TT> operator-(int);	
		Matrix<TT> operator*(int);
		Matrix<TT> operator/(int);
		Matrix<TT> operator%(int);
		Matrix<TT> operator^(int);
        Matrix<TT> T();
		Matrix<TT> emul(Matrix<TT>);
		Matrix<double> inv();
		int det();
        TT getValue(int row,int column);
		int getRow();
		int getColumn();
		TT **getArray2D();
		void editRow(int);
		void editColumn(int);
        void setValue(int row, int column,TT value);
};


// default constructor
// 10x10 matrix with zeros
template<typename TT>
Matrix<TT>::Matrix()
{   
    ROW = 10;
    COLUMN = 10;
    
    array2D = 0;
    array2D = new TT*[ROW];
    for (int h = 0; h < ROW; h++)  
    {
        array2D[h] = new TT[COLUMN];

        for (int w = 0; w < COLUMN; w++)
        {
            array2D[h][w] = 0;
        }
    }
}


// constructor
// creates matrix according to given rows and columns
// and fills the matrix according to the given value
template<typename TT>
Matrix<TT>::Matrix(int row,int column,TT value)
{
    ROW = row;
    COLUMN = column;

    array2D = 0;
    array2D = new TT*[ROW];
    for (int h = 0; h < ROW; h++)  
    {
        array2D[h] = new TT[COLUMN];

        for (int w = 0; w < COLUMN; w++)
        {
            array2D[h][w] = value;
        }
    }
}


// constructor given ch parameter number of columns and number of rows
// if ch is 'r' creates random valued matrix
// if ch is 'e' creates identification matrix
template<typename TT>
Matrix<TT>::Matrix(int row,int column,char ch)
{
    ROW = row;
    COLUMN = column;

	if (ch == 'e') // identification matrix
	{
		if (ROW == COLUMN)
		{	
			array2D = 0;
			array2D = new TT*[ROW];
			for (int h = 0; h < ROW; h++)  
			{
				array2D[h] = new TT[COLUMN];

				for (int w = 0; w < COLUMN; w++)
				{	
					if (h == w)
					{
						array2D[h][w] = 1;
					}else
					{
						array2D[h][w] = 0;
					}
				}
			}
		}
	}else if (ch == 'r') //matrix with random integers between [0-255] 
	{
		srand((unsigned)time(0)); 
		array2D = 0;
		array2D = new TT*[ROW];
		for (int h = 0; h < ROW; h++)  
		{
			array2D[h] = new TT[COLUMN];

			for (int w = 0; w < COLUMN; w++)
			{	
					array2D[h][w] = (rand()%255);
			}
		}
	}else
	{
		cout<<"invalid argument";
	}
}

/* destructor
template<class TT>
Matrix<TT>::~Matrix()
{
	for (int i = 0; i < this->COLUMN; i++)
		delete[] this->array2D[i];
	delete[] this->array2D;
}
*/


// matrix resize
template<typename TT>
void Matrix<TT>::resize(int row,int column)
{   

	if (this->ROW > row)
	{
		for (int i = row; i < this->ROW; i++)
		{
			delete [] array2D[i];
		}
		
	}else
	{
		this->array2D = (TT**)realloc(this->array2D,row*sizeof(TT*));
	}
	
	this->ROW = row;

	for (int i = 0; i < this->ROW; i++)
	{
			this->array2D[i] = (TT*)realloc ( array2D[i]
											  ,column*sizeof(TT));
	}
	this->COLUMN = column;
}


// print matrix on the screen
template<typename TT>
void Matrix<TT>::print() const
{	
	cout<<"\n\n";
	for(int i=0;i<ROW;i++)
	{
		for(int j=0;j<COLUMN;j++)
		{
			cout<<setw(5)<<array2D[i][j]<<" ";
		}
		cout<<endl;
	}
}


// write matrix to file 
template<typename TT>
void Matrix<TT>::print(string file) const
{	
	ofstream myfile (file);
	if (myfile.is_open())
	{
		cout<<"\n\n";
		for(int i=0;i<ROW;i++)
		{
			for(int j=0;j<COLUMN;j++)
			{
				myfile << setw(5) << array2D[i][j] << " ";
			}
			myfile<<endl;
		}	

		myfile.close();
	}
  	else cout << "Unable to open file";
}


// + operator overloading
template<typename TT>
Matrix<TT> Matrix<TT>::operator+(Matrix<TT> m)
{	
	if(ROW == m.ROW || COLUMN == m.COLUMN)
	{
		int a,b;
		Matrix<int> temp(ROW,COLUMN,0);

		for (int h = 0; h < ROW; h++)
		{
			for (int w = 0; w < COLUMN; w++)
			{   
				a = array2D[h][w];
				b = m.getValue(h,w);
				temp.setValue(h,w,(a+b));
			}
		}
		return temp;
	}else
	{	cout<<endl<<"Could not perform '+' operator "
			<<"dimensions of matrices do not match"<<endl;
	}	
}


// - operator overloading
template<typename TT>
Matrix<TT> Matrix<TT>::operator-(Matrix<TT> m)
{	
	if(ROW == m.ROW || COLUMN == m.COLUMN)
	{
		int a,b;
		Matrix<int> temp(ROW,COLUMN,0);

		for (int h = 0; h < ROW; h++)
		{
			for (int w = 0; w < COLUMN; w++)
			{   
				a = array2D[h][w];
				b = m.getValue(h,w);
				temp.setValue(h,w,(a-b));
			}
		}
		return temp;
	}else
	{	cout<<endl<<"Could not perform '-' operator "
			<<"dimensions of matrices do not match"<<endl;
	}
	
}


// * operator overloading
template<typename TT>
Matrix<TT> Matrix<TT>::operator*(Matrix<TT> m)
{	

	if(COLUMN == m.ROW)
	{
		int a,b;
		Matrix<int> temp(ROW,m.COLUMN,0);

		for (int h = 0; h < ROW; h++)
		{
			for (int w = 0; w < m.COLUMN; w++)
			{   
				for (int k = 0; k < COLUMN; k++)
				{	
					a = array2D[h][k];
					b = m.getValue(k,w);
					temp.array2D[h][w] += (a*b);
				}
			}
		}
		return temp;
	}else
	{	
		cout<<endl<<"Error '*' operator! column of first matrix "
				  << "not equal to row of second."<<endl;
	}
}


// + operator overloading
template<typename TT>
Matrix<TT> Matrix<TT>::operator+(int m)
{	
	int a;
	Matrix<int> temp(ROW,COLUMN,0);

	for (int h = 0; h < ROW; h++)
	{
		for (int w = 0; w < COLUMN; w++)
		{   
			a = array2D[h][w];
			temp.setValue(h,w,(a+m));
		}
	}
	return temp;
}


// - operator overloading
template<typename TT>
Matrix<TT> Matrix<TT>::operator-(int m)
{	
	int a;
	Matrix<int> temp(ROW,COLUMN,0);

	for (int h = 0; h < ROW; h++)
	{
		for (int w = 0; w < COLUMN; w++)
		{   
			a = array2D[h][w];
			temp.setValue(h,w,(a-m));
		}
	}
	return temp;
}


// * operator overloading
template<typename TT>
Matrix<TT> Matrix<TT>::operator*(int m)
{	
	int a;
	Matrix<int> temp(ROW,COLUMN,0);

	for (int h = 0; h < ROW; h++)
	{
		for (int w = 0; w < COLUMN; w++)
		{   
			a = array2D[h][w];
			temp.setValue(h,w,(a*m));
		}
	}
	return temp;
}


// / operator overloading
template<typename TT>
Matrix<TT> Matrix<TT>::operator/(int m)
{	
	int a;
	Matrix<int> temp(ROW,COLUMN,0);

	for (int h = 0; h < ROW; h++)
	{
		for (int w = 0; w < COLUMN; w++)
		{   
			a = array2D[h][w];
			temp.setValue(h,w,(a/m));
		}
	}
	return temp;
}


// % operator overloading
template<typename TT>
Matrix<TT> Matrix<TT>::operator%(int m)
{	
	int a;
	Matrix<int> temp(ROW,COLUMN,0);

	for (int h = 0; h < ROW; h++)
	{
		for (int w = 0; w < COLUMN; w++)
		{   
			a = array2D[h][w];
			temp.setValue(h,w,(a%m));
		}
	}
	return temp;
}


// ^ operator overloading
template<typename TT>
Matrix<TT> Matrix<TT>::operator^(int m)
{	
	int a;
	Matrix<int> temp(ROW,COLUMN,0);

	for (int h = 0; h < ROW; h++)
	{
		for (int w = 0; w < COLUMN; w++)
		{   
			a = 1;
			for (int i = 0; i < m; i++)
			{
				a = a * array2D[h][w];
			}
			
			temp.setValue(h,w,a);
		}
	}
	return temp;
}


// returns the transpose Matrix<TT>
template<typename TT>
Matrix<TT> Matrix<TT>::T()
{   
    Matrix<int> temp(COLUMN,ROW,0);

    for(int i = 0; i < ROW; ++i)
	{
        for(int j = 0; j < COLUMN; ++j)
        {	
			TT a = array2D[i][j];
            temp.setValue(j,i,a);
        }
	}
    return temp;
}


// elementwise multiply
template<typename TT>
Matrix<TT> Matrix<TT>::emul(Matrix<TT> m)
{	
	if(this->COLUMN == m.getColumn() && this->ROW == m.getRow())
	{
		int a,b;
		Matrix<TT> temp(this->ROW,this->COLUMN,0);

		for (int h = 0; h < ROW; h++)
		{
			for (int w = 0; w < m.COLUMN; w++)
			{   
				a = array2D[h][w];
				b = m.getValue(h,w);
				temp.setValue(h,w,(a*b));
			}
		}
		return temp;
	}else
	{	
		cout<<endl<<"Error emul function! Columns and rows" 
				  <<"are not equal"<<endl;
	}
}


// matrix inversion returns Matrix<double>
template<typename TT>
Matrix<double> Matrix<TT>::inv()
{	
	if (ROW == COLUMN)
	{
		Matrix<double> identMatrix(ROW,COLUMN,'e');

		double a,b;
		double doubleArray[ROW][COLUMN];

		// copy values of array2D to doubleArray for double type
		for (int x = 0; x < ROW; x++)
		{
			for (int y = 0; y < COLUMN; y++)
			{
				doubleArray[x][y] = array2D[x][y];
			}
		}
		
		for (int i = 0; i < ROW; i++)
		{
			a = doubleArray[i][i];
			for (int j = 0; j < ROW; j++)
			{
				doubleArray[i][j] = doubleArray[i][j]/a;
				identMatrix.getArray2D()[i][j] = 
					identMatrix.getArray2D()[i][j]/a;
			}
			for (int k = 0; k < ROW; k++)
			{
				if (k!=i)
				{
					b = doubleArray[k][i];
					for (int j = 0; j < ROW; j++)
					{
						doubleArray[k][j] = doubleArray[k][j]
										    -(doubleArray[i][j]*b);
						identMatrix.getArray2D()[k][j] = 
							identMatrix.getArray2D()[k][j]
							-(identMatrix.getArray2D()[i][j]*b);
					}	
				}	
			}	
		}
		return identMatrix;

	}else
	{
		cout<<endl<<"Error! the matrix you're trying" 
				  <<"to inverse is not square"<<endl;
	}
	
}


// returns determinant
template<typename TT>
int Matrix<TT>::det()
{	
    if ( this->ROW == this->COLUMN )
    {	
        int det = 0;
		if (this->ROW == 1)
		{
			return array2D[0][0];
		}
		
        if (this->ROW == 2)
		{
			int a = this->array2D[0][0];
			int b = this->array2D[1][1];
			int c = this->array2D[1][0];
			int d = this->array2D[0][1];
			return ( (a*b) - (c*d) );
		}
        else
        {
            Matrix<int> temp(this->ROW, this->COLUMN,0); 
            for( int x = 0 ; x < this->ROW ; x++ )
            {	
				int subi = 0;
                for(int i = 1 ; i < this->ROW ; i++ )
                {
					int subj = 0;
                    for(int j = 0 ; j <  this->ROW ;j ++)
                    {
                       if( j == x )
                       {
                          continue;
                       }
                       temp.setValue(subi,subj,(this->array2D[i][j]));
                       subj++;
                    }
					subi++;
                }
				temp.editRow( temp.getRow() - 1 );
				temp.editColumn( temp.getColumn() -1 );

			    det = det + (pow(-1,x) * this->array2D[0][x]
				 										* temp.det());
            }
        }
		return det;
    }
    else
    {
        cout<< "Error! the matrix is not square";
    }
}


// gets the value according to given matrix indexes
template<typename TT>
TT Matrix<TT>::getValue(int row,int column)
{
    return array2D[row][column];
}


// returns ROW count
template<typename TT>
int Matrix<TT>::getRow()
{	
	return ROW;
}


// returns COLUMN count
template<typename TT>
int Matrix<TT>::getColumn()
{	
	return COLUMN;
}


// returns array2D
template<typename TT>
TT** Matrix<TT>::getArray2D()
{	
	return array2D;
}


// update the ROW according to the entered parameter
template<typename TT>
void Matrix<TT>::editRow(int a)
{	
	ROW = a;
}


// update the COLUMN according to the entered parameter
template<typename TT>
void Matrix<TT>::editColumn(int a)
{	
	COLUMN = a;
}


// changes the value according to the given matrix indexes
template<typename TT>
void Matrix<TT>::setValue(int row,int column,TT value)
{	
    array2D[row][column] = value;
}


// created struct to store color values
struct RGB
{
	int red = 0;
	int green = 0;
	int blue = 0;
};


// An Image class derived from Matrix class,
// Image type could be a structure rgb which has three values
template<typename TT>
class Image : private Matrix<RGB>
{	
	private:
		bool isBinary();
	public:
        Image();
		Image(int ,int);
		Image(string filename,string format);
		void imRead(string,string);
		void imWrite(string,string);
		void color2gray();
		void gray2binary(int);
		void erosion();
		void dilation();
		void opening();
		void closing();
		void printBinaryImage();
};


// default constructor, with 255x255 image size,
// default pixel values are 0
template<typename Type>
Image<Type>::Image() : Matrix(255, 255, RGB())
{	
	for (int i = 0; i < 255; i++)
	{
		for (int j = 0; j < 255; j++)
		{
			array2D[i][j].blue 	= 0;
			array2D[i][j].red 	= 0;
			array2D[i][j].green = 0;
		}
	}
}


// contructor with (widthxheight) image size,
// default pixel values are 0.
template<typename Type>
Image<Type>::Image(int row,int column) : Matrix(row, column, RGB())
{	
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			array2D[i][j].blue 	= 0;
			array2D[i][j].red 	= 0;
			array2D[i][j].green = 0;
		}
	}
}


// read image from file
template<typename Type>
Image<Type>::Image(string filename,string format):Matrix(1,1, RGB())
{	
	this->imRead(filename,format); // reads file
}


// reads image from file and fill existing matrix
template<typename Type>
void Image<Type>::imRead(string filename, string format)
{	
	if (format.compare("bin") == 0)
	{
		char *buffer;
		string file__name = filename +"."+ format;
		ifstream myFile (file__name, ios::binary);
		int row_,column_;
		buffer = new char[1];
		if (myFile) {

			myFile.read(buffer,1);
			row_ = (unsigned int)(unsigned char)(buffer[0]);
			myFile.read(buffer,1);
			column_ = (unsigned int)(unsigned char)(buffer[0]);

			// resize matrix to new size
			resize(row_,column_);

			for (int i = 0; i < this->ROW; i++)
			{
				for (int j = 0; j < this->COLUMN; j++)
				{	
				   myFile.read(buffer,1);
				   array2D[i][j].green = (unsigned int)(unsigned char)
												(buffer[0]);
				}
			}
		}
	}else
	{
		cout<<endl << "the format of the file you "
				   << "are trying to read is not supported"<<endl;
	}
}


// write image to a file specified with filename.
// Format could be “bin”.
template<typename Type>
void Image<Type>::imWrite(string filename, string format)
{
	char *buffer;
	string file__name = filename +"."+ format;
    ofstream myFile (file__name, ios::binary);

	if (myFile) 
	{
		if (format.compare("bin") == 0 )   // if format is bin
		{
			buffer = new char[1];
			*buffer = (char)(unsigned char)(this->ROW);
			myFile.write(buffer,1); // 
			
			*buffer = (char)(unsigned char)(this->COLUMN);
			myFile.write(buffer,1);

			for (int i = 0; i < this->ROW; i++)
			{
				for (int j = 0; j < this->COLUMN; j++)
				{	
					*buffer = (char)(unsigned char)
								(this->array2D[i][j].green);
					myFile.write(buffer,1);
				}
			}
		}
    }
}


// convert color image to gray level
template<typename Type>
void Image<Type>::color2gray()
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; i < COLUMN; i++)
		{							// store gray value in green 
			array2D[i][j].green = 	(0.299*array2D[i][j].red)  
									+(0.587*array2D[i][j].green)
									+(0.114*array2D[i][j].blue);
			array2D[i][j].blue = 0;
			array2D[i][j].red = 0;
		}
	}
}


// convert gray image to binary with given threshold
template<typename Type>
void Image<Type>::gray2binary(int thr)
{
	for (int i = 0; i < this->ROW; i++)
	{
		for (int j = 0; j < this->COLUMN; j++)
		{	
			if (thr >= array2D[i][j].green)
			{
				this->array2D[i][j].green = 0;
			}else
			{
				this->array2D[i][j].green = 1;
			}
		}
	}
}


// morphological erosion
// image could be binary
template<typename Type>
void Image<Type>::erosion()
{	
	if (this->isBinary())
	{
		Matrix<RGB> m(this->ROW,this->COLUMN,RGB());

		for (int i = 0; i < this->ROW; i++)
		{
			for (int j = 0; j < this->COLUMN; j++)
			{
				m.setValue(i, j, (this->array2D[i][j]));
			}
		}
		
		for (int i = 0; i < this->ROW; i++)
		{
			for (int j = 0; j < this->COLUMN; j++)
			{	
				if (m.getArray2D()[i][j].green==1)
				{	
					for (int h = -1; h < 2; h++)
					{
						for (int w = -1; w < 2; w++)
						{	
							if ((i+h > 0)
								&& (j+w>0) 
								&& (i+h<this->ROW) 
								&& (j+w<this->COLUMN) 
								&& (m.getArray2D()[i+h][j+w].green 
															== 0))
							{	
								this->array2D[i][j].green = 0;
								h = 2; // Increases variable
								w = 2; // to exit for loops
							}		
						}		
					}
				}//end if
			}
		}
	}
}


// morphological dilation 
// image could be binary
template<typename Type>
void Image<Type>::dilation()
{	
	if (this->isBinary())
	{
		Matrix<RGB> m(this->ROW,this->COLUMN,RGB());

		for (int i = 0; i < this->ROW; i++)
		{
			for (int j = 0; j < this->COLUMN; j++)
			{
				m.setValue(i,j,(this->array2D[i][j]));
			}
		}		
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COLUMN; j++)
			{	
				if (m.getArray2D()[i][j].green==0)
				{	
					for (int h = -1; h < 2; h++)
					{
						for (int w = -1; w < 2; w++)
						{	
							if(	(i+h > 0) 
								&& (j+w>0)
								&& (i+h<this->ROW)
								&& (j+w<this->COLUMN)
								&& (m.getArray2D()[i+h][j+w].green 
															== 1)) 
							{
								this->array2D[i][j].green = 1;
								h = 2;
								w = 2;
							}	
						}		
					}
				}
			}
		}
	}
}


// morphological opening
// image could be binary
template<typename Type>
void Image<Type>::opening()
{
	this->erosion();
	this->dilation();
}


// morphological closing
// image could be binary
template<typename Type>
void Image<Type>::closing()
{
	this->dilation();
	this->erosion();
}


// checks whether the image is binary
template<typename Type>
bool Image<Type>::isBinary()
{	
	for (int i = 0; i < this->ROW; i++)
	{
		for (int j = 0; j < this->COLUMN; j++)
		{	
			// red, blue could be 0. green could be 0 or 1
			// in binary image
			if ((this->array2D[i][j].red == 0  
			 	&&	this->array2D[i][j].blue == 0	
				&&	this->array2D[i][j].green >= 0
				&&  this->array2D[i][j].green <= 1) == 0)
			{
				return false;
			}
		}
	}
	return true;
}


// prints binary image
template<typename Type>
void Image<Type>::printBinaryImage() 
{	
	cout<<"\n\n";
	for(int i=0;i<ROW;i++)
	{
		for(int j=0;j<COLUMN;j++)
		{
			cout<<array2D[i][j].green<<""<<flush;
		}
		cout<<endl;
	}
}


// A Table class derived from Matrix class
template<typename Type>
class Table : private Matrix<int>
{	
	protected:
		string* col_names = NULL; 	// store column names
		string* row_names = NULL;	// store row names
		void editColumnNames(); 
		void editRowNames();	
	public:
        Table();
		Table(int,int,int);
		Table(int,int,char);
		Type itemAt(int,int);
		Type itemAt(string, string);
		Type itemAt(string);
		void setRowNames(string str[],int n);
		void setColumnNames(string str[],int n);
		void print();
};


// default constructor, a 10x10 table with zeros
template<typename Type>
Table<Type>::Table():Matrix()
{   
	// edit names by column and row number
	editRowNames();
	editColumnNames();
}


//  row x column table with the value of given value
template<typename Type>
Table<Type>::Table(int row,int column,int value):Matrix(row,column
															,value)
{   
	editRowNames();
	editColumnNames();
}


// rxc table. if the ch is ‘r’, then the value of 
// the table could be random between 0 and 255. 
template<typename Type>
Table<Type>::Table(int row,int column,char ch):Matrix(row,column,ch)
{   
	if (ch == 'r')
	{
		editRowNames();
		editColumnNames();		
	}else
	{
		cout<<" constuctor parameter is could be 'r' "<<endl;
	}
}


// value according to the entered row,column numbers
template<typename Type>
Type Table<Type>::itemAt(int row,int column)
{   
	return getValue(row,column);
}


// return the value at the location specified by s. if s is “A2”,
// then the returning value could be the value at [2,0] index.
// sample code: itemAt("A2")
template<typename Type>
Type Table<Type>::itemAt(string str)
{   
	for (int i = 0; i < this->ROW; i++)
	{
		for (int j = 0; j < this->COLUMN; j++)
		{
			if (str[0]==col_names[j][0] && str[1] == row_names[i][0])
			{
				return this->array2D[i][j];
			}
		}
	}
	return -1;
}


// sample code : itemAt("1","A")
template<typename Type>
Type Table<Type>::itemAt(string str1, string str2)
{   
	for (int i = 0; i < this->ROW; i++)
	{
		for (int j = 0; j < this->COLUMN; j++)
		{
			if ( (str1.compare(row_names[i]) == 0) 
			   && str2.compare(col_names[j]) == 0)
			{
				return this->array2D[i][j];
			}
		}
	}
}


// set row names with given the string s. n is the length of s
template<typename Type>
void Table<Type>::setRowNames(string str[] ,int n)
{
	for (int i = 0; i < n; i++)
	{	
		if (str[i].length()>8)
		{
			string temp = str[i].substr(0,8);
			row_names[i] = temp;
		}else
		{
			row_names[i] = str[i];
		}
	}
}


// set col names with given the string s. n is the length of s
template<typename Type>
void Table<Type>::setColumnNames(string str[] ,int n)
{	
	for (int i = 0; i < n; i++)
	{
		if (str[i].length()>4)
		{
			string temp = str[i].substr(0,4);
			col_names[i] = temp;
		}else
		{
			col_names[i] = str[i];
		}
	}
}


// print the table values with row and col names, row names could be
// 8 characters at most and col names could be 4 characters at most.
template<typename Type>
void Table<Type>::print()
{	
	cout<<"\t ";
	for (int i = 0; i < this->COLUMN; i++)
	{
		cout<<left<<setw(8)<<col_names[i];
	}
	cout<<endl;
	
	for (int i = 0; i < this->ROW; i++)
	{	
		cout<<left<<setw(8)<<row_names[i]<<" ";
		for (int j = 0; j < this->COLUMN; j++)
		{		
			cout<<setw(8)<<this->array2D[i][j];
		}
		cout<<endl;
	}
}


// edit column names according to number of columns
template<typename Type>
void Table<Type>::editColumnNames()
{
	col_names = new string[COLUMN];

	for (int i = 0; i < COLUMN; i++)
	{
		col_names[i] = string(1, i+65);

		if ((i+65) == 90)
		{
			break;
		}	
	}
}


// edit row names according to number of rows
template<typename Type>
void Table<Type>::editRowNames()
{
	row_names = new string[ROW];

	for (int i = 0; i < ROW; i++)
	{
		row_names[i] = to_string(i);
	}
}


int main(){
  


    return 0;
}
