#define _DEFAULT_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <node.h>
#include <nan.h>

using namespace v8;

void MkfifoSync(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	if (args.Length() < 2) {
		Nan::ThrowTypeError("Wrong number of arguments");
		args.GetReturnValue().Set(Nan::Undefined());
	}

	if (!args[0]->IsString() || !args[1]->IsNumber()) {
		Nan::ThrowTypeError("Wrong arguments");
		args.GetReturnValue().Set(Nan::Undefined());
	}


	mode_t mode = (mode_t)args[1]->Uint32Value();
	Nan::Utf8String pathAscii(args[0]);

	if(mkfifo(*pathAscii, mode) != 0) {
		Nan::ThrowError(Nan::NanErrnoException(errno, "mkfifo", *pathAscii));

		args.GetReturnValue().Set(Nan::Undefined());
	}

	args.GetReturnValue().Set(Nan::Undefined());
}

void Init(Handle<Object> exports) {
	Nan::SetMethod(
		exports,
		"mkfifoSync",
		MkfifoSync
	);
}

NODE_MODULE(mkfifo, Init);
