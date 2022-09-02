#pragma once

#include <ostream>

// Using definition insead of templates since decision is proj dependand and at compile time
#ifndef MATRIX_T
    #define MATRIX_T double
#endif

class Matrix {

    MATRIX_T        *dat        = nullptr;
    unsigned int    m_height,
                    m_width,
                    m_size;

    // Compute coordinate
    inline unsigned int coord(unsigned int i, unsigned int j) const {return j+i*m_width; }

    // Used to detect incompatibilities in matrix operations
    inline int notMult(const Matrix& other) const { return other.m_height - this->m_width; }
    inline int diffWidth(const Matrix& other) const { return other.m_width - this->m_width; }
    inline int diffHeight(const Matrix& other) const { return other.m_height - this->m_height; }
    inline int differ(const Matrix& other) const { return diffHeight(other) | diffWidth(other); }

public:

    /* Constructors */
    // Generic constructor
    Matrix();
    // Size-specific constructor
    Matrix(unsigned int height, unsigned int width);
    // Size-specific and fill
    Matrix(unsigned int height, unsigned int width, MATRIX_T src[]);
    // Copy constructor
    Matrix(const Matrix& src);
    // Destructor
    ~Matrix();

    /* Getters */
    // Get dimensions
    const inline unsigned int width() const { return this->m_width; }
    const inline unsigned int height() const {return this->m_height; }
    // Compute coordinates and return content
    inline MATRIX_T get(unsigned int i, unsigned int j) const { return dat[coord(i,j)]; }

    /* Setters */
    // Set single values
    inline void set(unsigned int i, unsigned int j, MATRIX_T val) { dat[coord(i,j)] = val; }
    void load(const MATRIX_T* src);
    void dump(double* const dst);

    /* Access operators */
    // wrinting as a vector (expose memory)
    double& operator() (unsigned int i);
    // Reading as a vector (expose memory read only)
    const double& operator() (unsigned int i) const;
    // wrinting as a matrix (expose memory)
    double& operator() (unsigned int i, unsigned int j);
    // Reading as a vector (expose memory read only)
    const double& operator() (unsigned int i, unsigned int j) const;
    inline double sca() const { return (*this)(0,0); }

    /* Aritmethic operators */
    Matrix operator= (const Matrix& other);
    Matrix operator+ (const Matrix& other);
    Matrix operator- (const Matrix& other);
    Matrix operator* (const Matrix& other);
    Matrix operator* (double sca);
    inline Matrix operator/ (double sca) { return (*this) * (1/sca); }

    Matrix operator! ();
    double operator~ ();

};

inline Matrix operator*(double sca, Matrix mat) {
    return mat * sca;
};

std::ostream& operator<<(std::ostream &strm, const Matrix& mat);