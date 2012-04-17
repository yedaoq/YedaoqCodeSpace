
#ifndef __base_task_h__
#define __base_task_h__

#pragma once

#include "tuple.h"

namespace base
{
    const size_t kDeadTask = 0xDEAD7A53;
}

// Task表示可执行的任务, 通常用于在其它线程中执行代码或者在消息循环中安排
// 将来执行.
class Task
{
public:
	virtual ~Task(){};

    // Tasks在Run()函数调用完毕自动删除.
    virtual void Run() = 0;
};

class CancelableTask : public Task
{
public:
	virtual ~CancelableTask(){};

    // 不是所有的任务都支持取消操作.
    virtual void Cancel() = 0;
};

// 删除对象的Task.
template<class T>
class DeleteTask : public CancelableTask
{
public:
    explicit DeleteTask(const T* obj) : obj_(obj) {}
    virtual void Run()
    {
        delete obj_;
    }
    virtual void Cancel()
    {
        obj_ = NULL;
    }

private:
    const T* obj_;
};

// 调用对象Release()方法的Task.
template<class T>
class ReleaseTask : public CancelableTask
{
public:
    explicit ReleaseTask(const T* obj) : obj_(obj) {}
    virtual void Run()
    {
        if(obj_)
        {
            obj_->Release();
        }
    }
    virtual void Cancel()
    {
        obj_ = NULL;
    }

private:
    const T* obj_;
};

// Equivalents for use by base::Bind().
template<typename T>
void DeletePointer(T* obj)
{
    delete obj;
}

template<typename T>
void ReleasePointer(T* obj)
{
    obj->Release();
}

// RunnableMethodTraits --------------------------------------------------------
//
// This traits-class is used by RunnableMethod to manage the lifetime of the
// callee object.  By default, it is assumed that the callee supports AddRef
// and Release methods.  A particular class can specialize this template to
// define other lifetime management.  For example, if the callee is known to
// live longer than the RunnableMethod object, then a RunnableMethodTraits
// struct could be defined with empty RetainCallee and ReleaseCallee methods.
//
// The DISABLE_RUNNABLE_METHOD_REFCOUNT macro is provided as a convenient way
// for declaring a RunnableMethodTraits that disables refcounting.

// Convenience macro for declaring a RunnableMethodTraits that disables
// refcounting of a class.  This is useful if you know that the callee
// will outlive the RunnableMethod object and thus do not need the ref counts.
//
// The invocation of DISABLE_RUNNABLE_METHOD_REFCOUNT should be done at the
// global namespace scope.  Example:
//
//   namespace foo {
//   class Bar {
//     ...
//   };
//   }  // namespace foo
//
//   DISABLE_RUNNABLE_METHOD_REFCOUNT(foo::Bar)
//
// This is different from DISALLOW_COPY_AND_ASSIGN which is declared inside the
// class.
template<typename T> 
struct RunnableMethodTraits
{ 
    void RetainCallee(T* manager) {} 
    void ReleaseCallee(T* manager) {} 
};

// RunnableMethod and RunnableFunction -----------------------------------------
//
// Runnable methods are a type of task that call a function on an object when
// they are run. We implement both an object and a set of NewRunnableMethod and
// NewRunnableFunction functions for convenience. These functions are
// overloaded and will infer the template types, simplifying calling code.
//
// The template definitions all use the following names:
// T                - the class type of the object you're supplying
//                    this is not needed for the Static version of the call
// Method/Function  - the signature of a pointer to the method or function you
//                    want to call
// Param            - the parameter(s) to the method, possibly packed as a Tuple
// A                - the first parameter (if any) to the method
// B                - the second parameter (if any) to the method
//
// Put these all together and you get an object that can call a method whose
// signature is:
//   R T::MyFunction([A[, B]])
//
// Usage:
// PostTask(FROM_HERE, NewRunnableMethod(object, &Object::method[, a[, b]])
// PostTask(FROM_HERE, NewRunnableFunction(&function[, a[, b]])

// RunnableMethod and NewRunnableMethod implementation -------------------------

template<class T, class Method, class Params>
class RunnableMethod : public CancelableTask
{
public:
    RunnableMethod(T* obj, Method meth, const Params& params)
        : obj_(obj), meth_(meth), params_(params)
    {
        traits_.RetainCallee(obj_);

#ifdef BOOST_STATIC_ASSERT
		BOOST_STATIC_ASSERT(base::internal::ParamsUseScopedRefptrCorrectly<Params>::value);
#endif
#ifdef COMPILE_ASSERT
        COMPILE_ASSERT(
            (base::internal::ParamsUseScopedRefptrCorrectly<Params>::value),
            badrunnablemethodparams);
#endif
    }

    ~RunnableMethod()
    {
        ReleaseCallee();
        obj_ = reinterpret_cast<T*>(base::kDeadTask);
    }

    virtual void Run()
    {
        if(obj_)
        {
            DispatchToMethod(obj_, meth_, params_);
        }
    }

    virtual void Cancel()
    {
        ReleaseCallee();
    }

private:
    void ReleaseCallee()
    {
        T* obj = obj_;
        obj_ = NULL;
        if(obj)
        {
            traits_.ReleaseCallee(obj);
        }
    }

    T* obj_;
    Method meth_;
    Params params_;
    RunnableMethodTraits<T> traits_;
};

template<class T, class Method>
inline CancelableTask* NewRunnableMethod(T* object, Method method)
{
    return new RunnableMethod<T, Method, Tuple0>(object, method, MakeTuple());
}

template<class T, class Method, class A>
inline CancelableTask* NewRunnableMethod(T* object, Method method, const A& a)
{
    return new RunnableMethod<T, Method, Tuple1<A> >(object,
        method, MakeTuple(a));
}

template<class T, class Method, class A, class B>
inline CancelableTask* NewRunnableMethod(T* object, Method method,
                                         const A& a, const B& b)
{
    return new RunnableMethod<T, Method, Tuple2<A, B> >(object, method,
        MakeTuple(a, b));
}

template<class T, class Method, class A, class B, class C>
inline CancelableTask* NewRunnableMethod(T* object, Method method,
                                         const A& a, const B& b, const C& c)
{
    return new RunnableMethod<T, Method, Tuple3<A, B, C> >(object, method,
        MakeTuple(a, b, c));
}

template<class T, class Method, class A, class B, class C, class D>
inline CancelableTask* NewRunnableMethod(T* object, Method method,
                                         const A& a, const B& b,
                                         const C& c, const D& d)
{
    return new RunnableMethod<T, Method, Tuple4<A, B, C, D> >(object, method,
        MakeTuple(a, b, c, d));
}

template<class T, class Method, class A, class B, class C, class D, class E>
inline CancelableTask* NewRunnableMethod(T* object, Method method,
                                         const A& a, const B& b,
                                         const C& c, const D& d, const E& e)
{
    return new RunnableMethod<T,
        Method,
        Tuple5<A, B, C, D, E> >(object,
        method,
        MakeTuple(a, b, c, d, e));
}

template<class T, class Method, class A, class B, class C, class D, class E,
class F>
inline CancelableTask* NewRunnableMethod(T* object, Method method,
    const A& a, const B& b,
    const C& c, const D& d, const E& e,
    const F& f)
{
    return new RunnableMethod<T,
        Method,
        Tuple6<A, B, C, D, E, F> >(object,
        method,
        MakeTuple(a, b, c, d, e, f));
}

template<class T, class Method, class A, class B, class C, class D, class E,
class F, class G>
inline CancelableTask* NewRunnableMethod(T* object, Method method,
    const A& a, const B& b,
    const C& c, const D& d, const E& e,
    const F& f, const G& g)
{
    return new RunnableMethod<T,
        Method,
        Tuple7<A, B, C, D, E, F, G> >(object,
        method,
        MakeTuple(a, b, c, d, e, f, g));
}

// RunnableFunction and NewRunnableFunction implementation ---------------------

template<class Function, class Params>
class RunnableFunction : public Task
{
public:
    RunnableFunction(Function function, const Params& params)
        : function_(function), params_(params)
    {
#ifdef BOOST_STATIC_ASSERT
		//BOOST_STATIC_ASSERT(base::internal::ParamsUseScopedRefptrCorrectly<Params>::value);
#endif
#ifdef COMPILE_ASSERT
		//COMPILE_ASSERT((base::internal::ParamsUseScopedRefptrCorrectly<Params>::value), badrunnablefunctionparams);
#endif
    }

    ~RunnableFunction()
    {
        function_ = reinterpret_cast<Function>(base::kDeadTask);
    }

    virtual void Run()
    {
        if(function_)
        {
            DispatchToFunction(function_, params_);
        }
    }

private:
    Function function_;
    Params params_;
};

template<class Function>
inline Task* NewRunnableFunction(Function function)
{
    return new RunnableFunction<Function, Tuple0>(function, MakeTuple());
}

template<class Function, class A>
inline Task* NewRunnableFunction(Function function, const A& a)
{
    return new RunnableFunction<Function, Tuple1<A> >(function, MakeTuple(a));
}

template<class Function, class A, class B>
inline Task* NewRunnableFunction(Function function, const A& a, const B& b)
{
    return new RunnableFunction<Function, Tuple2<A, B> >(function,
        MakeTuple(a, b));
}

template<class Function, class A, class B, class C>
inline Task* NewRunnableFunction(Function function, const A& a, const B& b,
                                 const C& c)
{
    return new RunnableFunction<Function, Tuple3<A, B, C> >(function,
        MakeTuple(a, b, c));
}

template<class Function, class A, class B, class C, class D>
inline Task* NewRunnableFunction(Function function, const A& a, const B& b,
                                 const C& c, const D& d)
{
    return new RunnableFunction<Function, Tuple4<A, B, C, D> >(function,
        MakeTuple(a, b, c, d));
}

template<class Function, class A, class B, class C, class D, class E>
inline Task* NewRunnableFunction(Function function, const A& a, const B& b,
                                 const C& c, const D& d, const E& e)
{
    return new RunnableFunction<Function, Tuple5<A, B, C, D, E> >(function,
        MakeTuple(a, b, c, d, e));
}

template<class Function, class A, class B, class C, class D, class E,
class F>
    inline Task* NewRunnableFunction(Function function, const A& a, const B& b,
    const C& c, const D& d, const E& e,
    const F& f)
{
    return new RunnableFunction<Function, Tuple6<A, B, C, D, E, F> >(function,
        MakeTuple(a, b, c, d, e, f));
}

template<class Function, class A, class B, class C, class D, class E,
class F, class G>
    inline Task* NewRunnableFunction(Function function, const A& a, const B& b,
    const C& c, const D& d, const E& e, const F& f,
    const G& g)
{
    return new RunnableFunction<Function, Tuple7<A, B, C, D, E, F, G> >(function,
        MakeTuple(a, b, c, d, e, f, g));
}

template<class Function, class A, class B, class C, class D, class E,
class F, class G, class H>
    inline Task* NewRunnableFunction(Function function, const A& a, const B& b,
    const C& c, const D& d, const E& e, const F& f,
    const G& g, const H& h)
{
    return new RunnableFunction<Function, Tuple8<A, B, C, D, E, F, G, H> >(
        function, MakeTuple(a, b, c, d, e, f, g, h));
}

struct CRAII_PreposeTaskRunner
{
	CRAII_PreposeTaskRunner(Task* task)
	{
		if(task)
		{
			task->Run();
			delete task;
		}
	}
};

struct CRAII_ClosureTaskRunner
{
	CRAII_ClosureTaskRunner(Task* task)
		: task_(task)
	{}

	~CRAII_ClosureTaskRunner()
	{
		if(task_)
		{
			task_->Run();
			delete task_;
		}
	}

	Task* task_;
};

#endif //__base_task_h__