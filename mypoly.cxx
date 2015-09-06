//Sarah Niemeyer
//100027519
//Matt Sainz
//Linked List Part 2
//are my new functions (+, - & *) correct?
//what the heck is going on with my operator =?


#include "poly.h"
#include <cmath> 
#include <cstdlib>

using namespace std;

namespace main_savitch_5
{
	//making a new polyomial with a certain coefficient and exponent
	polynomial::polynomial(double c, unsigned int exponent)
	{	
		EPSILON = std::numeric_limits<double>::epsilon();
		//initialize a polynomial with just a blank head pointer
		head_ptr = new polynode(c, 0);
		recent_ptr = head_ptr;
		current_degree = 0;
		tail_ptr = head_ptr;
		
		
		if(exponent == 0 || fabs(c) < EPSILON)
		{
			return;
		}
		
		
		else
		{
			head_ptr->set_coef(0);
			recent_ptr = new polynode(c, exponent,nullptr,head_ptr);
			head_ptr->set_fore(recent_ptr);
			tail_ptr = recent_ptr;
			current_degree = exponent;
		}
		
		
    }

    polynomial& polynomial::operator=(const polynomial& source)
    {
		// store machine epsilon
		EPSILON = std::numeric_limits<double>::epsilon();
		
		//self assiging case
		if(*this == &source) return *this;
		
		//clear the list
		clear();
		
		
		//copying an empty list
		if(source.head_ptr == nullptr)
		{
			head_ptr = nullptr;
			return *this;
		}
		
		else
		{
			head_ptr = new polynode(source.head_ptr->coef(), source.head_ptr->exponent());
			const polynode* source_curr; 
			source_curr = source.head_ptr->next();
			polynode* curr = head_ptr;
			
			while(source_curr != nullptr)
			{
				curr->fore() = (new polynode(source_curr->coef(), source.head_ptr->exponent(), nullptr, recent_ptr));
				curr = curr->fore();
			}
		}	
		
		return *this;
	}
	
    polynomial::polynomial(const polynomial& source)
    {
		// store machine epsilon
		/* EPISLON: the smallest number your computer can recognize and 
		 * utilize (i.e. add or subtract), you use this to check your 
		 * coefficient and make sure it's valid
		 * polynode *head_ptr: this is the head pointer (the first pointer)
		 * of that polynode, we start either here or at the tail pointer when
		 * going through the list
		 * polynode *tail_ptr: this is the tail pointer (the last pointer)
		 * of that polynode
		 * mutable polynode recent_ptr: this points to the most recently
		 * used term, it is our 'place keeper' when we are iterating through 
		 * the list
		 * current_degree: this is the degree (exponent) of the current
		 * term, it is how we identify where we are in the list
		 * set_recent(): changes the most recent polynode to the polynode
		 * with the coeffiecient in the parenthesies, or (if that exponent
		 * doesn't exist), to the first polynode that is just less than that
		 * exponent*/
		 
		 //this doesn't work
		EPSILON = std::numeric_limits<double>::epsilon();
		head_ptr = new polynode(0, 0);
		
		*this = source;
    }

    polynomial::~polynomial()
    {
		clear();
    }
    
    void polynomial::clear()
    {
		polynode* current_ptr = head_ptr;
		while(head_ptr->fore() != nullptr)
		{
			current_ptr = head_ptr->fore();
			delete head_ptr;
			head_ptr = current_ptr;
		}
		
		
		head_ptr->set_exponent(0);
		head_ptr->set_coef(0);
		head_ptr->set_fore(nullptr);
		head_ptr->set_back(nullptr);
		current_degree = 0;
		
		
		tail_ptr = head_ptr;
		set_recent(0);
    }
    
    double polynomial::coefficient(unsigned int exponent) const
    {
		set_recent(exponent);
		if(recent_ptr->exponent() == exponent)
		{
			return recent_ptr->coef();
		}
		else
			return 0;
    }

    void polynomial::add_to_coef(double amount, unsigned int exponent)
    {
		/* suppose our polynomial has a node for 5x^6
		 * 	if we add -5 to the x^7 then we have to remove this node (5+-5 = 0)
		 * if we add 2 to the x^7 coefficient, we just change the coefficient to become 7(5+2=7)
		 * suppose our polynomial never had an x^7 node before
		 * if we add 5 to this coefficient then we'll add a new node for this term*/
		 set_recent(exponent);
		 
		 //if you are adding zero, just return the same thing
		 if(fabs(amount) < EPSILON)
		 {
			 return;
		 }
		 
		 
		 //if the node already exisits
		 if(exponent == recent_ptr->exponent())
		 {
			int newcoefficient = recent_ptr->coef() + amount;
			assign_coef(newcoefficient, exponent);
		 }
		 
		 //if it doesn't exist
		 else
		 {
			 assign_coef(amount, exponent);
		 }
	}

    void polynomial::assign_coef(double coefficient, unsigned int exponent)
    {
		 set_recent(exponent);
		 
		 
		 //if the coefficient is zero and the exponent is greater than any exponent we have before,
		 //just return what we aleady have
		 if(fabs(coefficient) < EPSILON && exponent > current_degree)
		 {
			 return;
		 }
		 
		 
		 //if you never had the node of an exponent this high, add a new one as the tail_ptr
		 else if(current_degree < exponent)
		 {
			 recent_ptr = new polynode(coefficient, exponent, nullptr, recent_ptr);
			 tail_ptr->set_fore(recent_ptr);
			 current_degree = exponent;
			 tail_ptr = recent_ptr;
		 }
		 
		 
		 //if it's not in the list, add it
		 else if(exponent > recent_ptr->exponent())
		 {
			 polynode* p = new polynode(coefficient, exponent, recent_ptr->fore(), recent_ptr);
			 recent_ptr->set_fore(p);
			 p->fore()->set_back(p);
			 recent_ptr = p;
		 }
		 
		 //if it's already in the list (the coefficient is not zero or the 
		 //exponent is zero,) just change the coefficient
		 else if(coefficient != 0 || exponent == 0)
		 {
			 recent_ptr->set_coef(coefficient);
		 }
		 
		 //if the exponent is the very last one that we have and the coefficient
		 //is zero, just remove the tail pointer
		 else if(exponent == current_degree && fabs(coefficient) < EPSILON)
		 {
			 tail_ptr = recent_ptr->back();
			 delete recent_ptr;
			 set_recent(exponent-1);
			 current_degree = recent_ptr->exponent();
			 recent_ptr->set_fore(nullptr);
			 recent_ptr->set_back(tail_ptr->back());
		 }
		 
		 //else if the coefficient is zero and it's not the head or tail, 
		 //then remove that pointer
		 else
		 {
			 polynode* p = recent_ptr->fore();
			 delete recent_ptr;
			 recent_ptr = p;
		 }
		 
    }

    unsigned int polynomial::next_term(unsigned int exponent) const
    {

			//if the exponent is at the end of the list, return zero
			if(exponent >= current_degree) return 0;
			
			
			set_recent(exponent);
			recent_ptr = recent_ptr->fore();
			int term = recent_ptr->exponent();
			return term;
    }

	unsigned int polynomial::previous_term(unsigned int exponent) const
    {
		set_recent(exponent);
		
		
		//if the exponent is zero
		if(exponent == 0) return current_degree;
		
		//if the exponent isn't zero, set recent to whatever the exponent 
		//of the previous term
		else
		{
			set_recent(exponent-1);
			return recent_ptr->exponent();
		}
    }
    
    void polynomial::set_recent(unsigned int exponent) const
    {
		if(exponent == 0)
		{
			recent_ptr = head_ptr;
		}
		else if(exponent >= current_degree)
		{
			recent_ptr = tail_ptr;
		}
		else if(recent_ptr->exponent() > exponent)
		{
			while(recent_ptr->back() != nullptr && recent_ptr->exponent() > exponent)
				recent_ptr = recent_ptr->back();
		}
		else
		{
			while(recent_ptr->fore() != nullptr && recent_ptr->exponent() < exponent)
			{
				recent_ptr = recent_ptr->fore();
			}
			if(recent_ptr->exponent() > exponent)
				recent_ptr = recent_ptr->back();
		}
	}
    
    double polynomial::eval(double x) const
    {
		double total = 0;
		for(double expo = 0.0; expo <= current_degree; expo++)
		{
			double coef = coefficient(expo);
			double power = pow(x, expo);
			total = coef * power;
			total = total + total;
		}
		
		return total;
    }

    polynomial polynomial::derivative() const
    {
		polynomial p_prime;
		unsigned int expo = 0;
		
		do
		{
			int newcoef = coefficient(expo+1) * (expo+1);
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
		polynomial p = p1;
		unsigned int expo = 0;
		
		do
		{
			int coef = p2.coefficient(expo) * p2.coefficient(expo);
			p.assign_coef(coef, expo);
			expo = p2.next_term(expo);
		}while(expo != 0);
		
		return p;
    }

    ostream& operator << (ostream& out, const polynomial& p)
    {
		unsigned int expo = 0;
		do
		{
			out<<p.coefficient(expo)<<"*x" << expo;
			expo = p.next_term(expo);
			if(expo != 0)out<<"+";
		}while(expo!= 0);
		return out;
    }
    
    void polynomial::find_root(
	double& answer,
	bool& success,
	unsigned int& iterations,
	double guess,
	unsigned int maximum_iterations,
	double epsilon) const
    {
		
		guess = rand()%100;
		iterations = 0;
		//count up iterations to see if you just ran out of time or actually got the root
		answer = eval(guess);
		polynomial p_prime = derivative();
		success = true;
		while(fabs(answer) < EPSILON && iterations >= maximum_iterations)
		{
			answer = guess - eval(guess)/p_prime.eval(guess);
			iterations++;
		}
		if(iterations < maximum_iterations) success = true;
	}
}
