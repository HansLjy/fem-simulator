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

#define BASE_DECLARE_CLONE(classname) \
	virtual classname* Clone() const = 0;

#define DERIVED_DECLARE_CLONE(classname) \
	classname* Clone() const;

#define DEFINE_CLONE(base, derived) \
base* derived::Clone() const {      \
	return new derived(*this);      \
}

#define DECLARE_VIRTUAL_ACCESSIBLE_MEMBER_ACCESSOR(MemberType, MemberName) 					\
public:                                                   									\
	virtual const MemberType& Get##MemberName() const;            							\
	virtual MemberType& Get##MemberName();

#define DECLARE_OVERWRITE_ACCESSIBLE_MEMBER_ACCESSOR(MemberType, MemberName) 				\
public:                                                   									\
	virtual const MemberType& Get##MemberName() const override;    							\
	virtual MemberType& Get##MemberName() override;

#define DECLARE_ACCESSIBLE_MEMBER_ACCESSOR(MemberType, MemberName, _member_name) \
public:                                                   \
	const MemberType& Get##MemberName() const;            \
	MemberType& Get##MemberName();                        \
protected:                                             	  \
	MemberType _member_name;

#define DEFINE_VIRTUAL_ACCESSIBLE_MEMBER_ACCESSOR(classname, MemberType, MemberName) \
const MemberType& classname::Get##MemberName() const {                               \
    throw "Access unsupported accessor";                                             \
}                                                                                    \
MemberType& classname::Get##MemberName() {                                           \
    throw "Access unsupported accesor";                                              \
}

#define DEFINE_ACCESSIBLE_MEMBER(classname, MemberType, MemberName, _member_name)	\
const MemberType& classname::Get##MemberName() const {								\
	return _member_name;															\
}                                                                  					\
MemberType& classname::Get##MemberName() {                         					\
	return _member_name;                                          					\
}

#define DECLARE_ACCESSIBLE_POINTER_MEMBER_ACCESSOR(MemberType, MemberName, _member_name) \
public:                                                   \
	const MemberType* Get##MemberName() const;            \
	MemberType* Get##MemberName();                        \
protected:                                             	  \
	MemberType* _member_name;

#define DEFINE_VIRTUAL_ACCESSIBLE_POINTER_MEMBER_ACCESSOR(classname, MemberType, MemberName) \
const MemberType* classname::Get##MemberName() const {                               \
    throw "Access unsupported accessor";                                             \
}                                                                                    \
MemberType* classname::Get##MemberName() {                                           \
    throw "Access unsupported accesor";                                              \
}

#define DEFINE_ACCESSIBLE_POINTER_MEMBER(classname, MemberType, MemberName, _member_name)	\
const MemberType* classname::Get##MemberName() const {								\
	return _member_name;															\
}                                                                  					\
MemberType* classname::Get##MemberName() {                         					\
	return _member_name;                                          					\
}

#endif //FEM_PATTERN_H
