#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#include <node.h>
#include <nan.h>

using namespace v8;

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
		NanThrowError(node::ErrnoException(errno, "mkfifo", strerror(errno), *pathAscii));

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
