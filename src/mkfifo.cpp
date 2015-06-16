#define _DEFAULT_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <node.h>
#include <nan.h>

using namespace v8;

NAN_INLINE v8::Local<v8::Value> ErrnoException(
		const int erno
		, const char *what
		, const char *where
) {
	char buf[256];

	snprintf(buf, sizeof(buf), "%s %s: %s [%d]", what, where, strerror(errno), (int)(errno));
	return NanError(buf, erno);
}

NAN_METHOD(MkfifoSync) {
	NanScope();

	if (args.Length() < 2) {
		NanThrowTypeError("Wrong number of arguments");
		NanReturnUndefined();
	}

	if (!args[0]->IsString() || !args[1]->IsNumber()) {
		NanThrowTypeError("Wrong arguments");
		NanReturnUndefined();
	}


	mode_t mode = (mode_t)args[1]->Uint32Value();
	NanUtf8String pathAscii(args[0]);

	if(mkfifo(*pathAscii, mode) != 0) {
		NanThrowError(ErrnoException(errno, "mkfifo", *pathAscii));

		NanReturnUndefined();
	}

	NanReturnUndefined();
}

void Init(Handle<Object> exports) {
	exports->Set(
		NanNew<v8::String>("mkfifoSync"),
		NanNew<v8::FunctionTemplate>(MkfifoSync)->GetFunction()
	);
}

NODE_MODULE(mkfifo, Init)
