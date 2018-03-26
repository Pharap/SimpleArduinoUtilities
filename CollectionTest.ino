#include <Arduboy2.h>
#include "List.h"
#include "Array.h"
#include "Deque.h"
#include "Queue.h"
#include "Stack.h"
#include "Grid.h"
#include "TypeTraits.h"
#include "Utility.h"

Arduboy2 arduboy;

List<int, 10> list;
Array<int, 10> array;
Deque<int, 10> deque;
Stack<int, 10> stack;
Queue<int, 10> queue;
Grid<int, 5, 5> grid;

integral_constant<int, 5> five;

struct SillyClass
{
	int reallyStupidFunctionA(int a, int b)// const volatile &
	{
		return a + b;
	}
	
	static int reallyStupidFunctionB(int a, int b)// const volatile &
	{
		return a + b;
	}
	
	int reallyStupidFunctionC(int a, int b) const volatile &
	{
		return a + b;
	}
};

static_assert(is_integral<bool>::value, "Bool not integral");
static_assert(is_integral<const volatile bool>::value, "cv Bool not integral");
static_assert(is_arithmetic<int>::value, "");
static_assert(is_arithmetic<const volatile int>::value, "");
static_assert(is_arithmetic<float>::value, "");
static_assert(is_arithmetic<const volatile float>::value, "");
static_assert(is_signed<int>::value, "");
static_assert(is_signed<const volatile int>::value, "");
static_assert(is_signed<float>::value, "");
static_assert(is_signed<const volatile float>::value, "");
static_assert(is_unsigned<unsigned int>::value, "");
static_assert(is_unsigned<const volatile unsigned int>::value, "");
static_assert(!is_signed<unsigned int>::value, "");
static_assert(!is_signed<const volatile unsigned int>::value, "");

using TypeA = decltype(&SillyClass::reallyStupidFunctionA);
using TypeB = decltype(SillyClass::reallyStupidFunctionB);
using TypeC = decltype(&SillyClass::reallyStupidFunctionC);

static_assert(is_member_function_pointer<TypeA>::value, "");
static_assert(is_function<TypeB>::value, "");
static_assert(is_member_function_pointer<TypeC>::value, "");

template< typename T, typename U > using decay_equiv = is_same<decay_t<T>, U>;


static_assert(decay_equiv<int, int>::value, "");
static_assert(decay_equiv<int&, int>::value, "");
static_assert(decay_equiv<int&&, int>::value, "");
static_assert(decay_equiv<const int&, int>::value, "");
static_assert(decay_equiv<int[2], int*>::value, "");
static_assert(decay_equiv<int(int), int(*)(int)>::value, "");


template <class T> struct Number { T n; };
 
template <class T, class U> Number<typename common_type<T, U>::type> operator+(const Number<T>& lhs, const Number<U>& rhs) 
{
    return {lhs.n + rhs.n};
}

template<typename A, typename B> struct Check
{
    static_assert(is_same<A, B>::value, "not same");
};
 
int altmain()
{
    Number<int> i1 = {1}, i2 = {2};
    Number<double> d1 = {2.3}, d2 = {3.5};
    static_assert(is_same<decltype((i1 + i2).n), int>::value, "not int");
    static_assert(is_same<decltype((d1 + d2).n), double>::value, "not double");
	Check<decltype((i1 + d2).n), double> v;
    static_assert(is_same<decltype((i1 + d2).n), double>::value, "not double");
    static_assert(is_same<decltype((d1 + i2).n), double>::value, "not double");
}

void setup()
{	
	arduboy.begin();
}

void loop()
{
	if(!arduboy.nextFrame()) return;

	dequeLoop();
}

void multiLoop()
{
	arduboy.pollButtons();

	if(arduboy.justPressed(UP_BUTTON))
	{
		deque.append(10);
		list.add(10);
		stack.push(10);
		queue.enqueue(10);
	}

	if(arduboy.justPressed(DOWN_BUTTON))
	{
		deque.unappend();
		list.removeAt(0);
		stack.drop();
		queue.dequeue();
	}

	if(arduboy.justPressed(B_BUTTON))
	{
		deque.clear();
		list.clear();
		stack.clear();
		queue.clear();
	}
	
	arduboy.clear();

	arduboy.println(F("Items:"));
	
	for(uint8_t i = 0; i < deque.getCount(); ++i)
	{
		arduboy.setCursor(0, i * 8);
		arduboy.print(deque[i]);
		arduboy.setCursor(16, i * 8);
		arduboy.print(15);
		//arduboy.print(list[i]);
		arduboy.setCursor(32, i * 8);
		arduboy.print(126);
		//arduboy.print(stack[i]);
		arduboy.setCursor(48, i * 8);
		arduboy.print(204);
		//arduboy.print(queue[i]);
		arduboy.println();
	}

	arduboy.display();
}


void listLoop()
{
	arduboy.pollButtons();

	if(arduboy.justPressed(A_BUTTON))
	{
		list.add(10);
	}

	if(arduboy.justPressed(B_BUTTON))
	{
		list.clear();
	}
	
	arduboy.clear();

	arduboy.println(F("Items:"));
	

	for(uint8_t i = 0; i < list.getCount(); ++i)
	{
		arduboy.println(list[i]);
	}

	arduboy.display();
}

void dequeLoop()
{
	arduboy.pollButtons();

	if(arduboy.justPressed(UP_BUTTON))
	{
		deque.prepend(20);
	}

	if(arduboy.justPressed(LEFT_BUTTON))
	{
		deque.removeFirst(10);
	}

	if(arduboy.justPressed(DOWN_BUTTON))
	{
		deque.append(10);
	}

	if(arduboy.justPressed(RIGHT_BUTTON))
	{
		deque.removeLast(20);
	}

	if(arduboy.justPressed(B_BUTTON))
	{
		deque.clear();
	}
	
	arduboy.clear();

	arduboy.println(F("Items:"));
	

	for(uint8_t i = 0; i < deque.getCount(); ++i)
	{
		arduboy.println(deque[i]);
	}

	arduboy.display();
}

void arrayLoop()
{
	arduboy.pollButtons();

	if(arduboy.justPressed(A_BUTTON))
	{
		array[0] = 10;
	}

	if(arduboy.justPressed(B_BUTTON))
	{
		array.clear();
	}
	
	arduboy.clear();

	arduboy.println(F("Items:"));
	

	for(uint8_t i = 0; i < array.getCount(); ++i)
	{
		arduboy.println(array[i]);
	}

	arduboy.display();
}
