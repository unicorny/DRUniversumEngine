<?php
$delimeter = "/";
if(PHP_OS == "WINNT") $delimeter = "\\";
if(count($argv) == 1)
{
	//var_dump($argv);
	echo "zu wenig Parameter\n";
	echo "Bitte gebe einen Objektnamen an:\n";	
	die("\n");
}

function writeHeader($openFile, $namespace, $objectName)
{
	$lizenz = 
	"/*/*************************************************************************\n"
   ."*                                                                         *\n"
   ."* UniversumLib, collection of classes for generating and go through a     *\n"
   ."* whole universe. It is for my Gameproject Spacecraft					   *\n" 
   ."* Copyright (C) 2014, 2015, 2016, 2017 Dario Rekowski.					   *\n"
   ."* Email: dario.rekowski@gmx.de   Web: www.spacecrafting.de                *\n"
   ."*                                                                         *\n"
   ."* This program is free software: you can redistribute it and/or modify    *\n"
   ."* it under the terms of the GNU General Public License as published by    *\n"
   ."* the Free Software Foundation, either version 3 of the License, or       *\n"
   ."* any later version.													   *\n"
   ."*																		   *\n"
   ."* This program is distributed in the hope that it will be useful,	       *\n"
   ."* but WITHOUT ANY WARRANTY; without even the implied warranty of	       *\n"
   ."* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	       *\n"
   ."* GNU General Public License for more details.							   *\n"
   ."*																		   *\n"
   ."* You should have received a copy of the GNU General Public License	   *\n"
   ."* along with this program.  If not, see <http://www.gnu.org/licenses/>.   *\n"
   ."*                                                                         *\n"
   ."***************************************************************************/";
	 $define = "__DR_UNIVERSUM_LIB_".strtoupper($namespace)."_".strtoupper($objectName)."_H__";
	 fprintf($openFile, "%s\n\n", $lizenz);
	 fprintf($openFile, "#ifndef %s\n#define %s\n\n", $define, $define);
	 fprintf($openFile, "#include \"UniversumLib.h\"\n\n");
	 fprintf($openFile, "namespace UniLib {\n\tnamespace $namespace {\n\t\tclass UNIVERSUM_LIB_API $objectName\n");
	 fprintf($openFile, "\t\t{\n\t\tpublic:\n\t\t\t$objectName();\n\t\t\t~$objectName();\n\t\tprivate:\n\t\t};\n\t};\n};\n");
	 fprintf($openFile, "\n#endif //$define");
	 
 };
 function writeSrc($openFile, $namespace, $objectName)
 {
	fprintf($openFile, "#include \"UniversumLib.h\"\n\n");
	fprintf($openFile, "namespace UniLib {\n\tnamespace $namespace {\n\n\t\t$objectName::$objectName()\n\t\t{\n\t\t}\n");
	fprintf($openFile, "\n\t\t//------------------------------------------------------------\n\n");
	fprintf($openFile, "\t\t$objectName::~$objectName()\n\t\t{\n\t\t}\n\n\t};\n};");
 }
 $namespaces = array("controller", "generator", "model", "view");

 foreach($namespaces as $namespace) {
	$handle1 = fopen("..".$delimeter."src".$delimeter.$namespace.$delimeter.$argv[1].".cpp", "wt");
	$handle2 = fopen("..".$delimeter."include".$delimeter.$namespace.$delimeter.$argv[1].".h", "wt");
	writeSrc($handle1, $namespace, $argv[1]);
	writeHeader($handle2, $namespace, $argv[1]);
 };

