#pragma once

#ifndef CORE_TRUNSAMPLE_H
#define CORE_TRUNSAMPLE_H

class RunSample
{
public:

	RunSample() {}
	virtual ~RunSample() {}

	virtual void Activate() = 0;
	virtual void Unactivate() = 0;

};


template<typename TSample>
class TRunSample : public RunSample
{
public:

	TRunSample()
	{
		_sampleInstance = &_sample;
	}

	~TRunSample()
	{

	}

	virtual void Activate()
	{
		TRunSample<TSample>::OnInit();

		// register callbacks
		glutCloseFunc(TRunSample<TSample>::OnShutdown);
		glutDisplayFunc(TRunSample<TSample>::OnRender);
		glutReshapeFunc(TRunSample<TSample>::OnResize);
		glutKeyboardFunc(TRunSample<TSample>::OnKey);
		glutMouseFunc(TRunSample<TSample>::OnMouseDown);
		glutMotionFunc(TRunSample<TSample>::OnMouseMove);
		glutIdleFunc(TRunSample<TSample>::OnIdle);
	}

	virtual void Unactivate()
	{
		glutCloseFunc(nullptr);
		glutDisplayFunc(nullptr);
		glutReshapeFunc(nullptr);
		glutKeyboardFunc(nullptr);
		glutMouseFunc(nullptr);
		glutMotionFunc(nullptr);
		glutIdleFunc(nullptr);

		TRunSample<TSample>::OnShutdown();
	}

	static void OnRender()
	{
		_sampleInstance->OnRender();
	}

	static void OnShutdown()
	{
		_sampleInstance->OnShutdown();
	}

	static void OnResize(int w, int h)
	{
		_sampleInstance->OnResize(w, h);
	}

	static void OnInit()
	{
		_sampleInstance->OnInit();
	}

	static void OnIdle()
	{
		_sampleInstance->OnIdle();
	}

	static void OnMouseDown(int button, int s, int x, int y)
	{
		_sampleInstance->OnMouseDown(button, s, x, y);
	}

	static void OnMouseMove(int x, int y)
	{
		_sampleInstance->OnMouseMove(x, y);
	}

	static void OnKey(unsigned char key, int x, int y)
	{
		_sampleInstance->OnKey(key, x, y);
	}

private:

	TSample _sample;

	static TSample* _sampleInstance;
};

template <typename TSample> TSample* TRunSample<TSample>::_sampleInstance = nullptr;

#endif// CORE_TRUNSAMPLE_H