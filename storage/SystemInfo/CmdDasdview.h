/*
 * Copyright (c) [2004-2014] Novell, Inc.
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


#ifndef CMD_DASDVIEW_H
#define CMD_DASDVIEW_H

#include "storage/StorageInterface.h"
#include "storage/Geometry.h"
#include "storage/Dasd.h"


namespace storage
{
    class SystemCmd;


    class Dasdview
    {

    public:

	Dasdview(const string& device, bool do_probe = true);

	void probe();

	friend std::ostream& operator<<(std::ostream& s, const Dasdview& dasdview);

	const Geometry& getGeometry() const { return geometry; }
	storage::DasdFormat getDasdFormat() const { return dasd_format; }
	storage::DasdType getDasdType() const { return dasd_type; }

	void parse(const vector<string>& lines);

    private:

	string device;
	Geometry geometry;
	storage::DasdFormat dasd_format;
	storage::DasdType dasd_type;

    };

}

#endif
