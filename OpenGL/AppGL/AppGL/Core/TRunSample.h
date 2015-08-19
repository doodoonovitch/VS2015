#pragma once

#ifndef CORE_TRUNSAMPLE_H
#define CORE_TRUNSAMPLE_H

template<typename TSample>
class TRunSample
{
public:

	TRunSample()
	{
		_sampleInstance = &_sample;
	}

	~TRunSample()
	{

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

private:

	TSample _sample;

	static TSample* _sampleInstance;
};

template <typename TSample> TSample* TRunSample<TSample>::_sampleInstance = nullptr;

#endif// CORE_TRUNSAMPLE_H