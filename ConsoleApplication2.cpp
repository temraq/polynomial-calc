#include <windows.h>
#include <iostream>
#include <cmath>
using namespace std;

class Polynomial {
protected:
	class Term {
	public:
		int exponent;
		int coefficient;
		Term* next;
		Term(int exp, int coeff, Term* n) {
			exponent = exp;
			coefficient = coeff;
			next = n;
		}
		friend class Polynomial;
	};

public:
	Polynomial() {
		term = new Term(0, 0, nullptr); 
	}

	Polynomial(const Polynomial& p) {
		Term* q;
		Term* terms = nullptr;
		bool flag = true;
		for (q = p.term; q != nullptr; q = q->next) {
			if (flag) {
				flag = false;
				terms = new Term(q->exponent, q->coefficient, nullptr);
				term = terms;
			}
			else {
				terms->exponent = q->exponent;
				terms->coefficient = q->coefficient;
			}
			if (q->next != nullptr) {
				terms->next = new Term(0, 0, nullptr);
				terms = terms->next;
			}
		}
	}

	~Polynomial() {
		while (true) {
			if (term->next == nullptr)
				break;
			Term* p = term;
			term = term->next;
			delete p;
		}
		delete term;
	}

	Polynomial& operator=(const Polynomial& p) {
		while (true) {
			if (term->next == nullptr)
				break;
			Term* pterm = term;
			term = term->next;
			delete pterm;
		}
		delete[] term;
		Term* q;
		Term* terms = nullptr;
		bool flag = true;
		for (q = p.term; q != nullptr; q = q->next) {
			if (flag) {
				flag = false;
				terms = new Term(q->exponent, q->coefficient, nullptr);
				term = terms;
			}
			else {
				terms->exponent = q->exponent;
				terms->coefficient = q->coefficient;
			}
			if (q->next != nullptr) {
				terms->next = new Term(0, 0, nullptr);
				terms = terms->next;
			}
		}
		return *this;
	}

	void addTerm(int expon, int coeff) {
		if (term->exponent == 0 && term->coefficient == 0) {
			term->coefficient = coeff;
			term->exponent = expon;
		}
		else {
			Term* q = nullptr;
			Term* p = term;
			if (term->exponent < expon) {
				q = new Term(expon, coeff, nullptr);
				q->next = term;
				term = q;
			}
			else if (term->exponent == expon) {
				term->coefficient += coeff;
				if (term->coefficient == 0) {
					if (term->next == nullptr)
						term->exponent = 0;
					else {
						term = term->next;
						delete p;
					}
				}
			}
			else {
				bool flag = false;
				while (true) {
					if (p->next == nullptr)
						break;
					q = p->next;
					if (q->exponent == expon) {
						flag = true;
						break;
					}
					if (q->exponent < expon)
						break;
					p = p->next;
				}
				if (flag) {
					q->coefficient += coeff;
					if (q->coefficient == 0) {
						p->next = q->next;
						delete q;
					}
				}
				else {
					q = new Term(expon, coeff, nullptr);
					q->next = p->next;
					p->next = q;
				}
			}
		}
	}

	double evaluate(double x) {
		double sum = 0;
		double value;
		Term* p;
		for (p = term; p != nullptr; p = p->next) {
			value = pow(x, p->exponent);
			sum += (p->coefficient * value);
		}
		return sum;
	}

	int size(const Polynomial& p) {
		Term* i;
		int size_of_poly = 0;
		for (i = p.term; i != nullptr; i = i->next) {
			size_of_poly += 1;
		}

		return size_of_poly;
	}

	friend Polynomial operator+(const Polynomial& p, const Polynomial& q) {
		Polynomial result;
		Term* terms_p, * terms_q;
		for (terms_p = p.term; terms_p != nullptr; terms_p = terms_p->next)  
			result.addTerm(terms_p->exponent, terms_p->coefficient);
		for (terms_q = q.term; terms_q != nullptr; terms_q = terms_q->next)
			result.addTerm(terms_q->exponent, terms_q->coefficient);
		return result;
	}

	friend Polynomial operator-(const Polynomial& p, const Polynomial& q) {
		Polynomial result;
		Term* terms_p, * terms_q;
		for (terms_p = p.term; terms_p != nullptr; terms_p = terms_p->next)
			result.addTerm(terms_p->exponent, terms_p->coefficient);
		for (terms_q = q.term; terms_q != nullptr; terms_q = terms_q->next)
			result.addTerm(terms_q->exponent, -(terms_q->coefficient));
		return result;
	}

	friend Polynomial operator/(const Polynomial& p, const Polynomial& q)
	{
		Polynomial result, temp2;
		Polynomial temp = p;
		Term* terms_p, * terms_q;
		int result_size = 0;

		terms_p = p.term;
		terms_q = q.term;

		if (temp.size(temp) < 2) { //если кол-во одночленов 1 или 0
			if (terms_p->exponent >= terms_q->exponent) { 
				result.addTerm(terms_p->exponent - terms_q->exponent, terms_p->coefficient / terms_q->coefficient);
				temp = temp - result * p; //остаток
			}
			else {
				result.addTerm(0, 0);
			}

		}

		else {
			if (terms_p->exponent >= terms_q->exponent) {
				temp = p;
				while (temp.term->exponent >= terms_q->exponent) {
					result.addTerm(temp.term->exponent - terms_q->exponent, temp.term->coefficient / terms_q->coefficient);
					temp2 = result;
					temp = p - temp2 * q;
				}

			}
			else
				cout << "Incorrect input" << "\n";
			
		}

		return result;
	}

	friend Polynomial operator*(const Polynomial& p, const Polynomial& q) {
		Polynomial result;
		Term* terms_p, * terms_q;
		for (terms_p = p.term; terms_p != nullptr; terms_p = terms_p->next) {
			for (terms_q = q.term; terms_q != nullptr; terms_q = terms_q->next) {
				result.addTerm(terms_q->exponent + terms_p->exponent, terms_q->coefficient * terms_p->coefficient);
			}
		}
		return result;
	}

	

	friend ostream& operator<<(ostream& out, const Polynomial& p) {
		bool flag = true;
		if (p.term->coefficient == 0)
			out << "0";
		else {
			for (Term* q = p.term; q != nullptr; q = q->next) {
				if (flag) {
					if (q->coefficient < 0)
						out << "- ";
				}
				else {
					if (q->coefficient > 0)
						out << "+ ";
					else if (q->coefficient < 0)
						out << "- ";
				}
				if (q->coefficient == 0)
					continue;
				else if (abs(q->coefficient) == 1) {
					if (q->exponent == 0)
						out << abs(q->coefficient);
				}
				else out << abs(q->coefficient);
				if (q->exponent == 0)
					continue;
				else if (q->exponent == 1)
					out << "(xe^(x))";
				else
					out << "(xe^(x))^" << q->exponent;
				if (q->next != nullptr)
					out << " ";
				flag = false;
			}
		}
		return out;
	}

private:
	Term* term;
};


void OutputMenu() {
	cout << "\n" << endl;
	cout << "Выбрать действие:" << endl;
	cout << "1. Ввести многочлены" << endl;
	cout << "2. Сложить многочлены" << endl;
	cout << "3. Разделить многочлены" << endl;
	cout << "4. Вывести многочлены" << endl;
	cout << "5. Вывести число одночленов в многочленах" << endl;
	cout << "6. Закрыть" << endl;
	cout << "Выберите номер действия:" << endl;
	
};



int main()
{
	SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
	Polynomial p, q;
	setlocale(LC_CTYPE,"Russian");

	while (1) {
		OutputMenu();
		int k;
		cin >> k;
		switch (k) {
		case 1:
			cout << "Введите число одночленов в первом многочлене P(x,e^(x)): " << '\n';
			int nump;
			cin >> nump;
			cout << "Добавьте одночлены: " << '\n';
			for (int i = 1; i <= nump; i++) {
				int a, b;
				cin >> a >> b;
				p.addTerm(a, b);
			}


			cout << "Введите число одночленов во втором многочлене Q(x,e^(x)): " << '\n';
			int numc;
			cin >> numc;
			cout << "Добавьте одночлены: " << '\n';
			for (int i = 0; i < numc; i++) {
				int c, d;
				cin >> c >> d;
				q.addTerm(c, d);
			}
			break;
		case 2:
			cout << "(P+Q)(x) = " << p + q << '\n';
			break;
		case 3:
			cout << "(P/Q)(x) = " << p/q << '\n';
			break;
		case 4:
			cout << "P(x) = " << p << '\n';
			cout << "P(1) = " << p.evaluate(1) << '\n';
			cout << "Q(x) = " << q << '\n';
			cout << "Q(1) = " << q.evaluate(1) << '\n';
			break;
		case 5:
			cout << "Число одночленов в P(x,e^(x)) = " << p.size(p) << '\n';
			cout << "Число одночленов в Q(x,e^(x)) = " << q.size(q) << '\n';
			break;
		case 6:
			goto m1;

		default:
			goto m1;
		}


	}

m1: cout << "Done!";
	return 0;
}

