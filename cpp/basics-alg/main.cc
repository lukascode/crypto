/* Copyright (C) 2017 Łukasz Sakowicz
*  Bezpieczeństwo sieci komputerowych
*  PB WI SemVI Grupa: PS4 
*  Implementacja podstawowych modułów kryptograficznych
* (Rail Fence, Przestawienia macierzowe, Szyfrowanie Cezara, Tablica Vigenere’a)
*	 kompilacja: g++ main.cc -std=c++11
*	 kompilator: gcc 5.3.0
*/

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>

static void clear();
static void pause();

class Cipher {
protected:
	char** prepareMatrix(int rows, int cols) {
		char** matrix = new char*[rows]; 
		for(int i=0; i<rows; ++i) matrix[i] = new char[cols];
			for(int i=0; i<rows; ++i) {
				for(int j=0; j<cols; ++j) {
					matrix[i][j] = '\0';
				}
			} return matrix;
	}

	void printMatrix(char** matrix, int rows, int cols) {
		std::cout<<"---matrix---\n";
		for(int i=0; i<rows; ++i) {
			for(int j=0; j<cols; ++j) {
				std::cout<<matrix[i][j]<<" ";
			} std::cout<<std::endl;
		}
	}

	void deleteMatrix(char** matrix, int rows) {
		for(int i=0; i<rows; ++i) {
			delete [] matrix[i];
		} delete [] matrix;
	}

	std::vector<int> getPermutation(std::string key) {
		struct key_el { char ch; int idx; };
		std::vector<key_el> key_elements;
		for(int i=0; i<key.length(); ++i) { //przypisz elementom klucza ich indeksy
			key_el el = { key[i], i};
			key_elements.push_back(el);
		}
		//posortuj wzg ch (ascii)
		std::sort(key_elements.begin(), key_elements.end(), [](const key_el& el1, const key_el& el2) {
			return el1.ch < el2.ch;
		});
		std::vector<int> result;
		for(int i=0; i<key_elements.size(); ++i) { //odczytaj permutacje
			result.push_back(key_elements[i].idx);
		} return result;
	}
};

class RailFence : public Cipher {

public:
	 std::string encrypt(std::string pText, int key) {
	 	if(key <= 0) throw "key should be greater than zero";
	 	else if(key == 1) return pText;
		char** matrix = prepareMatrix(key, pText.length()); //stworz macierz o key ilosci wierszy
		bool dir_bottom = false;
		int row=0, col=0;
		for(int i=0; i<pText.length(); ++i) { //wypelnij macierz tekstem idac w ksztalcie zigzag
			matrix[row][col++] = pText[i];
			if(row == 0 || row == key-1) dir_bottom = !dir_bottom; 
			if(dir_bottom) ++row; else --row;
		}
		std::string cText = "";
		for(int i=0; i<key; ++i) { //zaszyfruj tekst odczytujac wierszami
			for(int j=0; j<pText.length(); ++j) {
				if(matrix[i][j] != '\0') {
					cText += matrix[i][j];
				}
			}
		} 
		deleteMatrix(matrix, key);
		return cText;
	}

	std::string decrypt(std::string cText, int key) {
		if(key <= 0) throw "key should be greater than zero";
		else if(key == 1) return cText;
		char** matrix = prepareMatrix(key, cText.length());
		bool dir_bottom = false;
		int row=0, col=0;
		for(int i=0; i<cText.length(); ++i) { //wykresl sciezke zigzag w macierzy 
			matrix[row][col++] = '1';
			if(row == 0 || row == key-1) dir_bottom = !dir_bottom; 
			if(dir_bottom) ++row; else --row;
		}
		int idx = 0;
		for(int i=0; i<key; ++i) { //zapisz do macierzy zaszyfrowany tekst idac wierszami
			for(int j=0; j<cText.length(); ++j) {
				if(matrix[i][j] == '1') {
					if(idx < cText.length())
						matrix[i][j] = cText[idx++];
				}
			}
		}
		row = col = 0;
		dir_bottom = false;
		std::string pText = "";
		for(int i=0; i<cText.length(); ++i) { //odczytaj tekst jawny idac zigzag
			pText += matrix[row][col++];
			if(row == 0 || row == key-1) dir_bottom = !dir_bottom; 
			if(dir_bottom) ++row; else --row;
		}
		deleteMatrix(matrix, key);
		return pText;
	}

	void handle() {
		clear();
		std::cout<<"+------------+\n";
		std::cout<<"| RAIL FENCE |\n";
		std::cout<<"+------------+\n\n";

		std::cout<<"1. Szyfrowanie\n";
		std::cout<<"2. Deszyfrowanie\n";
		std::cout<<"wybor: ";
		int n;
		std::cin>>n;
		if(n == 1) {
			int key; 
			std::string plaintext, enctext, dectext; 
			try {
				std::cout<<"Podaj tekst do zaszyfrowania: "; 
				std::cin.clear();
				std::cin.ignore();
				std::getline(std::cin, plaintext);
				std::cout<<"(key > 0) Podaj klucz: "; std::cin>>key;
				if(std::cin.fail()) throw "key have to be numeric value";	
				enctext = this->encrypt(plaintext, key);
				dectext = this->decrypt(enctext, key);
				std::cout<<"Zaszyfrowano jako: "<<enctext<<"\n";
				std::cout<<"Odszyfrowano jako: "<<dectext<<"\n";
				if(dectext == plaintext) {
					std::cout<<"DK(EK(x)) == x | match\n";
				} else std::cout<<"DK(EK(x)) != x | not match\n";
			} catch(const char* m) { std::cout<<std::string(m)<<"\n"; }
			pause();
		} else {
			int key; 
			std::string enctext, dectext; 
			try {
				std::cout<<"Podaj tekst do odszyfrowania: "; 
				std::cin.clear();
				std::cin.ignore();
				std::getline(std::cin, enctext);
				std::cout<<"(key > 0) Podaj klucz: "; std::cin>>key;
				if(std::cin.fail()) throw "key have to be numeric value";
				dectext = this->decrypt(enctext, key);
				std::cout<<"Odszyfrowano jako: "<<dectext<<"\n";
			} catch(const char* m) { std::cout<<std::string(m)<<"\n"; }
			pause();
		}
	}	

};


class MatrixCipher2A : public Cipher {
public:
	std::string encrypt(std::string text) {
		if(text.length() == 0) return text;
		int key[] = { 3, 4, 1, 5, 2};
		int cols = sizeof(key)/sizeof(int), rows = ceil(text.length()/(double)cols); //wyznacz rozmiar macierzy
		char** matrix = prepareMatrix(rows, cols);
		for(int i=0,k=0; i<rows; ++i) { //zapisz tekst jawny w macierzy idac wierszami
			for(int j=0; (j<cols && k<text.length()); ++j, ++k) {
				matrix[i][j] = text[k];
			}
		}
		//encrypt
		std::string enctext = "";
		int unfilled;
		if(rows == 1) unfilled = cols - text.length();
		else unfilled = (rows*cols)%text.length();
		for(int i=0,k=0; i<rows; ++i) { //zaszyfruj idac wierszami, wybierajac odpowiednie elementy na podstawie permutacji key
			for(int j=0; (j<cols && k<text.length()); ++j,++k) {
				if(i == rows-1) {
					if( (key[j]-1) > (cols-unfilled-1) ) //zabezpiecz w przypadku niepelnego bloku
						{ --k; continue; }
				} enctext += matrix[i][key[j]-1];
			}
		}
		// printMatrix(matrix, rows, cols);
		deleteMatrix(matrix, rows);
		return enctext;
	}

	std::string decrypt(std::string text) {
		if(text.length() == 0) return text;
		int key[] = { 3, 4, 1, 5, 2};
		int cols = sizeof(key)/sizeof(int), rows = ceil(text.length()/(double)cols); //wyznacz rozmiar macierzy
		char** matrix = prepareMatrix(rows, cols);
		int unfilled;
		if(rows == 1) unfilled = cols - text.length();
		else unfilled = (rows*cols)%text.length();
		for(int i=0,k=0; i<rows; ++i) { //wstawiaj w macierz wartosci na podstawie permutacji key
			for(int j=0; (j<cols && k<text.length()); ++j,++k) {
				if(i == rows-1) { //zabezpiecz w przypadku niepelnego bloku
					if( (key[j]-1) > (cols-unfilled-1) )
						{ --k; continue; }
				} matrix[i][key[j]-1] = text[k];
			}
		} std::string plainText = "";
		for(int i=0,k=0; i<rows; ++i) { //odczytaj tekst jawny
			for(int j=0; (j<cols && k<text.length()); ++j,++k) {
				plainText += matrix[i][j];
			}
		} 
		// printMatrix(matrix, rows, cols);
		deleteMatrix(matrix, rows);
		return plainText;
	}

	void handle() {
		clear();
		std::cout<<"+-------------------------------+\n";
		std::cout<<"| Przestawienia macierzowe (2a) |\n";
		std::cout<<"|    key = [ 3, 4, 1, 5, 2 ]    |\n";
		std::cout<<"+-------------------------------+\n\n";

		std::cout<<"1. Szyfrowanie\n";
		std::cout<<"2. Deszyfrowanie\n";
		std::cout<<"wybor: ";
		int n;
		std::cin>>n;

		if(n == 1) {
			std::string plaintext, enctext, dectext;
			std::cout<<"Podaj tekst do zaszyfrowania: "; 
			std::cin.clear();
			std::cin.ignore();
			std::getline(std::cin, plaintext);
			enctext = this->encrypt(plaintext);
			dectext = this->decrypt(enctext);
			std::cout<<"Zaszyfrowano jako: "<<enctext<<"\n";
			std::cout<<"Odszyfrowano jako: "<<dectext<<"\n";
			if(dectext == plaintext) {
				std::cout<<"DK(EK(x)) == x | match\n";
			} else std::cout<<"DK(EK(x)) != x | not match\n";
			pause();
		} else {
			std::string enctext, dectext;
			std::cout<<"Podaj tekst do odszyfrowania: ";
			std::cin.clear();
			std::cin.ignore();
			std::getline(std::cin, enctext);
			dectext = this->decrypt(enctext);
			std::cout<<"Odszyfrowano jako: "<<dectext<<"\n";
			pause();
		}
		
	}
};

class Transposition2B : public Cipher {
public:
	std::string encrypt(std::string text, std::string key) {
		if(key.length() == 0 || text.length() == 0) return text;
		int cols = key.length(), rows = ceil(text.length()/(double)cols);
		char** matrix = prepareMatrix(rows, cols);
		for(int i=0,k=0; i<rows; ++i) { //zapisz tekst jawny w macierzy
			for(int j=0; (j<cols && k<text.length()); ++j,++k) {
				matrix[i][j] = text[k];
			}
		}
		int unfilled;
		if(rows == 1) unfilled = cols - text.length();
		else unfilled = (rows*cols)%text.length();
		std::string enctext = "";
		std::vector<int> permutation = getPermutation(key); //permutacja definiujaca kolejnosc kolumn
		for(int i=0,k=0; i<permutation.size(); ++i) { //zaszyfruj korzystajac z permutacji
			for(int j=0; (j<rows && k<text.length()); ++j,++k) {
				if(j == rows-1) {
					if( permutation[i] > (cols-unfilled-1) )
						{ --k; continue; }
				} enctext += matrix[j][permutation[i]];
			}
		} 
		deleteMatrix(matrix, rows);
		return enctext;
	}

	std::string decrypt(std::string text, std::string key) {
		if(key.length() == 0 || text.length() == 0) return text;
		int cols = key.length(), rows = ceil(text.length()/(double)cols);
		char** matrix = prepareMatrix(rows, cols);
		int unfilled;
		if(rows == 1) unfilled = cols - text.length();
		else unfilled = (rows*cols)%text.length();
		std::vector<int> permutation = getPermutation(key); 
		for(int i=0,k=0; i<permutation.size(); ++i) { //odszyfruj i zapisz do macierzy na podstawie permutacji
			for(int j=0; (j<rows && k<text.length()); ++j,++k) {
				if(j == rows-1) {
					if( permutation[i] > (cols-unfilled-1) )
						{ --k; continue; }
				} matrix[j][permutation[i]] = text[k];
			}
		}
		std::string dectext = "";
		for(int i=0,k=0; i<rows; ++i) { //odczytaj tekst jawny
			for(int j=0; (j<cols && k<text.length()); ++j,++k) {
				dectext += matrix[i][j];
			}
		}
		deleteMatrix(matrix, rows);
		return dectext;
	}

	void handle() {
		clear();
		std::cout<<"+-------------------------------+\n";
		std::cout<<"| Przestawienia macierzowe (2b) |\n";
		std::cout<<"+-------------------------------+\n\n";

		std::cout<<"1. Szyfrowanie\n";
		std::cout<<"2. Deszyfrowanie\n";
		std::cout<<"wybor: ";
		int n;
		std::cin>>n;
		if(n == 1) {
			std::string plaintext, enctext, dectext, key;
			std::cout<<"Podaj tekst do zaszyfrowania: "; 
			std::cin.clear();
			std::cin.ignore();
			std::getline(std::cin, plaintext);
			std::cout<<"Podaj klucz: ";
			std::getline(std::cin, key); 
			enctext = this->encrypt(plaintext, key);
			dectext = this->decrypt(enctext, key);
			std::cout<<"Zaszyfrowano jako: "<<enctext<<"\n";
			std::cout<<"Odszyfrowano jako: "<<dectext<<"\n";
			if(dectext == plaintext) {
				std::cout<<"DK(EK(x)) == x | match\n";
			} else std::cout<<"DK(EK(x)) != x | not match\n";
			pause();
		} else {
			std::string enctext, dectext, key;
			std::cout<<"Podaj tekst do odszyfrowania: ";
			std::cin.clear();
			std::cin.ignore();
			std::getline(std::cin, enctext);
			std::cout<<"Podaj klucz: ";
			std::getline(std::cin, key);
			dectext = this->decrypt(enctext, key);
			std::cout<<"Odszyfrowano jako: "<<dectext<<"\n";
			pause();
		}
		
	}
};

class Transposition2C : public Cipher {
public:
	std::string encrypt(std::string text, std::string key) {
		if(key.length() == 0 || text.length() == 0) return text;
		int cols = key.length(), rows = getRowsNumber(text.length(), cols);
		char** matrix = prepareMatrix(rows, cols);
		std::vector<int> permutation = getPermutation(key);
		for(int i=0,k=0; i<rows; ++i) { //zapisz w macierzy znaki wg wzoru do kolumn z numerami 1,2,3,4...
			for(int j=0; (j<=permutation[i%cols] && k < text.length()); ++j,++k) {
				matrix[i][j] = text[k];
			}
		}
		std::string enctext = "";
		for(int i=0; i<cols; ++i) { //zaszyfruj wedlug permutacji
			for(int j=0; j<rows; ++j) {
				if(matrix[j][permutation[i]] != '\0') 
				{ enctext += matrix[j][permutation[i]]; }
			}
		}
		deleteMatrix(matrix, rows);
		return enctext;
	}

	std::string decrypt(std::string text, std::string key) {
		if(key.length() == 0 || text.length() == 0) return text;
		int cols = key.length(), rows = getRowsNumber(text.length(), cols);
		char** matrix = prepareMatrix(rows, cols);
		std::vector<int> permutation = getPermutation(key);
		for(int i=0,k=0; i<rows; ++i) { //zaznacz w macierzy miejsca pojawiania sie znakow
			for(int j=0; (j<=permutation[i%cols] && k < text.length()); ++j,++k) {
				matrix[i][j] = '1';
			}
		}
		for(int i=0,k=0; i<cols; ++i) { //zapisz zaszyfrowany tekst kolumnami do macierzy
			for(int j=0; (j<rows && k<text.length()); ++j,++k) {
				if(matrix[j][permutation[i]] == '1') {
					matrix[j][permutation[i]] = text[k];
				} else --k;
			}
		}		
		std::string plainText = "";
		for(int i=0,k=0; i<rows; ++i) { //odczytaj tekst jawny
			for(int j=0; (j<=permutation[i%cols] && k < text.length()); ++j,++k) {
				plainText += matrix[i][j];
			}
		} 
		deleteMatrix(matrix, rows);
		return plainText;
	}

	void handle() {
		clear();
		std::cout<<"+-------------------------------+\n";
		std::cout<<"| Przestawienia macierzowe (2c) |\n";
		std::cout<<"+-------------------------------+\n\n";

		std::cout<<"1. Szyfrowanie\n";
		std::cout<<"2. Deszyfrowanie\n";
		std::cout<<"wybor: ";
		int n;
		std::cin>>n;

		if(n == 1) {
			std::string plaintext, enctext, dectext, key;
			std::cout<<"Podaj tekst do zaszyfrowania: "; 
			std::cin.clear();
			std::cin.ignore();
			std::getline(std::cin, plaintext);
			std::cout<<"Podaj klucz: ";
			std::getline(std::cin, key); 
			enctext = this->encrypt(plaintext, key);
			dectext = this->decrypt(enctext, key);
			std::cout<<"Zaszyfrowano jako: "<<enctext<<"\n";
			std::cout<<"Odszyfrowano jako: "<<dectext<<"\n";
			if(dectext == plaintext) {
				std::cout<<"DK(EK(x)) == x | match\n";
			} else std::cout<<"DK(EK(x)) != x | not match\n";
			pause();
		} else {
			std::string enctext, dectext, key;
			std::cout<<"Podaj tekst do odszyfrowania: ";
			std::cin.clear();
			std::cin.ignore();
			std::getline(std::cin, enctext);
			std::cout<<"Podaj klucz: ";
			std::getline(std::cin, key);
			dectext = this->decrypt(enctext, key);
			std::cout<<"Odszyfrowano jako: "<<dectext<<"\n";
			pause();
		}
		
	}

private:
	int getRowsNumber(int textLen, int colsNumber) {
		int n = ceil( (sqrt(8*textLen+1) - 1) / 2.0); //ile wierszy potrzeba aby zapisac textLen znakow
		if(n > colsNumber) {
			//ile znakow mozna zapisac za pomoca colsNumber ilosci wierszy
			int letters = floor( (colsNumber*(colsNumber+1))/ 2.0 );
			n = colsNumber;
			int rest_letters = textLen - letters; //ile znakow pozostalo do zapisania

			int rows_tmp;
			while( (rows_tmp = ceil((sqrt(8*rest_letters+1)-1)/2.0)) > colsNumber) {
				n += colsNumber;
				rest_letters -= letters;
			}
			n += rows_tmp;
			return n;
		} else return n;
	}

};

class Caesar3B : public Cipher {
public:
	Caesar3B() {
		k0 = 11;
		k1 = 5;
		n = 26;
		fi = 12;
	}
	std::string encrypt(std::string text) {
		if(text.length() == 0) return text;
		std::transform(text.begin(), text.end(), text.begin(), ::toupper);
		std::string enctext = "";
		for(int i=0; i<text.length(); ++i) {
			if(text[i] == ' ') enctext += text[i];
			else {
				char c = text[i];
				c -= 65;
				c = (c*k1+k0)%n;
				c += 65;
				enctext += c;
			}
		} return enctext;
	}

	std::string decrypt(std::string text) {
		if(text.length() == 0) return text;
		std::transform(text.begin(), text.end(), text.begin(), ::toupper);
		std::string plainText = "";
		for(int i=0; i<text.length(); ++i) {
			if(text[i] == ' ') plainText += ' ';
			else {
				char c = text[i];
				c -= 65;
				c = ( modulo(c+(n-k0), n)*modulo_pow(k1, fi-1.0, n) ) % n;
				c += 65;
				plainText += c;
			}
		} return plainText;
	}
	void handle() {
		clear();
		std::cout<<"+-------------------------------+\n";
		std::cout<<"| Szyfrowanie Cezara (3b)       |\n";
		std::cout<<"+-------------------------------+\n\n";

		std::cout<<"1. Szyfrowanie\n";
		std::cout<<"2. Deszyfrowanie\n";
		std::cout<<"wybor: ";
		int n;
		std::cin>>n;

		if(n == 1) {
			std::string plaintext, enctext, dectext;
			std::cout<<"Podaj tekst do zaszyfrowania: "; 
			std::cin.clear();
			std::cin.ignore();
			std::getline(std::cin, plaintext);
			std::transform(plaintext.begin(), plaintext.end(), plaintext.begin(), ::toupper);
			std::cout<<"Podaj k0: \n"; std::cin>>k0;
			std::cout<<"Podaj k1: \n"; std::cin>>k1;
			enctext = this->encrypt(plaintext);
			dectext = this->decrypt(enctext);
			std::cout<<"Zaszyfrowano jako: "<<enctext<<"\n";
			std::cout<<"Odszyfrowano jako: "<<dectext<<"\n";
			if(dectext == plaintext) {
				std::cout<<"DK(EK(x)) == x | match\n";
			} else std::cout<<"DK(EK(x)) != x | not match\n";
			pause();
		} else {
			std::string enctext, dectext;
			std::cout<<"Podaj tekst do odszyfrowania: ";
			std::cin.clear();
			std::cin.ignore();
			std::getline(std::cin, enctext);
			std::transform(enctext.begin(), enctext.end(), enctext.begin(), ::toupper);
			std::cout<<"Podaj k0: \n"; std::cin>>k0;
			std::cout<<"Podaj k1: \n"; std::cin>>k1;
			dectext = this->decrypt(enctext);
			std::cout<<"Odszyfrowano jako: "<<dectext<<"\n";
			pause();
		}
		
	}
private:
	int k0,k1,n,fi;

	long modulo_pow(int a, int exp, int mod) {
		if(exp == 0) return 1%mod;
		else if(exp == 1) return a%mod;
		else if(exp%2==0) 
			return (modulo_pow(a, exp/2, mod)*(modulo_pow(a, exp/2, mod)))%mod;
		else return (modulo_pow(a, exp-1, mod)*(a%mod))%mod;
	}

	// A^2modC = (A*A)modC = (AmodC*AmodC)modC
	long modulo_mult(long a, long b, int mod) {
		return ((a%mod)*(a%mod))%mod;
	}

	int modulo(int v, int mod) {
		if(mod < 0) 
			return modulo(v, -mod);
		int result = v%mod;
		if(result < 0) {
			result += mod;
		} return result;
	}
};

class Vigener4 : public Cipher {
public:
	std::string encrypt(std::string text, std::string key) {
		if(text.length() == 0 || key.length() == 0) return text;
		std::transform(text.begin(), text.end(), text.begin(), ::toupper);
		std::transform(key.begin(), key.end(), key.begin(), ::toupper);
		if(!hasOnlyLetters(text) || !hasOnlyLetters(key)) 
			throw "Only letters required";
		int rows = 26, cols = 26;
		char** matrix = prepareMatrix(rows, cols);
		fillCeaser(matrix, rows, cols);
		
		if(text.length() > key.length()) {
			std::string str = key; int k=0;
			while(text.length() != key.length()) {
				key += str[(k++)%str.length()];
			}
		}
		std::string enctext = "";
		for(int i=0; i<text.length(); ++i) { //szyfruj
			char a = text[i], k = key[i];
			enctext += matrix[k-65][a-65];
		}
		deleteMatrix(matrix,rows);
		return enctext;
	}

	std::string decrypt(std::string text, std::string key) {
		if(text.length() == 0 || key.length() == 0) return text;
		std::transform(text.begin(), text.end(), text.begin(), ::toupper);
		std::transform(key.begin(), key.end(), key.begin(), ::toupper);
		if(!hasOnlyLetters(text) || !hasOnlyLetters(key)) 
			throw "Only letters required";
		int rows = 26, cols = 26;
		char** matrix = prepareMatrix(rows, cols);
		fillCeaser(matrix, rows, cols);

		if(text.length() > key.length()) {
			std::string str = key; int k=0;
			while(text.length() != key.length()) {
				key += str[(k++)%str.length()];
			}
		}
		std::string plainText = "";
		for(int i=0; i<text.length(); ++i) { //odszyfruj
			int idx;
			for(int j=0; j<cols; ++j) {
				if(matrix[key[i]-65][j] == text[i])
					{ idx = j; break; }
			} plainText += idx+65;
		}
		deleteMatrix(matrix, rows);
		return plainText;
	}

	void handle() {
		clear();
		std::cout<<"+-------------------------------+\n";
		std::cout<<"| Szyfrowanie Vigenerea (4)     |\n";
		std::cout<<"+-------------------------------+\n\n";

		std::cout<<"1. Szyfrowanie\n";
		std::cout<<"2. Deszyfrowanie\n";
		std::cout<<"wybor: ";
		int n;
		std::cin>>n;

		if(n == 1) {
			std::string plaintext, enctext, dectext, key;
			try {
				std::cout<<"Podaj tekst do zaszyfrowania: "; 
				std::cin.clear();
				std::cin.ignore();
				std::getline(std::cin, plaintext);
				std::transform(plaintext.begin(), plaintext.end(), plaintext.begin(), ::toupper);
				std::cout<<"Podaj klucz: ";
				std::getline(std::cin, key); 
				enctext = this->encrypt(plaintext, key);
				dectext = this->decrypt(enctext, key);
				std::cout<<"Zaszyfrowano jako: "<<enctext<<"\n";
				std::cout<<"Odszyfrowano jako: "<<dectext<<"\n";
				if(dectext == plaintext) {
					std::cout<<"DK(EK(x)) == x | match\n";
				} else std::cout<<"DK(EK(x)) != x | not match\n";
			} catch(const char* m) { std::cout<<std::string(m)<<"\n"; }
			pause();
		} else {
			std::string enctext, dectext, key;
			std::cout<<"Podaj tekst do odszyfrowania: ";
			std::cin.clear();
			std::cin.ignore();
			std::getline(std::cin, enctext);
			std::cout<<"Podaj klucz: ";
			std::getline(std::cin, key);
			dectext = this->decrypt(enctext, key);
			std::cout<<"Odszyfrowano jako: "<<dectext<<"\n";
			pause();
		}
		
	}
private:
	std::string alf = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	bool hasOnlyLetters(std::string text) {
		for(int i=0; i<text.length(); ++i) {
			if((text[i] < 'A') || (text[i] > 'Z'))
				return false;
		} return true;
	}
	void fillCeaser(char** matrix, int rows, int cols) {
		for(int i=0; i<rows; ++i) {
			for(int j=0; j<cols; ++j) {
				matrix[i][j] = (((alf[j]-65)+i)%26)+65;
			}
		}
	}
};

void clear() {
	std::system("cls||clear");
}

void pause() {
	#ifdef __linux__
		// std::cin.clear();
		std::cin.ignore();
		std::cin.get();
	#elif _WIN32
		std::system("pause");
	#else
	#endif
}

void show_menu() {
	std::cout<<"+--------------------------------------+\n";
	std::cout<<"|PODSTAWOWE MODOLY KRYPTOGRAFICZNE     |\n";
	std::cout<<"|by lukascode 			               |\n";
	std::cout<<"+--------------------------------------+\n";
	std::cout<<"| 1. Rail Fence (1)                    |\n";
	std::cout<<"| 2. Przestawienia macierzowe (2a)     |\n";
	std::cout<<"| 3. Przestawienia macierzowe (2b)     |\n";
	std::cout<<"| 4. Przestawienia macierzowe (2c)     |\n";
	std::cout<<"| 5. Szyfrowanie Cezara (3b)           |\n";
	std::cout<<"| 6. Szyfrowanie Vigenerea (4)         |\n";
	std::cout<<"| 7. Zamknij                           |\n";
	std::cout<<"+--------------------------------------+\n";
}

int main(int argc, char* argv[]) {

	srand(time(NULL));

	RailFence railFence;
	MatrixCipher2A matrixCipher2A;
	Transposition2B transp2B;
	Transposition2C transp2C;
	Caesar3B caesar3B;
	Vigener4 vigener4;
	
	char c = '\0';
	do {
		clear();
		show_menu();
		std::cout<<"wybor: ";
		std::cin.clear();
		std::cin>>c;
		switch(c) {
			case '1': railFence.handle(); break;
			case '2': matrixCipher2A.handle(); break;
			case '3': transp2B.handle(); break;
			case '4': transp2C.handle(); break;
			case '5': caesar3B.handle(); break;
			case '6': vigener4.handle(); break;
		}
	} while(c != '7');
	

	return 0;
}