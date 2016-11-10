// Copyright 2016 The SwiftShader Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Query.h: Defines the gl::Query class

#ifndef LIBGL_QUERY_H_
#define LIBGL_QUERY_H_

#include "common/Object.hpp"
#include "Renderer/Renderer.hpp"

#define _GDI32_
#include <windows.h>
#include <GL/GL.h>
#include <GL/glext.h>

namespace gl
{

class Query : public NamedObject
{
public:
	Query(GLuint name, GLenum type);
	virtual ~Query();

	void begin();
	void end();
	GLuint getResult();
	GLboolean isResultAvailable();

	GLenum getType() const;

private:
	GLboolean testQuery();

	sw::Query* mQuery;
	GLenum mType;
	GLboolean mStatus;
	GLint mResult;
};

}

#endif   // LIBGL_QUERY_H_
