// INSPIRED BY

// cmdmp3
// A command-line MP3 player for Windows
// (console-mode version)
//
// License: MIT / X11
// Copyright (c) 2009, 2015 by James K. Lawless
// jimbo@radiks.net  http://jiml.us
// See http://www.mailsend-online.com/license2015.php
//
// To build, use the following MinGW command:
//    gcc -o cmdmp3.exe -lwinmm cmdmp3.c

#include <node.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

void sendCommand(char *);

namespace cmdmp3 {
    using v8::Exception;
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Number;
    using v8::Object;
    using v8::String;
    using v8::Value;

    void sendCommand(char *s) {
       int i;
       i=mciSendString(s,NULL,0,0);
       if(i) {
             fprintf(stderr,"Error %d when sending %s\n",i,s);
       }
    }

    void Play(const FunctionCallbackInfo<Value>& args) {
        char shortBuffer[MAX_PATH];
        char cmdBuff[MAX_PATH + 64];
        Isolate* isolate = args.GetIsolate();

        if (args.Length() < 1) {
          // Throw an Error that is passed back to JavaScript
          isolate->ThrowException(Exception::TypeError(
              String::NewFromUtf8(isolate, "Wrong number of arguments")));
          return;
        }

        GetShortPathName((LPCSTR)*String::Utf8Value(args[0]),shortBuffer,sizeof(shortBuffer));
        if(!*shortBuffer) {
            isolate->ThrowException(Exception::TypeError(
                    String::NewFromUtf8(isolate, "Cannot shorten filename")));
            return;
        }
        sendCommand("Close All");

        sprintf(cmdBuff,"Open %s Type MPEGVideo Alias theMP3",shortBuffer);
        sendCommand(cmdBuff);

        sendCommand("Play theMP3 Wait");
    }

    void init(Local<Object> exports) {
      NODE_SET_METHOD(exports, "play", Play);
    }

    NODE_MODULE(cmdmp3, init)
}
