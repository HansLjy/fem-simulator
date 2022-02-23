//
// Created by hansljy on 2022/2/22.
//

#ifndef FEM_PATTERN_H
#define FEM_PATTERN_H

#define DECLARE_GET_INSTANCE(classname) \
static classname* GetInstance();

#define DEFINE_GET_INSTANCE(classname) \
classname *classname::GetInstance() {  \
	static classname instance;         \
	return &instance;                  \
}

#define DECLARE_XXX_FACTORY(classname) \
class classname##Factory {             \
public:                                \
	DECLARE_GET_INSTANCE(classname##Factory) \
	classname* Get##classname(const classname##Type& type); \
};

#define BEGIN_DEFINE_XXX_FACTORY(classname) \
DEFINE_GET_INSTANCE(classname##Factory)     \
                                            \
classname *classname##Factory::Get##classname(const classname##Type &type) { \
	switch(type) {


#define ADD_PRODUCT(type, classname) \
		case type: return new classname;

#define END_DEFINE_XXX_FACTORY \
		default: return nullptr;          \
	}									  \
};
#endif //FEM_PATTERN_H
