//
// Puzzle.cpp - Puzzle class implementation
//
// Written by Xiaotian Zhu
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <time.h>
#include "Puzzle.h"
#include <limits>
#include <complex>

// default & init constructor
Puzzle::Puzzle(int nrow, int ncol){
	Nrow = nrow;
	Ncol = ncol;

	Entries = new int*[Nrow];
	for(int i = 0; i < Nrow; i++){
		Entries[i] = new int[Ncol];
	}

	for(int i = 0; i< Nrow; i++){
		for(int j = 0; j < Ncol; j++){
			Entries[i][j] = i*Ncol + j + 1;
		}
	}

	Entries[Nrow-1][Ncol-1] = 0;

	Vblank = Nrow - 1;
	Hblank = Ncol - 1;

}

// init constructor version 2
Puzzle::Puzzle(int nrow, int ncol, int* entries){
	Nrow = nrow;
	Ncol = ncol;
	Entries = new int*[Nrow];
	for(int i = 0; i < Nrow; i++){
		Entries[i] = new int[Ncol];
	}

	int index = 0;
	for(int i = 0; i< Nrow; i++){
		for(int j = 0; j < Ncol; j++){
			Entries[i][j] = entries[index];
			if (Entries[i][j] == 0){
				Vblank = i;
				Hblank = j;
			}
			index++;
		}
	}

}

// copy constructor
Puzzle::Puzzle(const Puzzle& source){
	// std::cout << std::endl << "copy constructor called" << std::endl;
	Entries = NULL;
	// Calls the assignment operator
	*this = source;
}

// destructor
Puzzle::~Puzzle(){
	// std::cout << std::endl << "destructor called" << std::endl;
	for(int i = 0; i < Nrow; i++){
		delete[] Entries[i];
	}
	delete[] Entries;
	Entries = NULL;
}

// deep copy assignment operator
Puzzle& Puzzle::operator = (const Puzzle& source){
	// std::cout << std::endl << "deep copy assignment operator called" << std::endl;

	// first we have to dispose previously allocated memory
	if(Entries != NULL){
		for(int i = 0; i < Nrow; i++){
			delete[] Entries[i];
		}
		delete[] Entries;
	}

	// copy fields
	Nrow = source.GetNrow();
	Ncol = source.GetNcol();
	Vblank = source.GetVblank();
	Hblank = source.GetHblank();

	// allocate new memory
	Entries = new int*[Nrow];
	for(int i = 0; i < Nrow; i++){
		Entries[i] = new int[Ncol];
	}

	// deep copy
	for(int i = 0; i < Nrow; i++){
		for(int j = 0; j < Ncol; j++){
			Entries[i][j] = source.GetEntries()[i][j];
		}
	}

	return
		*this;

}

// Comparison operator: Puzzles are equal when their static fields are the same
// , and they dynamic fields store the same values
bool Puzzle::operator == (const Puzzle& compareTo) const{
	if(Nrow == compareTo.GetNrow() && Ncol == compareTo.GetNcol() && Vblank == compareTo.GetVblank() && Hblank == compareTo.GetHblank()){
		bool isEqual = true;
		for(int i = 0; (i < Nrow) && (isEqual == true); i++){
			for(int j = 0; (j < Ncol) && (isEqual == true); j++){
				isEqual = (Entries[i][j] == compareTo.GetEntries()[i][j]);
			}
		}
		return isEqual;
	}else{
		return false;
	}
}

// get Nrow
int Puzzle::GetNrow() const {
	return Nrow;
}

// get Ncol
int Puzzle::GetNcol() const {
	return Ncol;
}

// get Vblank
int Puzzle::GetVblank() const {
	return Vblank;
}

// get Hblank
int Puzzle::GetHblank() const {
	return Hblank;
}

// get Entries
int** Puzzle::GetEntries() const {
	return Entries;
}

// set Entries
void Puzzle::SetEntries(int* entries){
	int index = 0;
	for(int i = 0; i < Nrow; i++){
		for(int j = 0; j < Ncol; j++){
			Entries[i][j] = entries[index];
			if (Entries[i][j] == 0){
				Vblank = i;
				Hblank = j;
			}
			index++;
		}
	}
}

// randomly permute all entries
void Puzzle::RandomSet(){
	int *entries = new int[Nrow*Ncol+1];
	for(int i = 0; i < (Nrow*Ncol); i++){
		entries[i] = i + 1;
	}
	entries[Nrow*Ncol-1] = 0;
	entries[Nrow*Ncol] = 0;

	/* initialize random seed:*/
	srand(time(NULL));

	std::random_shuffle(entries, entries + Nrow*Ncol);
	int index = 0;
	for(int i = 0; i < Nrow; i++){
		for(int j = 0; j < Ncol; j++){
			Entries[i][j] = entries[index];
			if (Entries[i][j] == 0){
				Vblank = i;
				Hblank = j;
			}
			index++;
		}
	}

	if(!IsSolvable()){
		bool stop = false;
		int index_ = 0;
		int temp;
		while(!stop){
			if(entries[index_]!=0 && entries[index_+1]!=0 && index_<(Nrow*Ncol-1)){
				stop = true;

			}else{
				index_++;
			}
		}
		temp = entries[index_];
		entries[index_] = entries[index_+1];
		entries[index_+1] = temp;

		index = 0;
		for(int i = 0; i < Nrow; i++){
			for(int j = 0; j < Ncol; j++){
				Entries[i][j] = entries[index];
				if (Entries[i][j] == 0){
					Vblank = i;
					Hblank = j;
				}
				index++;
			}
		}
	}

	delete[] entries;
}

// let user set the entries
void Puzzle::ByUser(){
	std::cout << "Please enter the entries. Enter 0 for Empty Space." << std::endl;
	int* tempEntries = new int[Nrow*Ncol];
	for (int i = 0; i < Nrow*Ncol; i++){
		tempEntries[i] = -1;
	}
	int tempIndex = 0;
	bool somethingWrong;
	for(int i = 0; i < Nrow; i++){
		for(int j = 0; j < Ncol; j++){
			do{
				somethingWrong = false;

				for (;;) {
					std::cout << "[" << i << "][" << j << "]:  ";
					if (std::cin >> tempEntries[tempIndex]) {
						break;
					} else {
						std::cout << std::endl << "Please enter a valid integer within [" << 0 << ", " << Nrow*Ncol-1 << "]" << std::endl;
						std::cin.clear();
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					}
				}



				if(tempEntries[tempIndex] < 0 || tempEntries[tempIndex] >= Nrow*Ncol){
					std::cout << std::endl << "Out of bounds!" << std::endl;
					somethingWrong = true;
				}else{
					for(int k = 0; k < tempIndex; k++){
						if(tempEntries[tempIndex] == tempEntries[k]){
							std::cout << std::endl << "Same value entered before!" << std::endl;
							somethingWrong = true;
							break;
						}
					}
				}

			}while(somethingWrong);

			Entries[i][j] = tempEntries[tempIndex];

			tempIndex++;

			if (Entries[i][j] == 0){
				Vblank = i;
				Hblank = j;
			}

			std::cout << std::endl;
		}
	}
}

// displaying
void Puzzle::Display() const {
	int width = 4 + int(std::log10(Nrow * Ncol));
	std::cout << std::endl;
	for(int i = 0; i < Nrow; i++){
		for(int j = 0; j < Ncol; j++){
			if( (i == Vblank) && (j == Hblank) ){
				std::cout << std::setw(width) << " ";
			}else{
				std::cout << std::setw(width) << Entries[i][j];
			}
		}
		std::cout << std::endl;
	}

}

// swap blank space and a nearby entry
bool Puzzle::Swap(MoveDirect thisDirect){
	int temp;
	switch(thisDirect){
	case BLANK_UP:
		if(Vblank != 0){
			temp = Entries[Vblank-1][Hblank];
			Entries[Vblank-1][Hblank] = 0;
			Entries[Vblank][Hblank] = temp;
			Vblank -= 1;
		}else{
			std::cout << std::endl << "Invalid move!" << std::endl;
			return false;
		}
		break;
	case BLANK_DOWN:
		if(Vblank != (Nrow-1)){
			temp = Entries[Vblank+1][Hblank];
			Entries[Vblank+1][Hblank] = 0;
			Entries[Vblank][Hblank] = temp;
			Vblank += 1;
		}else{
			std::cout << std::endl << "Invalid move!" << std::endl;
			return false;
		}
		break;
	case BLANK_LEFT:
		if(Hblank != 0){
			temp = Entries[Vblank][Hblank-1];
			Entries[Vblank][Hblank-1] = 0;
			Entries[Vblank][Hblank] = temp;
			Hblank -= 1;
		}else{
			std::cout << std::endl << "Invalid move!" << std::endl;
			return false;
		}
		break;
	case BLANK_RIGHT:
		if(Hblank != (Ncol-1)){
			temp = Entries[Vblank][Hblank+1];
			Entries[Vblank][Hblank+1] = 0;
			Entries[Vblank][Hblank] = temp;
			Hblank += 1;
		}else{
			std::cout << std::endl << "Invalid move!" << std::endl;
			return false;
		}
		break;
	default:;
	}
	return true;
}

// swap blank space and a nearby entry, no message for invalid moves
bool Puzzle::Swap_NoMessage(MoveDirect thisDirect){
	int temp;
	switch(thisDirect){
	case BLANK_UP:
		if(Vblank != 0){
			temp = Entries[Vblank-1][Hblank];
			Entries[Vblank-1][Hblank] = 0;
			Entries[Vblank][Hblank] = temp;
			Vblank -= 1;
		}else{
			//std::cout << std::endl << "Invalid move!" << std::endl;
			return false;
		}
		break;
	case BLANK_DOWN:
		if(Vblank != (Nrow-1)){
			temp = Entries[Vblank+1][Hblank];
			Entries[Vblank+1][Hblank] = 0;
			Entries[Vblank][Hblank] = temp;
			Vblank += 1;
		}else{
			//std::cout << std::endl << "Invalid move!" << std::endl;
			return false;
		}
		break;
	case BLANK_LEFT:
		if(Hblank != 0){
			temp = Entries[Vblank][Hblank-1];
			Entries[Vblank][Hblank-1] = 0;
			Entries[Vblank][Hblank] = temp;
			Hblank -= 1;
		}else{
			//std::cout << std::endl << "Invalid move!" << std::endl;
			return false;
		}
		break;
	case BLANK_RIGHT:
		if(Hblank != (Ncol-1)){
			temp = Entries[Vblank][Hblank+1];
			Entries[Vblank][Hblank+1] = 0;
			Entries[Vblank][Hblank] = temp;
			Hblank += 1;
		}else{
			//std::cout << std::endl << "Invalid move!" << std::endl;
			return false;
		}
		break;
	default:;
	}
	return true;
}

// count number of inversions
int Puzzle::CountInversions(int irow, int icol) const{
	int inversions = 0;
	int currentIndex = irow * Ncol + icol;
	int lastIndex = Ncol*Nrow;
	int currentValue = Entries[irow][icol];
	int jrow, jcol;
	int compValue;
	for (int i=currentIndex+1; i<lastIndex; i++) {
		jrow = floor(i / Ncol);
		jcol = i % Ncol;
		compValue = Entries[jrow][jcol];
		if (currentValue > compValue && currentIndex != (lastIndex-1) && compValue != 0) {
			inversions++;
		}
	}
	return inversions;
}

// sum number of inversions
int Puzzle::SumInversions() const{
	int inversions = 0;
	for (int irow=0; irow < Nrow; irow++) {
		for (int icol=0; icol < Ncol; icol++) {
			inversions += CountInversions(irow,icol);
		}
	}
	return inversions;
}

// function to determine solvability
bool Puzzle::IsSolvable() const{
	if (Ncol % 2 == 1) {
		return (SumInversions() % 2 == 0);
	}
	else {
		return ((SumInversions() + Nrow - Vblank - 1) % 2 == 0);
	}
}

// check if Puzzle is same as default
// chengping please check if it works
bool Puzzle::IsDefault() const{
	Puzzle tempPuzzle(Nrow, Ncol);
	return tempPuzzle == *this;
}

// manhattan distance between *this and default
int Puzzle::Heuristic() const{
	int heuristic = 0;
	int i,j, i_, j_;
	int cellValue;
	for(i = 0; i < Nrow; i++){
		for(j = 0; j < Ncol; j++){
			cellValue = Entries[i][j];
			if(cellValue != 0){
				i_ = (cellValue-1) / Ncol;
				j_ = (cellValue-1) % Ncol;
				heuristic += std::abs(i-i_) + std::abs(j-j_);
			}
		}
	}
	return heuristic;
}

