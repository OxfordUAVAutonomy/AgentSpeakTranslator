/*
Copyright (C) 2015, Samuel Bucheli, Department of Computer Science, University
of Oxford.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AUTOPILOT_TYPES_H
#define AUTOPILOT_TYPES_H

typedef const char* messaget;

typedef struct {
  double x;
  double y;
  double z;
  double yaw;
} positiont;

typedef struct {
  double roll;
  double pitch;
  double yaw;
  double gaz;
} control_commandt;

#endif /* AUTOPILOT_TYPES_H */
