// 10727119_吳恩賜  未完成需使用Github

# include<iostream>
# include<cstring>
# include<string>
# include<cstdlib>
# include<fstream>
# include<iomanip>
# include<vector>
# include<new>
# include<math.h>

using namespace std ;

# define Columns 6
# define Max_Len 10
# define Big_Int 255


typedef struct Data {
	
	char sid[Max_Len] ; // 學號 
	char sname[Max_Len] ; // 姓名 
	unsigned char score[Columns] ; // 分數 
	float mean ; // 平均 
	
} Data ;
    
vector<Data> ST ;
    

int TextBinary( string & File_Name ) {
		
	fstream inFile, outFile ;
	int stNo = 0 ;
		
	do {
		
	    cout << endl << "Input a file number:" ;
	    cin >> File_Name ;  // 讀入一數字
		    
	    if ( File_Name.compare("0") )
	        return false ;
	    inFile.open( ("input" + File_Name + ".txt").c_str(), fstream::in ) ;
		    
	    if ( inFile.is_open() )
	        break ;
		        
	} while ( true ) ;
	    
	 File_Name = "input" + File_Name + ".bin" ;
	outFile.open( File_Name.c_str(), fstream::out | fstream::binary ) ;
		
	if ( outFile.is_open() ) {
			
		char rBuf[Big_Int] ;
		while ( inFile.getline( rBuf, Big_Int, '\n' ) ) {
				
			string temp ;
			Data oneSt ;
				
			int cNo = 0, pre = 0, pos = 0 ;
			stNo++ ;
			temp.assign( rBuf ) ;	
			pos = temp.find_first_of( '\t', pre ) ;
				
			while ( pos != string::npos ) {
					
				switch ( ++cNo ) {
						
					case 1 : strcpy( oneSt.sid, temp.substr( pre, pos - pre ).c_str() ) ;
						    break ;
						
					case 2 : strcpy( oneSt.sname, temp.substr( pre, pos - pre ).c_str() ) ;
						    break ;
						    
					case 3 : oneSt.score[cNo-3] = atoi( temp.substr( pre, pos - pre ).c_str() ) ;
						    break ;
				}
				
			    pre = ++pos ;
			    pos = temp.find_first_of( '\t', pre ) ;
			}
				
			pos = temp.find_last_of( '\t' ) ;
		    oneSt.mean = atoi( temp.substr( pos+1 ).c_str() ) ;
		    outFile.write( (char * ) & oneSt, sizeof(oneSt) ) ;
		}
		outFile.close() ;
	}
	
	inFile.close() ;
	return stNo ;
}
	
void readBinary ( string & File_Name ) {
		
	fstream binFile ;
	Data oneSt ;
	int stNo = 0 ;
		
	binFile.open( File_Name.c_str(), fstream::in | fstream::binary ) ;
		
	if ( binFile.is_open() ) {
			
		binFile.seekg( 0, binFile.end ) ;
		stNo = binFile.tellg() / sizeof( oneSt ) ;
		binFile.seekg( 0, binFile.beg ) ;
			
		for ( int i = 0 ; i < stNo ; i++ ) {
				
			binFile.read( (char * ) & oneSt, sizeof(oneSt) ) ;
			ST.push_back(oneSt) ;
				
		}
	}
	
	binFile.close() ;
}

class QuadraticHash {
	
	struct HashData {
		
		int hvalue ;
		int sid ;
		string sname ;
		float mean ;
		
	} ;
	
	HashData * hash ;
	int hashTable ;
	float exist ;
	float dexist ;
	
public:
	
	QuadraticHash() { 
	
		exist = 0, dexist = 0 ;
		HashTable() ;
		hash = new HashData[hashTable] ;
		
	}
	
	int FindPrime( int range ) { // 找最小質數 

		bool isPrime = false ;
		int i = 0 ; 
		int num = range ;
		
		for ( ; !isPrime ; num++ ) {
			
			for( i = 2 ; i <= (int)sqrt(num) ; i++ ) {
				
				if ( num % i == 0 )
					break;
					
			}
			
			if ( i == (int)sqrt(num) + 1 )
				isPrime = true ;
		}
		
		num-- ;
		return num ;
	}
	
	void HashTable() { 
		
		int range = (int)( 1.2 * ST.size() ) + 1 ;
		hashTable = FindPrime( range ) ;
		
	}
	
	void Hash() {
		
		for ( int index = 0 ; index < ST.size() ; index++ ) {
			
			int x = 1 ;
		
		    for ( int i = 0 ; i < Max_Len && (int)ST.at(index).sid[i] != 0 ; i++ ) {
			
			    x = x * (int)ST.at(index).sid[i] % hashTable ;
			
		    }
		
		    if ( hash[x].sname != "" ) {
			
			    Impact( index, x ) ;
			
		    }
		
		    else {
			
			    hash[x].hvalue = x ;
			    hash[x].sid = atoi(ST.at(index).sid) ;
			    hash[x].sname = ST.at(index).sname ;
			    hash[x].mean = ST.at(index).mean ;
			    exist++ ;
			
		    }
			
		}


		for ( int i = 0; i < hashTable ; i++ ) {
			
			for ( int j = 0; j < hashTable / 2 ; j++ ) {
				
				dexist++ ;
				int y = ( i + (int) pow( j, 2 ) ) % hashTable ;
				
				if ( hash[y].sname == "" )
					break ;

			}
		} 
		
		dexist = dexist - hashTable;
		 
		cout << endl << "Hash table has been successfully created by Quadratic probing \n" ;	

		cout << endl << "unsuccessful search: " ;
		cout << setiosflags(ios::fixed) << setprecision(4) << dexist / hashTable << " comparisons on average" ;
		
		cout << endl << "successful search: " ;
		cout << setiosflags(ios::fixed) << setprecision(4) << exist / ST.size() ;
		cout << " comparisons on average" ;
		
	}
	
	void Impact ( int index, int location ) { // 碰撞 
		
		int temp = 1 ;
		
		for ( int i = 1; i <= hashTable / 2 ; i++ ) {
			
			temp++ ;
			
			int j = ( location + (int) pow( i, 2 ) )  % hashTable ;
			
			if ( hash[j].sname == "" ) {
				
				exist = exist + temp ;
				hash[j].hvalue = location ;
				hash[j].sid = atoi( ST.at(index).sid ) ;
				hash[j].sname = ST.at(index).sname ;
				hash[j].mean = ST.at(index).mean ;
				break ;
				
			}
		}
	}
	
	
	void OutQuadraticHash( string fileName ) {
		
		fstream outFile ;
		fileName = "quardratic" + fileName + ".txt" ;
		
		outFile.open( fileName.c_str(), fstream::out ) ;
		
		if ( outFile.is_open() ) {
			
			outFile << "  --- Hash table created by Quardratic hashing    ---\n" ;
			
			for ( int i = 0 ; i < hashTable ; i++ ) {
				
				outFile << "[" << setw(3) << i << "]" << '\t' ;
				
				if ( hash[i].sname != "" ) {
					
					outFile << hash[i].hvalue << "," << '\t' ;
					outFile << setw(8) << hash[i].sid << "," << '\t' ;
					outFile << hash[i].sname << "," << '\t' ;
					outFile << hash[i].mean ;
					
				}
				
				outFile << '\n';
			}
			
			outFile << "-----------------------------------------" ;
			outFile.close() ;
			
		}
	}
} ;

class DoubleHash {
	
	struct HashData {
		
		int hvalue ;
		int sid ;
		string sname ;
		float mean ;
		
	} ;
	
	HashData * dHash ;
	int hashTable ;
	int MaxStep ;
	float exist ;
	
public:
	
	DoubleHash() { 
	
		exist = 0 ;
		HashTable() ;
		dHash = new HashData[hashTable] ;
		SetMaxStep() ;
		
	}
	
	int FindPrime( int range ) {
		
		int i ;
		bool isPrime = false ;
		int num = range ;
		
		for ( ; ! isPrime ; num++ ) {
			
			for( i = 2 ; i <= (int)sqrt(num) ; i++ ) {
				
				if ( num % i == 0 )
					break;
			}
			
			if ( i == (int)sqrt(num) + 1 )
				isPrime = true ;
		}
		
		num-- ;
		return num ;
	}
	
	void HashTable() {
		
		int range = (int)(1.2 * ST.size() ) + 1;
		hashTable = FindPrime( range );
	}
	
	void SetMaxStep() {
		
		int range = ( ST.size() / 3 ) + 1; 
		MaxStep = FindPrime( range );
	}
	
	void Hash() {
		
		for ( int index = 0; index < ST.size(); index++ ) {
			
			int x = 1 ;
			
		    for ( int i = 0 ; i < Max_Len && (int)ST.at(index).sid[i] != 0; i++ )	
			    x = x * (int)ST.at(index).sid[i] % hashTable;

		
		    if ( dHash[x].sname != "" )
			    Impact ( index, x ) ;
			    
		    else {
		    	
			    dHash[x].hvalue = x ;
			    dHash[x].sid = atoi(ST.at(index).sid) ;
			    dHash[x].sname = ST.at(index).sname ;
			    dHash[x].mean = ST.at(index).mean ;
			    exist++ ;
			    
		    }
		}
	
		cout << endl << "Hash table has been successfully created by Double hashing" ;
		
		cout << endl << "successful search: " ;
		cout << setiosflags(ios::fixed) << setprecision(4) << exist / ST.size() ;
		cout << " comparisons on average" ;
	}
	
	void Impact ( int index, int location ) {
		
		int step = 1 ;
		
		for ( int i = 0; i < Max_Len && (int)ST.at(index).sid[i] != 0; i++ )
			step = step * (int)ST.at(index).sid[i] % MaxStep ;
		
		step = MaxStep - step ;
		int x = ( location + step ) % hashTable ;
		
		int tempexist = 1 ;
		while ( x != location ) {
			
			tempexist++ ;
			if ( dHash[x].sname == "" ) {
				
				exist = exist + tempexist ;
				dHash[x].hvalue = location ;
				dHash[x].sid = atoi(ST.at(index).sid) ;
				dHash[x].sname = ST.at(index).sname ;
				dHash[x].mean = ST.at(index).mean ;
				break ;
				
			}
			
			x = ( x + step ) % hashTable ;
			
		}
	}
	
	void OutDoubleHash( string fileName ) {
		
		fstream outFile ;
		fileName = "double" + fileName + ".txt" ;
		outFile.open( fileName.c_str(), fstream::out );
		
		if ( outFile.is_open() ) {
			
			outFile << "  --- Hash table created by Double hashing    ---\n" ;
			
			for ( int i = 0; i < hashTable; i++ ) {
				
				outFile << "[" << setw(3) << i << "]" << '\t' ;
				
				if ( dHash[i].sname != "" ) {
					
					outFile << dHash[i].hvalue << "," << '\t' ;
					outFile << setw(8) << dHash[i].sid << "," << '\t' ;
					outFile << dHash[i].sname << "," << '\t' ;
					outFile << dHash[i].mean ;
					
				}
				
				outFile << '\n' ;
			}
			
			outFile << "-----------------------------------------";
			outFile.close() ;
		}
	}
} ;

int main() {
	
	int command = -1 ;
	string File_Name ;
	
	while ( command != 0 ) {
		
		cout << endl << "****  Heap Construction  ****" ;
		cout << endl << "*  0. 離開                  *" ;
		cout << endl << "*  1. QuadraticHash         *" ;
		cout << endl << "*  2. DoubleHash            *" ;
		cout << endl << "*****************************" ;
		cout << endl << "Input a command(0, 1, 2): "    ;
		cin >> command ;
			
		if ( command == 0 )
			cout << endl << "Bye~Bye~" << endl ;
			
		else if ( command == 1 ) {
			
		    if ( TextBinary( File_Name ) > 0 ) {
			
		        readBinary( File_Name ) ;
		        QuadraticHash Hash ;
		        Hash.Hash() ;
		        Hash.OutQuadraticHash( File_Name ) ;
		    
			}
	
		}
		
		else if ( command == 2 ) {
			
			if ( TextBinary( File_Name ) > 0 ) {
				
			    readBinary( File_Name ) ;
			    DoubleHash doubleHash ;
			    doubleHash.Hash() ;
			    doubleHash.OutDoubleHash( File_Name ) ;
			    
            }
		}
	}
	
}
