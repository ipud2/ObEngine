//Author : Sygmei
//Key : 976938ef7d46c286a2027d73f3a99467bcfa8ff0c1e10bd0016139744ef5404f4eb4d069709f9831f6de74a094944bf0f1c5bf89109e9855290336a66420376f

#pragma once

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <memory>
#include <functional>
#include <cmath>
#include <ctype.h>

#include <aube/ErrorHandler.hpp>

#include "Functions.hpp"

namespace vili
{
	void LoadErrors(std::string errorFile);

	namespace Types
	{
		enum AttributeType
		{
			Attribute = 0x0,
			ContainerAttribute = 0x1,
			BaseAttribute = 0x2,
			ListAttribute = 0x3,
			ComplexAttribute = 0x4,
			LinkAttribute = 0x5
		};

		enum DataType
		{
			String = 0x0,
			Int = 0x1,
			Float = 0x2,
			Bool = 0x3,
			Range = 0x4,
			Link = 0x5,
			Template = 0x6,
			Unknown = 0x7
		};

		Types::DataType stringToDataType(std::string type);
		Types::DataType getVarType(std::string var);
		std::string getDefaultValueForType(Types::DataType type);
		std::string dataTypeToString(Types::DataType type);
		std::string attributeTypeToString(Types::AttributeType type);
	}

	std::vector<std::string> convertPath(std::string path);

	class ContainerAttribute;
	//Attribute
	class Attribute
	{
	protected:
		std::string m_id;
		Types::AttributeType m_type = Types::Attribute;
		std::string m_annotation = "";
		ContainerAttribute* m_parent = nullptr;
		virtual void removeParent(ContainerAttribute* currentParent);
		virtual ContainerAttribute* getParent();
		friend class ContainerAttribute;
		friend class LinkAttribute;
		friend void LoadErrors(std::string errorFile);
	public:
		const static Types::AttributeType ClassType = Types::Attribute;
		Attribute(ContainerAttribute* parent, const std::string& id, const Types::AttributeType& type);
		virtual ~Attribute() {}
		
		virtual void setAnnotation(const std::string& annotation);
		virtual std::string getAnnotation();
		virtual std::string getID();
		virtual Types::AttributeType getType();
		virtual void setParent(ContainerAttribute* parent);
		virtual std::string getNodePath();
		unsigned int getDepth();
		virtual void setID(const std::string& id);
		virtual void copy(ContainerAttribute* newParent, std::string newid = "") = 0;
		virtual void write(std::ofstream* file, unsigned int depth) = 0;
	};

	//ContainerAttribute
	class ContainerAttribute : public Attribute
	{
	public:
		const static Types::AttributeType ClassType = Types::ContainerAttribute;
		ContainerAttribute(ContainerAttribute* parent, const std::string& id, const Types::AttributeType& type);
		virtual Attribute* removeOwnership(Attribute* element);
		virtual ~ContainerAttribute() {}
		virtual Attribute* extractElement(Attribute* element) = 0;
		void copy(ContainerAttribute* newParent, std::string newid = "") override = 0;
		void write(std::ofstream* file, unsigned int depth) override = 0;
	};

	//BaseAttribute
	class BaseAttribute : public Attribute
	{
	protected:
		std::string m_data;
		Types::DataType m_dataType;
		friend std::ostream& operator<<(std::ostream& stream, const BaseAttribute& attribute);
	public:
		const static Types::AttributeType ClassType = Types::BaseAttribute;
		BaseAttribute(ContainerAttribute* parent, const std::string& id, const Types::DataType& dataType, const std::string& data);
		BaseAttribute(const std::string& id, const Types::DataType& dataType, const std::string& data);
		virtual ~BaseAttribute() {}
		std::string returnData() const;
		void set(int var);
		void set(double var);
		void set(const std::string& var);
		void set(bool var);
		void autoset(std::string rawData);
		Types::DataType getDataType() const;
		template <class T> T get() { }
		void copy(ContainerAttribute* newParent, std::string newid = "") override;
		void write(std::ofstream* file, unsigned int depth) override;
		operator std::string();
		operator int();
		operator double();
		operator bool();
	};
	std::ostream& operator<<(std::ostream& stream, const BaseAttribute& attribute);

	//LinkAttribute
	class ComplexAttribute;
	class LinkAttribute : public Attribute
	{
		private:
			std::string m_path = "";
		public:
			const static Types::AttributeType ClassType = Types::LinkAttribute;
			LinkAttribute(ComplexAttribute* parent, const std::string& id, const std::string& path);
			Attribute* getTarget();
			template <class T> T get() { }
			std::string getPath() const;
			std::string getFullPath() const;
			void apply();
			bool operator==(LinkAttribute compare) const;
			void copy(ContainerAttribute* newParent, std::string newid = "") override;
			void write(std::ofstream* file, unsigned int depth) override;
	};
	
	//ListAttribute
	class ListAttribute : public ContainerAttribute
	{
	private:
		std::vector<std::unique_ptr<BaseAttribute>> m_dataList;
		std::vector<BaseAttribute*> m_iteratorCache;
	public:
		const static Types::AttributeType ClassType = Types::ListAttribute;
		ListAttribute(ContainerAttribute* parent, const std::string& id);
		ListAttribute(const std::string& id);
		virtual ~ListAttribute() {}
		unsigned int size() const;
		BaseAttribute* get(unsigned int index);
		std::vector<BaseAttribute*>::iterator begin();
		std::vector<BaseAttribute*>::iterator end();
		void push(const std::string& element);
		void push(const int& element);
		void push(const bool& element);
		void push(const double& element);
		void insert(unsigned int index, const std::string& element);
		void insert(unsigned int index, const int& element);
		void insert(unsigned int index, const bool& element);
		void insert(unsigned int index, const double& element);
		void clear();
		void erase(unsigned int index);
		Attribute* extractElement(Attribute* element) override;
		void copy(ContainerAttribute* newParent, std::string newid = "") override;
		void write(std::ofstream* file, unsigned int depth) override;
	};

	//ComplexAttribute
	class ComplexAttribute : public ContainerAttribute
	{
		private:
			ComplexAttribute* m_parent = nullptr;
		protected:
			std::map<std::string, std::unique_ptr<Attribute>> m_childAttributes;
			std::vector<std::string> m_childAttributesNames;
		public:
			const static Types::AttributeType ClassType = Types::ComplexAttribute;
			ComplexAttribute(ComplexAttribute* parent, const std::string& id);
			ComplexAttribute(ComplexAttribute* parent, const std::string& id, ComplexAttribute* herit);
			ComplexAttribute(ComplexAttribute* parent, const std::string& id, std::vector<ComplexAttribute*>* multipleHerit);
			ComplexAttribute(const std::string& id);
			ComplexAttribute(const std::string& id, ComplexAttribute* herit);
			ComplexAttribute(const std::string& id, std::vector<ComplexAttribute*>* multipleHerit);
			virtual ~ComplexAttribute() {}

			Attribute* extractElement(Attribute* element) override;
			void heritage(ComplexAttribute* heritTarget);

			ComplexAttribute* getPath(std::string attributePath);
			ComplexAttribute& operator[](const std::string& cPath);

			ComplexAttribute* at(std::string cPath);
			template<class T, class ...Args>
			T* at(const std::string& cPath, Args ...pathParts);
			template<class ...Args>
			ComplexAttribute* at(const std::string& cPath, Args ...pathParts);
			template<class T> T* at(std::string cPath) { }

			Attribute* get(const std::string& attributeID);
			BaseAttribute* getBaseAttribute(const std::string& attributeID);
			ComplexAttribute* getComplexAttribute(const std::string& attributeID);
			ListAttribute* getListAttribute(const std::string& attributeID);
			LinkAttribute* getLinkAttribute(const std::string& attributeID);

			Types::AttributeType getAttributeType(const std::string& id);
			std::vector<std::string> getAll(Types::AttributeType searchType = Types::Attribute);

			bool contains(const std::string& attributeID);
			bool contains(Types::AttributeType searchType, const std::string& attributeID);

			void createBaseAttribute(const std::string& attributeID, const Types::DataType& type, const std::string& data);
			void createBaseAttribute(const std::string& attributeID, const std::string& data);
			void createBaseAttribute(const std::string& attributeID, bool data);
			void createBaseAttribute(const std::string& attributeID, int data);
			void createBaseAttribute(const std::string& attributeID, double data);
			void pushBaseAttribute(BaseAttribute* attribute);

			void createListAttribute(const std::string& attributeID);
			void pushListAttribute(ListAttribute* attribute);

			void createComplexAttribute(const std::string& attributeID);
			void pushComplexAttribute(ComplexAttribute* attribute);

			void createLinkAttribute(const std::string& attributeID, const std::string& path);
			void pushLinkAttribute(LinkAttribute* attribute);

			void deleteBaseAttribute(const std::string& attributeID, bool freeMemory = false);
			void deleteComplexAttribute(const std::string& attributeID, bool freeMemory = false);
			void deleteListAttribute(const std::string& attributeID, bool freeMemory = false);
			void deleteLinkAttribute(const std::string& attributeID, bool freeMemory = false);

			void write(std::ofstream* file, unsigned int depth = 0) override;
			void copy(ContainerAttribute* newParent, std::string newid = "") override;
			void walk(std::function<void(ComplexAttribute*)> walkFunction);
	};

	class AttributeConstraintManager
	{
		private:
			LinkAttribute m_attribute;
			std::vector<std::function<bool(BaseAttribute*)>> m_constraints;
		public:
			AttributeConstraintManager(ComplexAttribute* parent, std::string path);
			void addConstraint(std::function<bool(BaseAttribute*)> constraint);
			bool checkAllConstraints();
			LinkAttribute* getLinkAttribute();
			std::string getArgumentPath() const;
	};

	class DataTemplate
	{
		private:
			std::vector<AttributeConstraintManager> m_signature;
			ComplexAttribute m_body;
			bool m_signatureEnd = false;
			bool m_defaultLinkRoot = false;
			bool checkSignature();
		public:
			DataTemplate();
			ComplexAttribute* getBody();
			void build(ComplexAttribute* parent, const std::string& id);
			void addConstraintManager(AttributeConstraintManager constraintManager, bool facultative = false);
			void useDefaultLinkRoot();
	};

	//DataParser
	class DataParser
	{
	private:
		std::unique_ptr<ComplexAttribute> m_root = nullptr;
		std::map<std::string, DataTemplate*> m_templateList;
		std::vector<std::string> m_flagList;
		ComplexAttribute* getPath(std::string path) const;
		ComplexAttribute* getRootChild(std::string child) const;
	public:
		DataParser();
		DataParser(std::string file);
		ComplexAttribute* operator->() const;
		void createFlag(const std::string& flag);
		ComplexAttribute& operator[](std::string cPath) const;
		ComplexAttribute* at(std::string cPath) const;
		template<class ...Args>
		ComplexAttribute* at(const std::string& cPath, Args ...pathParts);
		template<class T, class ...Args>
		T* at(const std::string& cPath, Args ...pathParts);
		bool parseFile(const std::string& filename, bool verbose = false);
		void writeFile(const std::string& filename, bool verbose = false);
		bool hasFlag(const std::string& flagName);
		unsigned int getAmountOfFlags() const;
		std::string getFlagAtIndex(int index);
	};
}

namespace vili
{
	//BaseAttribute
	template <> inline int BaseAttribute::get() {
		if (m_dataType == Types::Int)
			return std::stoi(m_data);
		if (m_dataType == Types::Float)
			return std::stod(m_data);
		throw aube::ErrorHandler::Raise("Vili.ViliHeader.BaseAttribute.WrongIntCast", { {"path", getNodePath()}, {"type", dataTypeToString(m_dataType)} });
	}
	template <> inline double BaseAttribute::get() {
		if (m_dataType == Types::Float)
			return std::stod(m_data);
		if (m_dataType == Types::Int)
			return std::round(std::stod(m_data));
		throw aube::ErrorHandler::Raise("Vili.ViliHeader.BaseAttribute.WrongFloatCast", { { "path", getNodePath() }, { "type", dataTypeToString(m_dataType) } });
	}
	template <> inline bool BaseAttribute::get() {
		if (m_dataType == Types::Bool)
			return (m_data == "True") ? true : false;
		throw aube::ErrorHandler::Raise("Vili.ViliHeader.BaseAttribute.WrongBoolCast", { { "path", getNodePath() }, { "type", dataTypeToString(m_dataType) } });
	}
	template <> inline std::string BaseAttribute::get() {
		if (m_dataType == Types::String)
			return m_data;
		throw aube::ErrorHandler::Raise("Vili.ViliHeader.BaseAttribute.WrongStringCast", { { "path", getNodePath() }, { "type", dataTypeToString(m_dataType) } });
	}

	//LinkAttribute
	template <> inline BaseAttribute* LinkAttribute::get() {
		if (getTarget()->getType() == Types::BaseAttribute)
			return static_cast<BaseAttribute*>(getTarget());
		throw aube::ErrorHandler::Raise("Vili.ViliHeader.LinkAttribute.WrongBaseAttributeLink", { {"path", getNodePath()}, {"targetpath", m_path} });
	}

	template <> inline ComplexAttribute* LinkAttribute::get() {
		if (getTarget()->getType() == Types::ComplexAttribute)
			return static_cast<ComplexAttribute*>(getTarget());
		throw aube::ErrorHandler::Raise("Vili.ViliHeader.LinkAttribute.WrongComplexAttributeLink", { { "path", getNodePath() }, { "targetpath", m_path } });
	}
	template <> inline ListAttribute* LinkAttribute::get() {
		if (getTarget()->getType() == Types::ListAttribute)
			return static_cast<ListAttribute*>(getTarget());
		throw aube::ErrorHandler::Raise("Vili.ViliHeader.LinkAttribute.WrongListAttributeLink", { { "path", getNodePath() }, { "targetpath", m_path } });
	}

	//ComplexAttribute
	template<class ...Args>
	inline ComplexAttribute* ComplexAttribute::at(const std::string& cPath, Args ...pathParts)
	{
		return getPath(cPath)->at(pathParts...);
	}
	template<class T, class ...Args>
	T* ComplexAttribute::at(const std::string& cPath, Args ...pathParts)
	{
		return getPath(cPath)->at<T>(pathParts...);
	}
	template <> inline ComplexAttribute* ComplexAttribute::at(std::string cPath)
	{
		return getComplexAttribute(cPath);
	}
	template <> inline ListAttribute* ComplexAttribute::at(std::string cPath)
	{
		return getListAttribute(cPath);
	}
	template <> inline BaseAttribute* ComplexAttribute::at(std::string cPath)
	{
		return getBaseAttribute(cPath);
	}
	template <> inline LinkAttribute* ComplexAttribute::at(std::string cPath)
	{
		return getLinkAttribute(cPath);
	}

	//DataParser
	template<class ...Args>
	ComplexAttribute* DataParser::at(const std::string& cPath, Args ...pathParts)
	{
		return m_root->at(cPath, pathParts...);
	}
	template<class T, class ...Args>
	T* DataParser::at(const std::string& cPath, Args ...pathParts)
	{
		return m_root->at<T>(cPath, pathParts...);
	}
}