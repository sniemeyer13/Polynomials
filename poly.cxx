//Sarah Niemeyer
// Matt Sainz
//100027519	
//Homework 4 part 2 (Linked List Polynomial)

#include "poly.h"
using namespace std;

namespace main_savitch_5
{
	polynomial::polynomial(double c, unsigned int exponent)
	{
		EPSILON = std::numeric_limits<double>::epsilon();
		head_ptr = new polynode(c, 0);
		if (exponent == 0 || fabs(c) < EPSILON)
		{
			recent_ptr = head_ptr;
			tail_ptr = head_ptr;
			current_degree = 0;
		}
		else
		{
			head_ptr->set_coef(0);
			polynode* p = new polynode(c, exponent, nullptr, head_ptr);
			head_ptr->set_fore(p);
			tail_ptr = p;
			recent_ptr = p;
			current_degree = exponent;
		}
    }

    polynomial& polynomial::operator=(const polynomial& source)
    {
		EPSILON = std::numeric_limits<double>::epsilon();
		if (this == &source)
			return *this;
		if (head_ptr != nullptr)
			clear();
		delete head_ptr;
		tail_ptr = nullptr;
		head_ptr = nullptr;
		recent_ptr = nullptr;
	
		if (source.head_ptr != nullptr)
		{
			head_ptr = new polynode(source.head_ptr->coef(), source.head_ptr->exponent());
			tail_ptr = head_ptr;
			recent_ptr = head_ptr;
			current_degree = 0;
	
			for (unsigned int expo = source.next_term(0); expo != 0; expo = source.next_term(expo))
				assign_coef(source.coefficient(expo), expo);
		}
		return *this;
    }
	
    polynomial::polynomial(const polynomial& source)
    {
		EPSILON = std::numeric_limits<double>::epsilon();
		head_ptr = nullptr;
		*this = source;
    }

    polynomial::~polynomial()
    {
		clear();
		delete head_ptr;
		head_ptr = nullptr;
		tail_ptr = nullptr;
		recent_ptr = nullptr;
		current_degree = 0;
    }

    void polynomial::clear()
    {
		polynode* curr_node = head_ptr;
		while (head_ptr->fore() != nullptr)
		{
			curr_node = head_ptr->fore();
			delete head_ptr;
			head_ptr = curr_node;
		}
		head_ptr->set_coef(0);
		head_ptr->set_exponent(0);
		current_degree = 0;
		tail_ptr = head_ptr;
		recent_ptr = head_ptr;
    }
    
    double polynomial::coefficient(unsigned int exponent) const
    {
		set_recent(exponent);
		if (recent_ptr->exponent() == exponent)
			return recent_ptr->coef();
		return 0;
    }

    void polynomial::add_to_coef(double amount, unsigned int exponent)
    {
		set_recent(exponent);
		if (fabs(amount) < EPSILON)
			return;
		else
		{
			if (recent_ptr->exponent() < exponent)
			{
				polynode* new_node = new polynode(amount, exponent, recent_ptr->fore(), recent_ptr);
				if (recent_ptr->fore() != nullptr)
					recent_ptr->fore()->set_back(new_node);
				else
					tail_ptr = new_node;
				recent_ptr->set_fore(new_node);
				if (exponent > current_degree && fabs(amount) >= EPSILON)
					current_degree = exponent;
			}
			else
			{
				if (exponent == 0)
					recent_ptr->set_coef(recent_ptr->coef() + amount);
				else
				{
					if (exponent == current_degree)
					{	
						if (fabs(amount + recent_ptr->coef()) < EPSILON)
						{
							tail_ptr = recent_ptr->back();
							delete recent_ptr;
							recent_ptr = tail_ptr;
							tail_ptr->set_fore(nullptr);
							current_degree = tail_ptr->exponent();
						}
						else
							recent_ptr->set_coef(recent_ptr->coef() + amount);
					}
					else
					{
						if (fabs(amount + recent_ptr->coef()) < EPSILON)
						{
							recent_ptr->back()->set_fore(recent_ptr->fore());
							recent_ptr->fore()->set_back(recent_ptr->back());
							delete recent_ptr;
							recent_ptr = recent_ptr->back();
						}
						else
							recent_ptr->set_coef(recent_ptr->coef() + amount);
					}
				}
			}
		}
	}

    void polynomial::assign_coef(double coefficient, unsigned int exponent)
    {
		set_recent(exponent);
		if (fabs(coefficient) < EPSILON && exponent > current_degree)
			return;
		else
			if (recent_ptr->exponent() < exponent)
			{
				polynode* new_node = new polynode(coefficient, exponent, recent_ptr->fore(), recent_ptr);
				if (recent_ptr->fore() != nullptr)
				{
					recent_ptr->fore()->set_back(new_node);
				}
				else
				{
					tail_ptr = new_node;
				}
				recent_ptr->set_fore(new_node);
				if (exponent > current_degree)
					current_degree = exponent;
				recent_ptr = new_node;
			}
			else
				if (fabs(coefficient) > EPSILON || exponent == 0)
				{
					recent_ptr->set_coef(coefficient);
					if (exponent > current_degree)
						current_degree = exponent;
				}
				else
					if (exponent == current_degree)
					{
						tail_ptr = recent_ptr->back();
						delete recent_ptr;
						recent_ptr = tail_ptr;
						tail_ptr->set_fore(nullptr);
						current_degree = tail_ptr->exponent();
					}
					else
					{
						recent_ptr->back()->set_fore(recent_ptr->fore());
						recent_ptr->fore()->set_back(recent_ptr->back());
						polynode* to_delete = recent_ptr;
						recent_ptr = recent_ptr->back();
						delete to_delete;
					}
			
    }

    unsigned int polynomial::next_term(unsigned int exponent) const
    {
		if (exponent >= current_degree) 
		{
			return 0;
		}
		set_recent(exponent);   
		if (recent_ptr->fore() == nullptr) 
		{
			return 0;
		}
		return recent_ptr->fore()->exponent();
    }

	unsigned int polynomial::previous_term(unsigned int exponent) const
    {
		if (exponent <= 0) return UINT_MAX;
		set_recent(exponent - 1);
		if (recent_ptr == nullptr) return UINT_MAX;
		if (recent_ptr->exponent() == 0 && fabs(recent_ptr->coef()) < EPSILON)
			return UINT_MAX;
		else
		{
			return recent_ptr->exponent();
		}
		return UINT_MAX;
    }
    
    void polynomial::set_recent(unsigned int exponent) const
    {
		if (exponent == 0)
		{
			recent_ptr = head_ptr;
		}
		else
			if (exponent >= current_degree)
			{
				recent_ptr = tail_ptr;
			}
			else
				if (exponent < recent_ptr->exponent())
				{
					while (recent_ptr->back() != nullptr && recent_ptr->exponent() > exponent)
					recent_ptr = recent_ptr->back();
				}
				else
				{	
					while (recent_ptr->fore() != nullptr && recent_ptr->exponent() < exponent)
						recent_ptr = recent_ptr->fore();
					if (recent_ptr->exponent() > exponent)
						recent_ptr = recent_ptr->back();
				}
	}
 
 
    double polynomial::eval(double x) const
    {
		double total = 0;
		double int_total = 0;
		for(double expo = 0.0; expo <= current_degree; expo++)
		{
			double coef = coefficient(expo);
			double power = pow(x, expo);
			if(fabs(expo) <= EPSILON) power = 1;
			if(fabs(coef) <= EPSILON) power = 0;
			int_total = coef * power;
			total = int_total + total;
		}
		
		return total;
    }


    polynomial polynomial::derivative() const
    {
		polynomial p_prime;
		unsigned int expo = 0;
		
		do
		{
			double newcoef = coefficient(expo+1) * (expo+1);
			p_prime.assign_coef(newcoef, expo);
			expo++;
		}while(expo < current_degree);
		
		return p_prime;
    }
    
    polynomial operator+(const polynomial& p1, const polynomial& p2)
    {
		polynomial p = p1;
		unsigned int expo = 0;
		
		
		do
		{
			p.add_to_coef(p2.coefficient(expo), expo);
			expo = p2.next_term(expo);
		}while(expo != 0);
		
		
		return p;
    }
    
    polynomial operator-(const polynomial& p1, const polynomial& p2)
    {
		polynomial p = p1;
		unsigned int expo = 0;
		
		do
		{
			p.add_to_coef((p2.coefficient(expo))*-1, expo);
			expo = p2.next_term(expo);
		}while(expo != 0);
		
		return p;
    }
    

   polynomial operator*(const polynomial& p1, const polynomial& p2)
    {		
		polynomial p;
		
		unsigned int expo1 = 0;
		unsigned int expo2 = 0;
		
		while(expo1 <= p1.degree())
		{
			while(expo2 <= p2.degree())
			{
				double newcoef = p1.coefficient(expo1) * p2.coefficient(expo2);
				int newexpo = expo1 + expo2;
				p.add_to_coef(newcoef, newexpo);
				expo2 = p2.next_term(expo2);
				//if you are looping around back from zero to the highest number, exit the while loop
				if(expo2 == 0) break;
			}
			expo1 = p1.next_term(expo1);
			//if you are looping around back from zero to the highest number, exit the while loop
			if(expo1 == 0) break;
		}
		
		return p;
    }

    ostream& operator << (ostream& out, const polynomial& p)
    {
		unsigned int expo = 0;
		do
		{
			out << p.coefficient(expo) << "*x^" << expo;
			expo = p.next_term(expo);
			if (expo != 0) out << " + ";
		} while(expo != 0);
		return out;
    }
    
//not working

    void polynomial::find_root(
	double& answer,
	bool& success,
	unsigned int& iterations,
	double guess,
	unsigned int maximum_iterations,
	double epsilon) const
    {
		iterations = 0;
		polynomial p_prime = derivative();
		success = false;
		
		
		while(iterations < maximum_iterations)
		{
			
			if(fabs(p_prime.eval(guess)) > epsilon)
			{
				answer = guess - eval(guess)/p_prime.eval(guess);
				guess = answer; 
				iterations++;
			}
			
			else return;
			
			if(fabs(eval(guess)) <= epsilon)
			{
				answer = guess;
				success = true;
				return;
			}
		}
		
	}
}
