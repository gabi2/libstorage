/*
 * Copyright (c) [2004-2009] Novell, Inc.
 *
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, contact Novell, Inc.
 *
 * To contact Novell about this file by physical or electronic mail, you may
 * find current contact information at www.novell.com.
 */


#ifndef STORAGE_TYPES_H
#define STORAGE_TYPES_H


#include <string>
#include <vector>
#include <ostream>
#include <boost/algorithm/string.hpp>

#include "storage/Regex.h"
#include "storage/AppUtil.h"
#include "storage/StorageInterface.h"


namespace storage
{
    using std::string;
    using std::vector;


inline bool operator<(CType a, CType b)
{
    static const int order[COTYPE_LAST_ENTRY] = {
	0, // CUNKNOWN
	1, // DISK
	5, // MD
	8, // LOOP
	7, // LVM
	6, // DM
	2, // DMRAID
	9, // NFSC
	3, // DMMULTIPATH
	4  // MDPART
    };

    bool ret = order[a] < order[b];
    y2mil("a:" << a << " o(a):" << order[a] << " b:" << b << " o(b):" << order[b] << " ret:" << ret);
    return ret;
}

inline bool operator<=( CType a, CType b )
    {
    return( a==b || a<b );
    }

inline bool operator>=( CType a, CType b )
    {
    return( !(a<b) );
    }

inline bool operator>( CType a, CType b )
    {
    return( a!=b && !(a<b) );
    }

struct contOrder
    {
    contOrder(CType t) : order(0)
	{
	if( t==LOOP )
	    order=1;
	}
    operator unsigned() const { return( order ); }
    protected:
	unsigned order;
    };


    enum CommitStage { DECREASE, INCREASE, FORMAT, MOUNT };


    class Volume;
    class Container;

    struct commitAction
    {
	commitAction(CommitStage s, CType t, const Text& d, const Volume* v,
		     bool destr = false)
	    : stage(s), type(t), description(d), destructive(destr), container(false), u(v)
	{
	}

	commitAction(CommitStage s, CType t, const Text& d, const Container* c,
		     bool destr = false)
	    : stage(s), type(t), description(d), destructive(destr), container(true), u(c)
	{
	}

	commitAction(CommitStage s, CType t, const Volume* v)
	    : stage(s), type(t), description(), destructive(false), container(false), u(v)
	{
	}

	commitAction(CommitStage s, CType t, const Container* c)
	    : stage(s), type(t), description(), destructive(false), container(true), u(c)
	{
	}

	const CommitStage stage;
	const CType type;
	const Text description;
	const bool destructive;
	const bool container;

	const union U
	{
	    U(const Volume* v) : vol(v) {}
	    U(const Container* c) : co(c) {}

	    const Volume* vol;
	    const Container* co;
	} u;

	const Container* co() const { return container ? u.co : NULL; }
	const Volume* vol() const { return container ? NULL : u.vol; }

	bool operator==(const commitAction& rhs) const
	    { return stage == rhs.stage && type == rhs.type; }
	bool operator<(const commitAction& rhs) const;
	bool operator<=(const commitAction& rhs) const
	    { return *this < rhs || *this == rhs; }
	bool operator>=(const commitAction& rhs) const
	    { return !(*this < rhs); }
	bool operator>(const commitAction& rhs) const
	    { return !(*this < rhs && *this == rhs); }

	friend std::ostream& operator<<(std::ostream& s, const commitAction& a);
    };


    struct stage_is
    {
	stage_is(CommitStage t) : val(t) {}
	bool operator()(const commitAction& t) const { return t.stage == val; }
	const CommitStage val;
    };


    class UsedBy
    {

    public:

	UsedBy(UsedByType type, const string& device) : ub_type(type), ub_device(device) {}

	bool operator==(const UsedBy& rhs) const
	    { return ub_type == rhs.ub_type && ub_device == rhs.ub_device; }
	bool operator!=(const UsedBy& rhs) const
	    { return !(*this == rhs); }

	UsedByType type() const { return ub_type; }
	string device() const { return ub_device; }

	friend std::ostream& operator<<(std::ostream&, const UsedBy&);

	operator UsedByInfo() const { return UsedByInfo(ub_type, ub_device); }

    private:

	UsedByType ub_type;
	string ub_device;

    };


    struct regex_matches
    {
	regex_matches(const Regex& t) : val(t) {}
	bool operator()(const string& s) const { return val.match(s); }
	const Regex& val;
    };

    struct string_starts_with
    {
	string_starts_with(const string& t) : val(t) {}
	bool operator()(const string& s) const { return boost::starts_with(s, val); }
	const string& val;
    };

    struct string_contains
    {
	string_contains(const string& t) : val(t) {}
	bool operator()(const string& s) const { return boost::contains(s, val); }
	const string& val;
    };


    template <class Pred>
    vector<string>::const_iterator
    find_if(const vector<string>& lines, Pred pred)
    {
	return std::find_if(lines.begin(), lines.end(), pred);
    }

}


#endif
