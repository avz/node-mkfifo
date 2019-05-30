#define _DEFAULT_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <string>

#include "napi.h"

Napi::Value MkfifoSync(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();

	if (args.Length() < 2) {
		Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();

		return env.Undefined();
	}

	if (!args[0].IsString() || !args[1].IsNumber()) {
		Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();

		return env.Undefined();
	}

	const mode_t mode = (mode_t)args[1].ToNumber().Uint32Value();
	const std::string path(args[0].ToString().Utf8Value());

	if (mkfifo(path.c_str(), mode) != 0) {
		Napi::TypeError::New(env, "mkfifo(" + path + "):" + strerror(errno)).ThrowAsJavaScriptException();

		return env.Undefined();
	}

	return env.Undefined();
}

class MkfifoAsyncWorker : public Napi::AsyncWorker
{
	const std::string path;
	mode_t mode;
	int error;

public:

	MkfifoAsyncWorker(Napi::Function &callback, const std::string &path, mode_t mode)
		: Napi::AsyncWorker(callback), path(path), mode(mode)
	{
	}

	~MkfifoAsyncWorker()
	{
	}

	void Execute()
	{
		if (mkfifo(path.c_str(), mode) == 0) {
			error = 0;
		} else {
			error = errno;
		}
	}

	void OnOK()
	{
		Napi::Env env = Env();
		Napi::HandleScope scope(env);

		if (error == 0) {
			Callback().Call({env.Undefined()});

			return;
		}

		Napi::Object err = Napi::Object::New(env);

		err.Set("errno", (int)error);
		err.Set("errstr", strerror(error));

		this->Callback().Call({err});
	}
};

Napi::Value MkfifoAsync(const Napi::CallbackInfo& args)
{
	Napi::Env env = args.Env();

	if (args.Length() < 2) {
		Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();

		return env.Undefined();
	}

	if (!args[0].IsString() || !args[1].IsNumber()) {
		Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();

		return env.Undefined();
	}

	const std::string path(args[0].ToString().Utf8Value());
	const mode_t mode = (mode_t)args[1].ToNumber().Uint32Value();

	if (!args[2].IsFunction()) {
		Napi::TypeError::New(env, "callback must be a function").ThrowAsJavaScriptException();

		return env.Undefined();
	}

	Napi::Function callback = args[2].As<Napi::Function>();
	MkfifoAsyncWorker* worker = new MkfifoAsyncWorker(callback, path, mode);

	worker->Queue();

	return env.Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
	exports.Set(Napi::String::New(env, "mkfifoSync"), Napi::Function::New(env, MkfifoSync));
	exports.Set(Napi::String::New(env, "mkfifo"), Napi::Function::New(env, MkfifoAsync));

	return exports;
}

NODE_API_MODULE(mkfifo, Init);
