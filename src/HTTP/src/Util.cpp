//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#include "Sourcey/HTTP/Util.h"
#include "Sourcey/Util.h"
#include "Poco/String.h"


using namespace std;
using namespace Poco;


namespace scy {
namespace util {


string parseURI(const string& request) 
{
	string req = request;
	string value = "";
	string::size_type start, end = 0;
	toUpper(req);
	start = req.find(" ");
	if (start != string::npos) {
		start++;
		end = req.find(" HTTP", start);
		if (end == string::npos) end = req.find(" RTSP", start);
		if (end == string::npos) return "";
		value = request.substr(start, end-start);
	}
	return value;
}


bool matchURI(const std::string& uri, const std::string& expression) 
{
	string::size_type index = uri.find("?");
	return util::matchNodes(uri.substr(0, index), expression, "/");
}


string parseHeader(const string& request, const string& name) 
{
	string req = request;
	toLower(req);
	toLower(name);
	string value = "";
	string::size_type start, end = 0;
	start = req.find(name+": ");
	if (start != string::npos) {
		start += name.length() + 2;
		end = req.find("\r\n", start);
		if (end == string::npos) return "";
		value = request.substr(start, end-start);
		replaceInPlace(value,"\"","");
		replaceInPlace(value,"\r","");
		replaceInPlace(value,"\n","");
	}
	return value;
}


std::string parseCookieItem(const std::string& cookie, const std::string& item)
{
	string::size_type start, end = 0;
	start = cookie.find(item + "=");
	if (start != string::npos) {
		start += item.size() + 1;
		end = cookie.find(";", start);
		return cookie.substr(start, end-start);
	}
	return "";
}


bool parseURIQuery(const string& uri, Poco::Net::NameValueCollection& out)
{
	size_t len = uri.length();
	size_t i = 0;

	// Parse REST parameters
	//i++;
	while (i < len && uri[i] != '?') {	
		i++; 

		string value = "";	
		while (uri[i] != '/' && uri[i] != '?' && i < len)
			value += uri[i++];

		// REST parameters are referenced by index
		if (!value.empty())
			out.set(toString(out.size()), value);		
	}
	
	// Parse query parameters
	if (uri[i] == '?') i++;
	while (i < len)
	{
		string name = "";
		while (uri[i] != '=' && i < len)
			name += uri[i++];
		i++;
		string value = "";
		while (uri[i] != '&' && i < len)
			value += uri[i++];
		i++;
		
		if (!name.empty() && !value.empty())
			out.set(name, value);
	}

	return out.size() > 0;
}


} // namespace util
} // namespace scy