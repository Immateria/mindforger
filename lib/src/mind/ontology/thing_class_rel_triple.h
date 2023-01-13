/*
 thing_class_rel_triple.h     MindForger thinking notebook

 Copyright (C) 2016-2023 Martin Dvorak <martin.dvorak@mindforger.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef M8R_THING_CLASS_REL_TRIPLE_H_
#define M8R_THING_CLASS_REL_TRIPLE_H_

#include <string>
#include <set>

#include "../../debug.h"
#include "../../config/color.h"
#include "../../gear/string_utils.h"
#include "../../gear/datetime_utils.h"

/*
 * Thing, Class, Relationship, RelationshipType and Triple
 * share the same header file to simplify compilation units
 * dependencies for the compiler.
 */

namespace m8r {

class Relationship;

/**
 * @brief Ontology Thing.
 *
 * See m8r::Ontology.
 */
class Thing
{
private:
    static long sequence;

public:
    static std::string getNextKey() { return std::to_string(++sequence); }

protected:
    /**
     * @brief Thing identifier.
     */
    std::string key;

    /**
     * @brief Display name.
     */
    std::string name;

    /**
     * @brief Relationships.
     *
     * Explicit relationships (both incoming and outgoing) distinquished
     * using subject.
     */
    std::set<Relationship*> relationships;

    /*
     * Transient fields
     */

    // name used for autolinking
    std::string autolinkingName;
    std::string autolinkingAbbr;
    // autolinking: abbrev (if exists), name otherwise
    std::string autolinkingAlias;

public:
    Thing();
    explicit Thing(const std::string name);
    Thing(const Thing&) = delete;
    Thing(const Thing&&) = delete;
    Thing& operator=(const Thing&) = delete;
    Thing& operator=(const Thing&&) = delete;
    virtual ~Thing();

    /**
     * @brief Get think key.
     *
     * Key might be set on get.
     *
     * @return unique thing identifier.
     */
    virtual std::string& getKey() { return key; }

    const std::string& getName() const { return name; }
    virtual void setName(const std::string& name) { this->name = name; autolinkName(); }
    const std::string& getAutolinkingName() const { return autolinkingName; }
    const std::string& getAutolinkingAbbr() const { return autolinkingAbbr; }
    const std::string& getAutolinkingAlias() const { return autolinkingAlias; }

    const std::set<Relationship*> getRelationships() const { return relationships; }
    size_t getRelationshipsCount() const { return relationships.size(); }

protected:
    void autolinkName();
};

/**
 * @brief Thing which exists in time.
 */
class ThingInTime : public Thing {
protected:
    time_t created;
    time_t read;
    time_t modified;

public:
    explicit ThingInTime();
    explicit ThingInTime(const std::string name);
    ThingInTime(const ThingInTime&) = delete;
    ThingInTime(const ThingInTime&&) = delete;
    ThingInTime& operator=(const ThingInTime&) = delete;
    ThingInTime& operator=(const ThingInTime&&) = delete;
    virtual ~ThingInTime();

    virtual time_t getCreated() const;
    virtual void setCreated();
    virtual void setCreated(time_t created);

    virtual time_t getModified() const;
    virtual void setModified();
    virtual void setModified(time_t modified);

};

/**
 * @brief Ontology class.
 *
 * See m8r::Ontology.
 */
class Clazz : public Thing
{
public:
    static constexpr const auto ROOT_CLASS = nullptr;

private:
    /**
     * @brief Parent class in ISA hierarchy.
     *
     * All Classes are organized to ISA (C++ is-a programming language) hierarchy.
     * The root of ISA hierarchy is the Thing (instance of m8r::Class) defined by Ontology.
     */
    Clazz* isA;

public:
    explicit Clazz(const std::string& name, Clazz* isA);
    Clazz(const Clazz&) = delete;
    Clazz(const Clazz&&) = delete;
    Clazz &operator=(const Clazz&) = delete;
    Clazz &operator=(const Clazz&&) = delete;
    ~Clazz();

    Clazz* getIsA() const { return isA; }
    void setIsA(Clazz* isA) { this->isA = isA; }
};

/**
 * @brief Ontology reationship type.
 *
 * See m8r::Ontology.
 */
class RelationshipType : public Clazz
{
private:
    bool reflexive; // Thing op Thing (==,>=,*1) tj. *1(T) = T, ==(T) = T, ...
    bool symetric;
    bool transitive;

    const Color& color;

public:
    // static initialization order fiasco prevention
    static const std::string& KeyIsA() {
        static const std::string KEY_IS_A= std::string{"is a"};
        return KEY_IS_A;
    }
    static const std::string& KeySameAs() {
        static const std::string KEY_SAME_AS = std::string{"same as"};
        return KEY_SAME_AS;
    }
    static const std::string& KeyOppositeOf() {
        static const std::string KEY_OPPOSITE_OF= std::string{"opposite of"};
        return KEY_OPPOSITE_OF;
    }
    static const std::string& KeyDependsOn() {
        static const std::string KEY_DEPENDS_ON= std::string{"depends on"};
        return KEY_DEPENDS_ON;
    }

public:
    RelationshipType() = delete;
    explicit RelationshipType(const std::string& name, Clazz* isA, const Color& color);
    RelationshipType(const RelationshipType&) = delete;
    RelationshipType(const RelationshipType&&) = delete;
    RelationshipType &operator=(const RelationshipType&) = delete;
    RelationshipType &operator=(const RelationshipType&&) = delete;
    ~RelationshipType();

    bool isReflexive() const { return reflexive; }
    void setReflexive(bool r) { reflexive = r; }
    bool isTransitive() const { return transitive; }
    void setTransitive(bool t) { transitive = t; }
    bool isSymetric() const { return symetric; }
    void setSymetric(bool s) { symetric = s; }

    const Color& getColor() const { return color; }
};

/**
 * @brief Ontology triple.
 *
 * Subject -predicate-> Object
 *
 * See m8r::Ontology.
 */
class Triple : public Thing
{
protected:
    const Thing* subject;
    const RelationshipType* predicate;
    const Thing* object;

public:
    explicit Triple(const std::string name, const Thing* subject, const RelationshipType* predicate, const Thing* object);
    Triple(const Triple&) = delete;
    Triple(const Triple&&) = delete;
    Triple &operator=(const Triple&) = delete;
    Triple &operator=(const Triple&&) = delete;
    ~Triple();

    const Thing* getSubject() const { return subject; }
    void setSubject(const Thing* subject) { this->subject = subject; }
    const RelationshipType* getPredicate() const { return predicate; }
    void setPredicate(const RelationshipType* predicate) { this->predicate = predicate; }
    const Thing* getObject() const { return object; }
    void setObject(const Thing* object) { this->object = object; }
};

/**
 * @brief Ontology reationship.
 *
 * See m8r::Ontology.
 */
// IMPROVE consider Relationship and Triple to be template
class Relationship : public Triple
{
public:
    /**
     * @brief Triple constructor - every Relationship is a Triple.
     */
    explicit Relationship(const std::string name, Thing* subject, RelationshipType* predicate, Thing* object);
    Relationship(const Relationship&) = delete;
    Relationship(const Relationship&&) = delete;
    Relationship &operator=(const Relationship&) = delete;
    Relationship &operator=(const Relationship&&) = delete;
    ~Relationship();
};

} // m8r namespace

#endif /* M8R_THING_CLASS_REL_TRIPLE_H_ */
