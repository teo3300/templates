#include "matrix.hpp"
#include <iostream>
#include <memory.h>
#include <math.h>

Matrix::Matrix() : Matrix(0,0) {};

Matrix::Matrix(unsigned int height, unsigned int width)
        : m_height(height), m_width(width), m_size(width*height) {
    
    // Allocate enough memory for matrix data
    this->dat = new MATRIX_T[m_size];
}

Matrix::Matrix(unsigned int height, unsigned int width, MATRIX_T src[]) : Matrix(height,width) {
    load(src);
}

Matrix::Matrix(const Matrix &src)
        : m_height(src.height()), m_width(src.width()), m_size(src.m_size){
    dat = new double[src.m_size];
    load(src.dat);
}

Matrix::~Matrix() {
    // free matrix memory
    delete [] this->dat;
}

inline void Matrix::load(const MATRIX_T* src) { memcpy(this->dat, src, m_size*sizeof(MATRIX_T)); }
inline void Matrix::dump(double* const dst) { mempcpy(dst, this->dat, m_size*sizeof(MATRIX_T)); }

double& Matrix::operator() (unsigned int i) {
    return this->dat[i];
}

const double& Matrix::operator() (unsigned int i) const {
    return this->dat[i];
}

double& Matrix::operator() (unsigned int i, unsigned int j) {
    return this->dat[coord(i,j)];
}

const double& Matrix::operator() (unsigned int i, unsigned int j) const {
    return this->dat[coord(i,j)];
}

Matrix Matrix::operator= (const Matrix& other) {
    if(this == &other) return *this;    // Ignore null assignments

    if(this->differ(other)){
        delete [] this->dat;
        this->dat = new double[other.m_size];
    }
    
    this->m_width = other.width();
    this->m_height = other.width();
    this->m_size = other.m_size;

    load(other.dat);

    return *this; // to concatenate assignments
}

Matrix Matrix::operator+ (const Matrix& other) {
    if(this->differ(other)) {
        throw std::invalid_argument("Error in matrix sum/sub: wrong size");
    }

    Matrix tmp(height(), width());

    for(unsigned int i=0; i<this->m_size; i++){
        tmp.dat[i] = this->dat[i] + other.dat[i];
    }

    return tmp;
}

Matrix Matrix::operator- (const Matrix& other) {
    return (*this) + (-1)*other;
}

Matrix Matrix::operator* (const Matrix& other) {
    if(this->notMult(other)){
        throw std::invalid_argument("Error in matrix multiplication: wrong size");
    }

    Matrix tmp(height(),other.width());
    double buffer;
    for(unsigned int i=0; i<height(); i++){
        for(unsigned int j=0; j<other.width(); j++){
            buffer = 0;
            for(unsigned int t=0; t<width(); t++){
                buffer += get(i,t)*other.get(t,j); 
            }
            tmp(i,j) = buffer;
        }
    }
    return tmp;
}

Matrix Matrix::operator* (double sca) {
    Matrix tmp(height(), width());
    for(unsigned int i=0; i<this->m_size; i++){
        tmp.dat[i] = this->dat[i]*sca;
    }

    return tmp;
}

Matrix Matrix::operator ! () {
    Matrix tmp(width(), height());
    for(unsigned int i=0; i<height(); i++){
        for(unsigned int j=0; j<width(); j++){
            tmp(j,i) = get(i,j);
        }
    }
    return tmp;
}


double Matrix::operator ~ () {
    if(this->width() != 1 ) {
        throw std::invalid_argument("Error: Norm is only defined on vectors");
    }
    double c=0;
    for(unsigned int i=0; i<height(); i++) {
        c += dat[i] * dat[i];
    }
    return sqrt(c);
}

std::ostream& operator<< (std::ostream &strm, const Matrix &mat) {
    std::string buffer = "";
    for(unsigned int i=0; i<mat.height(); i++){
        for(unsigned int j=0; j<mat.width(); j++){
            buffer += std::to_string(mat(i, j)) + " ";
        }
        buffer += i==mat.height()-1 ? "" : "\n";
    }
    return strm << buffer;
}