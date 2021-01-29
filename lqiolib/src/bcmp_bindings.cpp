/*
 *  $Id: bcmp_bindings.cpp 14405 2021-01-24 22:01:02Z greg $
 *
 *  Created by Martin Mroz on 16/04/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <cstdio>
#include <sstream>
#include <cstring>
#include <lqx/Environment.h>
#include <lqx/SymbolTable.h>
#include <lqx/MethodTable.h>
#include <lqx/LanguageObject.h>
#include <lqx/Array.h>
#include <lqx/RuntimeException.h>

#include "bcmp_document.h"

namespace BCMP {
    const char * __lqx_residence_time           = "residence_time";
    const char * __lqx_throughput               = "throughput";
    const char * __lqx_utilization              = "utilization";
    const char * __lqx_queue_length             = "queue_length";
    

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- [Object] */
/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

    class LQXObject : public LQX::LanguageObject {
    protected:
	typedef double (Model::Result::*get_result_fptr)() const;

	struct attribute_table_t
	{
	    attribute_table_t( get_result_fptr m=nullptr ) : mean(m) {}
	    LQX::SymbolAutoRef operator()( const Model::Result& result ) const { return LQX::Symbol::encodeDouble( (result.*mean)() ); }
	    const get_result_fptr mean;
	};

    public:
	LQXObject( uint32_t kLQXobject, const Model::Result * result ) : LQX::LanguageObject(kLQXobject), _result(result)
	    {
	    }

	virtual LQX::SymbolAutoRef getPropertyNamed(LQX::Environment* env, const std::string& name) 
	    {
		std::map<const std::string,attribute_table_t>::const_iterator attribute =  __attributeTable.find( name.c_str() );
		if ( attribute != __attributeTable.end() ) {
		    try {
			if ( _result ) {
			    return attribute->second( *_result );
			}
		    }
		    catch ( const LQIO::should_implement& e ) {
		    }
		}

		/* Anything we don't handle may be handled by our superclass */
		return this->LanguageObject::getPropertyNamed(env, name);
	    }

        const Model::Result* getDOMObject() const { return _result; }

    protected:
        const Model::Result * _result;
        static const std::map<const std::string,attribute_table_t> __attributeTable;

    };

    const std::map<const std::string,LQXObject::attribute_table_t> LQXObject::__attributeTable =
    {
	{ __lqx_residence_time,	attribute_table_t( &Model::Result::residence_time ) },
	{ __lqx_throughput,     attribute_table_t( &Model::Result::throughput ) },
	{ __lqx_utilization,    attribute_table_t( &Model::Result::utilization ) },
	{ __lqx_queue_length,   attribute_table_t( &Model::Result::queue_length ) }
    };

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- [Station] */
/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
#pragma mark -

    class LQXStation : public LQXObject {
    public:

        const static uint32_t kLQXStationObjectTypeId = 10+1;

        /* Designated Initializers */
        LQXStation(const Model::Station* proc) : LQXObject(kLQXStationObjectTypeId,proc)
            {
            }

        virtual ~LQXStation()
            {
            }

        /* Comparison and Operators */
        virtual bool isEqualTo(const LQX::LanguageObject* other) const
            {
                const LQXStation* station = dynamic_cast<const LQXStation *>(other);
                return station && station->getDOMStation() == getDOMStation();  /* Return a comparison of the types */
            }

        virtual std::string description() const
            {
                /* Return a description of the class */
                std::stringstream ss;
//                ss << getTypeName() << "(" << getDOMStation()->getName() << ")";
		return ss.str();
	    }

	virtual std::string getTypeName() const
	    {
		return Model::Station::__typeName;
	    }

        const Model::Station* getDOMStation() const { return dynamic_cast<const Model::Station*>(_result); }


#if 0
	virtual LQX::SymbolAutoRef getPropertyNamed(LQX::Environment* env, const std::string& name) 
	    {
		/* All the valid properties of classs */
		if (name == "utilization") {
		    return LQX::Symbol::encodeDouble(getDOMStation()->utilization());
		}

		/* Anything we don't handle may be handled by our superclass */
		return this->LanguageObject::getPropertyNamed(env, name);
	    }
#endif
    };

    class LQXGetStation : public LQX::Method {
    public:
	LQXGetStation(const BCMP::Model* model) : _model(model), _symbolCache() {}
	virtual ~LQXGetStation() {}

	/* Basic information for the method itself */
	virtual std::string getName() const { return Model::Station::__typeName; }
	virtual const char* getParameterInfo() const { return "s"; }
	virtual std::string getHelp() const { return "Returns the station associated with a name."; }

	/* Invocation of the method from the language */
	virtual LQX::SymbolAutoRef invoke(LQX::Environment* env, std::vector<LQX::SymbolAutoRef >& args) {

	    /* Decode the name of the class and look it up in cache */
	    const std::string stationName = decodeString(args, 0);
	    if (_symbolCache.find(stationName) != _symbolCache.end()) {
		return _symbolCache[stationName];
	    }

	    /* Obtain the class reference  */
	    try {
		/* Return an encapsulated reference to the class */
		LQXStation* stationObject = new LQXStation(&_model->stationAt(stationName));
		_symbolCache[stationName] = LQX::Symbol::encodeObject(stationObject, false);
		return _symbolCache[stationName];
	    }
	    catch ( const std::out_of_range& e ) {
		throw LQX::RuntimeException( "No station specified with name ", stationName.c_str() );
		return LQX::Symbol::encodeNull();
	    }
	}

    private:
	const Model* _model;
	std::map<std::string,LQX::SymbolAutoRef> _symbolCache;
    };

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- [Class] */
/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

    class LQXClass : public LQXObject
    {
    public:

	const static uint32_t kLQXClassObjectTypeId = 10+0;

	/* Designated Initializers */
        LQXClass(const Model::Station::Class* class_) : LQXObject(kLQXClassObjectTypeId,class_)
            {
            }

	virtual ~LQXClass()
	    {
	    }

	/* Comparison and Operators */
	virtual bool isEqualTo(const LQX::LanguageObject* other) const
	    {
		const LQXClass* class_ = dynamic_cast<const LQXClass *>(other);
		return class_ && class_->getDOMClass() == getDOMClass();	/* Return a comparison of the types */
	    }

	virtual std::string description() const
	    {
		/* Return a description of the class */
                std::stringstream ss;
//                ss << getTypeName() << "(" << getDOMClass()->getName() << ")";
		return ss.str();
	    }

	virtual std::string getTypeName() const
	    {
		return Model::Station::Class::__typeName;
	    }

        const Model::Station::Class* getDOMClass() const { return dynamic_cast<const Model::Station::Class*>(_result); }

    private:
    };

    class LQXGetClass : public LQX::Method {
    public:
	LQXGetClass(const Model* model) {}
	virtual ~LQXGetClass() {}

	/* Basic information for the method itself */
	virtual std::string getName() const { return Model::Station::Class::__typeName; }
	virtual const char* getParameterInfo() const { return "os"; }
	virtual std::string getHelp() const { return "Returns the class associated with a name for station."; }

	/* Invocation of the method from the language */
	virtual LQX::SymbolAutoRef invoke(LQX::Environment* env, std::vector<LQX::SymbolAutoRef >& args) {

	    /* Decode the name of the class and look it up in cache */
	    LQX::LanguageObject* lo = decodeObject(args, 0);
	    const std::string className = decodeString(args, 1);
	    LQXStation* station = dynamic_cast<LQXStation *>(lo);

	    /* Make sure that what we have is a station */
	    if ( !station ) {
		throw LQX::RuntimeException("No class specified with name `%s'.", className.c_str());
		return LQX::Symbol::encodeNull();
	    }

	    /* Obtain the class from the station */
	    const BCMP::Model::Station* domStation = station->getDOMStation();
	    const BCMP::Model::Station::Class* classObject = &domStation->classAt(className);
	    return LQX::Symbol::encodeObject(new LQXClass(classObject), false);
	}
    };
}

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
#pragma mark -

namespace BCMP {

    void RegisterBindings(LQX::Environment* env, const Model* model)
    {
	LQX::MethodTable* mt = env->getMethodTable();
	mt->registerMethod(new LQXGetClass(model));
	mt->registerMethod(new LQXGetStation(model));
    }
}