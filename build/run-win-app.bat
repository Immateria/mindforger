@echo off
rem MindForger thinking notebook

rem Copyright (C) 2016-2025

rem This program is free software; you can redistribute it and/or
rem modify it under the terms of the GNU General Public License
rem as published by the Free Software Foundation; either version 2
rem of the License, or (at your option) any later version.

rem This program is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
rem GNU General Public License for more details.

rem You should have received a copy of the GNU General Public License
rem along with this program. If not, see <http://www.gnu.org/licenses/>.

rem This script copies nice and safe Outlines from personal repository
rem to demo repository used for creation of screenshots and videos.
@echo on

setlocal
call "%~dp0%\env.bat"
echo on
set "MF_APP=%MF_BASE%\app\release\mindforger.exe"
if not exist "%MF_APP%" goto :err
start "" "%MF_APP%"
goto :end
:err
echo cannot find '%MF_APP%'
pause
:end

