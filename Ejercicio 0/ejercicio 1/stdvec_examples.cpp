// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include <vector>

int num_of_new_int = 0;
int num_of_delete = 0;
int num_of_const_d = 0;
int num_of_const_c = 0;
int num_of_const_m = 0;
int num_of_assig_c = 0;
int num_of_assig_m = 0;
int num_of_dest = 0;	// destroyer

void reset_stats() {
	num_of_new_int = 0;
	num_of_delete = 0;
	num_of_const_d = 0;
	num_of_const_c = 0;
	num_of_const_m = 0;
	num_of_assig_c = 0;
	num_of_assig_m = 0;
	num_of_dest = 0;
}

void print_stats() {
	std::cout << "--------------" << std::endl;
	std::cout << "new int: " << num_of_new_int << std::endl; // objeto creado (new)
	std::cout << "del int: " << num_of_delete << std::endl; // objeto eliminado
	std::cout << "const_default: " << num_of_const_d << std::endl; // constructora default
	std::cout << "const copy: " << num_of_const_c << std::endl; // objeto copiado
	std::cout << "const move: " << num_of_const_m << std::endl; // objeto movido
	std::cout << "assig copy: " << num_of_assig_c << std::endl; // copia asignada
	std::cout << "assig move: " << num_of_assig_m << std::endl; // movimiento asignado
	std::cout << "--------------" << std::endl;
}

class A {
public:
	int *p;

	// DEFAULT
	A(int n) {
		p = new int(n);
		num_of_new_int++;
		num_of_const_d++;
		std::cout << "Def. const. *p=" << *p << std::endl;
	}

	// COPY
	A(const A &o) {
		p = new int(*o.p);
		num_of_new_int++;
		num_of_const_c++;
		std::cout << "Copy. const. *p=" << *p << ", copied " << &o << " to "
				<< this << std::endl;
	}

	// MOVE
	A(A &&o) noexcept {
		p = o.p;
		o.p = nullptr;
		num_of_const_m++;
		std::cout << "Move. const. *p=" << *p << ", moved " << &o << " to "
				<< this << std::endl;
	}

	//  ----------------------------------------------------------------------------------------
	A& operator=(const A &o) {
		if (p != nullptr) { // we have something in p, we should release it first
			delete p;
			num_of_delete++;
		}

		if (o.p == nullptr)
			p = nullptr;
		else
			p = new int(*o.p);

		num_of_new_int++;
		num_of_assig_c++;
		std::cout << "Copy. assign. *p=" << *p << ", copied " << &o << " to "
				<< this << std::endl;
		return *this;
	}

	A& operator=(A &&o) noexcept {
		if (p != nullptr) { // we have something in p, we should release it first
			delete p;
			num_of_delete++;
		}
		p = o.p;
		o.p = nullptr;
		num_of_assig_m++;
		std::cout << "Move. assign. *p=" << *p << ", moved " << &o << " to "
				<< this << std::endl;
		return *this;
	}

	~A() {
		std::cout << "Dtor." << std::endl;
		num_of_dest++;
		if (p != nullptr) {
			num_of_delete++;
			delete p;
		}
	}

	operator int() const {
		return *p;
	}

};

template<typename T>
void print_vector(const std::vector<T> &v) {
	for (const T &a : v) {
		std::cout << a << " ";
	}
	std::cout << std::endl;
}

void addTen() {
	std::vector<A> v;
	for (int i = 0; i < 10; i++) {
		std::cout << "# adding " << i << " using emblace_back" << std::endl;
		if (v.capacity() == v.size()) {
			std::cout << "# The vector is about to be resized!" << std::endl;
		}
		v.emplace_back(i);
	}
}

void test0() {

	// crea un vector de tipo A
	std::vector<A> v;

	// rvalue -> objeto que no ocupa una posicion IDENTIFICABLE en memoria ----- x = 1 --> 1
	// lvalue -> objeto que ocupa una posicion IDENTIFICABLE en memoria -------- x = 1 --> x

	// A(1) es un rvalue, por lo que, primero se llama a la constructora default (como con todo objeto en principio),
	// y luego, al pasarlo al pushback para meterlo en el vector, llama a la constructora por movmimiento, MOVE,
	// llama a la constructora con doble & (rvalue reference, &&); esta constructora cambia la posicion del vector, no la
	// direccion a la que apunta (copia superficial)
	v.push_back(A(1));

	std::cout << "buibaya" << std::endl;
}

void test1() {

	// crea un vector de tipo A donde meter el objeto
	std::vector<A> v;

	// rvalue -> objeto que no ocupa una posicion IDENTIFICABLE en memoria ----- x = 1 --> 1
	// lvalue -> objeto que ocupa una posicion IDENTIFICABLE en memoria -------- x = 1 --> x

	// A x(1) es un lvalue, por lo que, primero se llama a la constructora default (como con todo objeto en principio),
	// y luego, al pasarlo al pushback para meterlo en el vector, llama a la constructora por copia, COPY,
	// llama a la constructora con un & (rvalue reference, &); esta constructora copia todos los valores de la clase,
	// asi como los new (copia profunda)
	A x(1);
	v.push_back(x);

	std::cout << "buibaya" << std::endl;
}

void test2() {
	std::vector<A> v;

	// rvalue -> objeto que no ocupa una posicion IDENTIFICABLE en memoria ----- x = 1 --> 1
	// lvalue -> objeto que ocupa una posicion IDENTIFICABLE en memoria -------- x = 1 --> x
	A x(1);

	// A x(1) es un lvalue, por lo que, primero se llama a la constructora default (como con todo objeto en principio),
	// y luego, al pasarlo al pushback para meterlo en el vector, deberia llamar a la constructora por copia pero, al usar
	// std::move(x), se trata a x como un rvalue en vez de un lvalue y llama a la constructoria por movimiento, MOVE
	// llama a la constructora con doble & (rvalue reference, &&); esta constructora cambia la posicion del vector, no la
	// direccion a la que apunta (copia superficial)
	
	v.push_back(std::move(x));

	std::cout << "buibaya" << std::endl;
}

void test3() {

	std::vector<A> v;

	// rvalue -> objeto que no ocupa una posicion IDENTIFICABLE en memoria ----- x = 1 --> 1
	// lvalue -> objeto que ocupa una posicion IDENTIFICABLE en memoria -------- x = 1 --> x

	// lo crea todo directamente por el emplace_back (mirar mas que hace el emplace_back)
	v.emplace_back(1);

	std::cout << "buibaya" << std::endl;
}

int main(int, char**) {
	reset_stats();
	test0();
	print_stats();

	reset_stats();
	test1();
	print_stats();

	reset_stats();
	test2();
	print_stats();

	reset_stats();
	test3();
	print_stats();

	return 0;
}