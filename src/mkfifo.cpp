#define _DEFAULT_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>


#include <v8.h>
#include <node.h>
#include <nan.h>

using namespace v8;

void MkfifoSync(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	if (args.Length() < 2) {
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}

	if (!args[0]->IsString() || !args[1]->IsNumber()) {
		Nan::ThrowTypeError("Wrong arguments");
		return;
	}

	mode_t mode = (mode_t)args[1]->Uint32Value();
	Nan::Utf8String pathAscii(args[0]);

	if (mkfifo(*pathAscii, mode) != 0) {
		Nan::ThrowError(Nan::ErrnoException(errno, "mkfifo", *pathAscii));
		return;
	}
}

class MkfifoInfo {
public:
	Nan::Utf8String path;
	mode_t mode;
	Nan::Persistent<Function> cb;
	int error;

	MkfifoInfo(Local<String> path, mode_t mode, Local<Function> callback)
		: path(path), mode(mode), error(0)
	{
		cb.Reset(callback);
	}

	~MkfifoInfo() {
		cb.Reset();
	}
};

void MkfifoAsyncWorkHandler(uv_work_t* req) {
	MkfifoInfo* mkfifoInfo = (MkfifoInfo *)req->data;

	if (mkfifo(*mkfifoInfo->path, mkfifoInfo->mode) != 0) {
		mkfifoInfo->error = errno;
	}
}


void MkfifoAsyncWorkReturnHandler(uv_work_t* req) {
	MkfifoInfo* mkfifoInfo = (MkfifoInfo *)req->data;
	Nan::HandleScope scope;

	Local<Value> argv[1];

	if (mkfifoInfo->error) {
		argv[0] = Nan::ErrnoException(mkfifoInfo->error, "mkfifo", *mkfifoInfo->path);
	} else {
		argv[0] = Nan::Undefined();
	}

	Local<Function> cb = Nan::New(mkfifoInfo->cb);

	Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, 1, argv);

	delete mkfifoInfo;
	delete req;
}

void MkfifoAsync(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	if (args.Length() < 3) {
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}

	if (!args[0]->IsString()) {
		return Nan::ThrowTypeError("path must be a string");
	}

	Local<String> path = args[0].As<String>();

	if (!args[1]->IsNumber()) {
		return Nan::ThrowTypeError("mode must be a number");
	}

	mode_t mode = (mode_t)args[1]->Uint32Value();

	if (!args[2]->IsFunction()) {
		return Nan::ThrowTypeError("callback must be a function");
	}

	Local<Function> cb = Local<Function>::Cast(args[2]);

	MkfifoInfo* mkfifoInfo = new MkfifoInfo(path, mode, cb);

	uv_work_t* req = new uv_work_t;
	req->data = mkfifoInfo;
	uv_queue_work(
		uv_default_loop(),
		req,
		MkfifoAsyncWorkHandler,
		(uv_after_work_cb)MkfifoAsyncWorkReturnHandler
	);
}

void Init(Handle<Object> exports) {
	Nan::SetMethod(
		exports,
		"mkfifoSync",
		MkfifoSync
	);

	Nan::SetMethod(
		exports,
		"mkfifo",
		MkfifoAsync
	);
}

NODE_MODULE(mkfifo, Init);
