///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// TetGen                                                                    //
//                                                                           //
// A Quality Tetrahedral Mesh Generator and A 3D Delaunay Triangulator       //
//                                                                           //
// Version 1.5                                                               //
// May 31, 2014                                                              //
//                                                                           //
// Copyright (C) 2002--2014                                                  //
//                                                                           //
// TetGen is freely available through the website: http://www.tetgen.org.    //
//   It may be copied, modified, and redistributed for non-commercial use.   //
//   Please consult the file LICENSE for the detailed copyright notices.     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "tetgen.h"

extern void myprintf( const char* format, ... );

#pragma warning( disable : 4703 )
#pragma warning( disable : 4996 )

//// io_cxx ///////////////////////////////////////////////////////////////////
////                                                                       ////
////                                                                       ////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// readline()   Read a nonempty line from a file.                            //
//                                                                           //
// A line is considered "nonempty" if it contains something more than white  //
// spaces.  If a line is considered empty, it will be dropped and the next   //
// line will be read, this process ends until reaching the end-of-file or a  //
// non-empty line.  Return NULL if it is the end-of-file, otherwise, return  //
// a pointer to the first non-whitespace character of the line.              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

char* tetgenio::readline( char *string, FILE *infile, int *linenumber )
{
	char *result;

	// Search for a non-empty line.
	do
	{
		result = fgets( string, INPUTLINESIZE - 1, infile );
		if(linenumber) ( *linenumber )++;
		if(result == (char *)NULL)
		{
			return (char *)NULL;
		}
		// Skip white spaces.
		while(( *result == ' ' ) || ( *result == '\t' )) result++;
		// If it's end of line, read another line and try again.
	}
	while(( *result == '\0' ) || ( *result == '\r' ) || ( *result == '\n' ));
	return result;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// findnextfield()   Find the next field of a string.                        //
//                                                                           //
// Jumps past the current field by searching for whitespace or a comma, then //
// jumps past the whitespace or the comma to find the next field.            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

char* tetgenio::findnextfield( char *string )
{
	char *result;

	result = string;
	// Skip the current field.  Stop upon reaching whitespace or a comma.
	while(( *result != '\0' ) && ( *result != ' ' ) && ( *result != '\t' ) &&
		   ( *result != ',' ) && ( *result != ';' ))
	{
		result++;
	}
	// Now skip the whitespace or the comma, stop at anything else that looks
	//   like a character, or the end of a line. 
	while(( *result == ' ' ) || ( *result == '\t' ) || ( *result == ',' ) ||
		   ( *result == ';' ))
	{
		result++;
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// readnumberline()   Read a nonempty number line from a file.               //
//                                                                           //
// A line is considered "nonempty" if it contains something that looks like  //
// a number.  Comments (prefaced by `#') are ignored.                        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

char* tetgenio::readnumberline( char *string, FILE *infile, char *infilename )
{
	char *result;

	// Search for something that looks like a number.
	do
	{
		result = fgets( string, INPUTLINESIZE, infile );
		if(result == (char *)NULL)
		{
			return result;
		}
		// Skip anything that doesn't look like a number, a comment, 
		//   or the end of a line. 
		while(( *result != '\0' ) && ( *result != '#' )
			   && ( *result != '.' ) && ( *result != '+' ) && ( *result != '-' )
			   && ( ( *result < '0' ) || ( *result > '9' ) ))
		{
			result++;
		}
		// If it's a comment or end of line, read another line and try again.
	}
	while(( *result == '#' ) || ( *result == '\0' ));
	return result;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// findnextnumber()   Find the next field of a number string.                //
//                                                                           //
// Jumps past the current field by searching for whitespace or a comma, then //
// jumps past the whitespace or the comma to find the next field that looks  //
// like a number.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

char* tetgenio::findnextnumber( char *string )
{
	char *result;

	result = string;
	// Skip the current field.  Stop upon reaching whitespace or a comma.
	while(( *result != '\0' ) && ( *result != '#' ) && ( *result != ' ' ) &&
		   ( *result != '\t' ) && ( *result != ',' ))
	{
		result++;
	}
	// Now skip the whitespace and anything else that doesn't look like a
	//   number, a comment, or the end of a line. 
	while(( *result != '\0' ) && ( *result != '#' )
		   && ( *result != '.' ) && ( *result != '+' ) && ( *result != '-' )
		   && ( ( *result < '0' ) || ( *result > '9' ) ))
	{
		result++;
	}
	// Check for a comment (prefixed with `#').
	if(*result == '#')
	{
		*result = '\0';
	}
	return result;
}

////                                                                       ////
////                                                                       ////
//// io_cxx ///////////////////////////////////////////////////////////////////


//// behavior_cxx /////////////////////////////////////////////////////////////
////                                                                       ////
////                                                                       ////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// syntax()    Print list of command line switches.                          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenbehavior::syntax( )
{
	myprintf( "  tetgen [-pYrq_Aa_miO_S_T_XMwcdzfenvgkJBNEFICQVh] input_file\n" );
	myprintf( "    -p  Tetrahedralizes a piecewise linear complex (PLC).\n" );
	myprintf( "    -Y  Preserves the input surface mesh (does not modify it).\n" );
	myprintf( "    -r  Reconstructs a previously generated mesh.\n" );
	myprintf( "    -q  Refines mesh (to improve mesh quality).\n" );
	myprintf( "    -R  Mesh coarsening (to reduce the mesh elements).\n" );
	myprintf( "    -A  Assigns attributes to tetrahedra in different regions.\n" );
	myprintf( "    -a  Applies a maximum tetrahedron volume constraint.\n" );
	myprintf( "    -m  Applies a mesh sizing function.\n" );
	myprintf( "    -i  Inserts a list of additional points.\n" );
	myprintf( "    -O  Specifies the level of mesh optimization.\n" );
	myprintf( "    -S  Specifies maximum number of added points.\n" );
	myprintf( "    -T  Sets a tolerance for coplanar test (default 1e-8).\n" );
	myprintf( "    -X  Suppresses use of exact arithmetic.\n" );
	myprintf( "    -M  No merge of coplanar facets or very close vertices.\n" );
	myprintf( "    -w  Generates weighted Delaunay (regular) triangulation.\n" );
	myprintf( "    -c  Retains the convex hull of the PLC.\n" );
	myprintf( "    -d  Detects self-intersections of facets of the PLC.\n" );
	myprintf( "    -z  Numbers all output items starting from zero.\n" );
	myprintf( "    -f  Outputs all faces to .face file.\n" );
	myprintf( "    -e  Outputs all edges to .edge file.\n" );
	myprintf( "    -n  Outputs tetrahedra neighbors to .neigh file.\n" );
	myprintf( "    -v  Outputs Voronoi diagram to files.\n" );
	myprintf( "    -g  Outputs mesh to .mesh file for viewing by Medit.\n" );
	myprintf( "    -k  Outputs mesh to .vtk file for viewing by Paraview.\n" );
	myprintf( "    -J  No jettison of unused vertices from output .node file.\n" );
	myprintf( "    -B  Suppresses output of boundary information.\n" );
	myprintf( "    -N  Suppresses output of .node file.\n" );
	myprintf( "    -E  Suppresses output of .ele file.\n" );
	myprintf( "    -F  Suppresses output of .face and .edge file.\n" );
	myprintf( "    -I  Suppresses mesh iteration numbers.\n" );
	myprintf( "    -C  Checks the consistency of the final mesh.\n" );
	myprintf( "    -Q  Quiet:  No terminal output except errors.\n" );
	myprintf( "    -V  Verbose:  Detailed information, more terminal output.\n" );
	myprintf( "    -h  Help:  A brief instruction for using TetGen.\n" );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// usage()    Print a brief instruction for using TetGen.                    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenbehavior::usage( )
{
	myprintf( "TetGen\n" );
	myprintf( "A Quality Tetrahedral Mesh Generator and 3D Delaunay " );
	myprintf( "Triangulator\n" );
	myprintf( "Version 1.5\n" );
	myprintf( "May 31, 2014\n" );
	myprintf( "\n" );
	myprintf( "Copyright (C) 2002 - 2014\n" );
	myprintf( "\n" );
	myprintf( "What Can TetGen Do?\n" );
	myprintf( "\n" );
	myprintf( "  TetGen generates Delaunay tetrahedralizations, constrained\n" );
	myprintf( "  Delaunay tetrahedralizations, and quality tetrahedral meshes.\n" );
	myprintf( "\n" );
	myprintf( "Command Line Syntax:\n" );
	myprintf( "\n" );
	myprintf( "  Below is the basic command line syntax of TetGen with a list of " );
	myprintf( "short\n" );
	myprintf( "  descriptions. Underscores indicate that numbers may optionally\n" );
	myprintf( "  follow certain switches.  Do not leave any space between a " );
	myprintf( "switch\n" );
	myprintf( "  and its numeric parameter.  \'input_file\' contains input data\n" );
	myprintf( "  depending on the switches you supplied which may be a " );
	myprintf( "  piecewise\n" );
	myprintf( "  linear complex or a list of nodes.  File formats and detailed\n" );
	myprintf( "  description of command line switches are found in user's " );
	myprintf( "manual.\n" );
	myprintf( "\n" );
	syntax( );
	myprintf( "\n" );
	myprintf( "Examples of How to Use TetGen:\n" );
	myprintf( "\n" );
	myprintf( "  \'tetgen object\' reads vertices from object.node, and writes " );
	myprintf( "their\n  Delaunay tetrahedralization to object.1.node, " );
	myprintf( "object.1.ele\n  (tetrahedra), and object.1.face" );
	myprintf( " (convex hull faces).\n" );
	myprintf( "\n" );
	myprintf( "  \'tetgen -p object\' reads a PLC from object.poly or object." );
	myprintf( "smesh (and\n  possibly object.node) and writes its constrained " );
	myprintf( "Delaunay\n  tetrahedralization to object.1.node, object.1.ele, " );
	myprintf( "object.1.face,\n" );
	myprintf( "  (boundary faces) and object.1.edge (boundary edges).\n" );
	myprintf( "\n" );
	myprintf( "  \'tetgen -pq1.414a.1 object\' reads a PLC from object.poly or\n" );
	myprintf( "  object.smesh (and possibly object.node), generates a mesh " );
	myprintf( "whose\n  tetrahedra have radius-edge ratio smaller than 1.414 and " );
	myprintf( "have volume\n  of 0.1 or less, and writes the mesh to " );
	myprintf( "object.1.node, object.1.ele,\n  object.1.face, and object.1.edge\n" );
	myprintf( "\n" );
	myprintf( "Please send bugs/comments to Hang Si <si@wias-berlin.de>\n" );
	terminatetetgen( NULL, 0 );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// parse_commandline()    Read the command line, identify switches, and set  //
//                        up options and file names.                         //
//                                                                           //
// 'argc' and 'argv' are the same parameters passed to the function main()   //
// of a C/C++ program. They together represent the command line user invoked //
// from an environment in which TetGen is running.                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

bool tetgenbehavior::parse_commandline( int argc, char **argv )
{
	int startindex;
	int increment;
	int meshnumber;
	int i, j, k;
	char workstring[ 1024 ];

	// First determine the input style of the switches.
	if(argc == 0)
	{
		startindex = 0;                    // Switches are given without a dash.
		argc = 1;                    // For running the following for-loop once.
		commandline[ 0 ] = '\0';
	}
	else
	{
		startindex = 1;
		strcpy( commandline, argv[ 0 ] );
		strcat( commandline, " " );
	}

	for(i = startindex; i < argc; i++)
	{
		// Remember the command line for output.
		strcat( commandline, argv[ i ] );
		strcat( commandline, " " );
		if(startindex == 1)
		{
			// Is this string a filename?
			if(argv[ i ][ 0 ] != '-')
			{
				strncpy( infilename, argv[ i ], 1024 - 1 );
				infilename[ 1024 - 1 ] = '\0';
				continue;
			}
		}
		// Parse the individual switch from the string.
		for(j = startindex; argv[ i ][ j ] != '\0'; j++)
		{
			if(argv[ i ][ j ] == 'p')
			{
				plc = 1;
				if(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
					( argv[ i ][ j + 1 ] == '.' ))
				{
					k = 0;
					while(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
						   ( argv[ i ][ j + 1 ] == '.' ))
					{
						j++;
						workstring[ k ] = argv[ i ][ j ];
						k++;
					}
					workstring[ k ] = '\0';
					facet_separate_ang_tol = (REAL)strtod( workstring, (char **)NULL );
				}
				if(( argv[ i ][ j + 1 ] == '/' ) || ( argv[ i ][ j + 1 ] == ',' ))
				{
					j++;
					if(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
						( argv[ i ][ j + 1 ] == '.' ))
					{
						k = 0;
						while(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
							   ( argv[ i ][ j + 1 ] == '.' ) || ( argv[ i ][ j + 1 ] == 'e' ) ||
							   ( argv[ i ][ j + 1 ] == '-' ) || ( argv[ i ][ j + 1 ] == '+' ))
						{
							j++;
							workstring[ k ] = argv[ i ][ j ];
							k++;
						}
						workstring[ k ] = '\0';
						facet_overlap_ang_tol = (REAL)strtod( workstring, (char **)NULL );
					}
				}
				if(( argv[ i ][ j + 1 ] == '/' ) || ( argv[ i ][ j + 1 ] == ',' ))
				{
					j++;
					if(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
						( argv[ i ][ j + 1 ] == '.' ))
					{
						k = 0;
						while(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
							   ( argv[ i ][ j + 1 ] == '.' ))
						{
							j++;
							workstring[ k ] = argv[ i ][ j ];
							k++;
						}
						workstring[ k ] = '\0';
						facet_small_ang_tol = (REAL)strtod( workstring, (char **)NULL );
					}
				}
			}
			else if(argv[ i ][ j ] == 's')
			{
				psc = 1;
			}
			else if(argv[ i ][ j ] == 'Y')
			{
				nobisect = 1;
				if(( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ))
				{
					nobisect_nomerge = ( argv[ i ][ j + 1 ] - '0' );
					j++;
				}
				if(( argv[ i ][ j + 1 ] == '/' ) || ( argv[ i ][ j + 1 ] == ',' ))
				{
					j++;
					if(( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ))
					{
						supsteiner_level = ( argv[ i ][ j + 1 ] - '0' );
						j++;
					}
				}
				if(( argv[ i ][ j + 1 ] == '/' ) || ( argv[ i ][ j + 1 ] == ',' ))
				{
					j++;
					if(( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ))
					{
						addsteiner_algo = ( argv[ i ][ j + 1 ] - '0' );
						j++;
					}
				}
			}
			else if(argv[ i ][ j ] == 'r')
			{
				refine = 1;
			}
			else if(argv[ i ][ j ] == 'q')
			{
				quality = 1;
				if(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
					( argv[ i ][ j + 1 ] == '.' ))
				{
					k = 0;
					while(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
						   ( argv[ i ][ j + 1 ] == '.' ))
					{
						j++;
						workstring[ k ] = argv[ i ][ j ];
						k++;
					}
					workstring[ k ] = '\0';
					minratio = (REAL)strtod( workstring, (char **)NULL );
				}
				if(( argv[ i ][ j + 1 ] == '/' ) || ( argv[ i ][ j + 1 ] == ',' ))
				{
					j++;
					if(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
						( argv[ i ][ j + 1 ] == '.' ))
					{
						k = 0;
						while(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
							   ( argv[ i ][ j + 1 ] == '.' ))
						{
							j++;
							workstring[ k ] = argv[ i ][ j ];
							k++;
						}
						workstring[ k ] = '\0';
						mindihedral = (REAL)strtod( workstring, (char **)NULL );
					}
				}
			}
			else if(argv[ i ][ j ] == 'R')
			{
				coarsen = 1;
				if(( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ))
				{
					coarsen_param = ( argv[ i ][ j + 1 ] - '0' );
					j++;
				}
				if(( argv[ i ][ j + 1 ] == '/' ) || ( argv[ i ][ j + 1 ] == ',' ))
				{
					j++;
					if(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
						( argv[ i ][ j + 1 ] == '.' ))
					{
						k = 0;
						while(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
							   ( argv[ i ][ j + 1 ] == '.' ))
						{
							j++;
							workstring[ k ] = argv[ i ][ j ];
							k++;
						}
						workstring[ k ] = '\0';
						coarsen_percent = (REAL)strtod( workstring, (char **)NULL );
					}
				}
			}
			else if(argv[ i ][ j ] == 'w')
			{
				weighted = 1;
				if(( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ))
				{
					weighted_param = ( argv[ i ][ j + 1 ] - '0' );
					j++;
				}
			}
			else if(argv[ i ][ j ] == 'b')
			{
				// -b(brio_threshold/brio_ratio/hilbert_limit/hilbert_order)
				brio_hilbert = 1;
				if(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
					( argv[ i ][ j + 1 ] == '.' ))
				{
					k = 0;
					while(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
						   ( argv[ i ][ j + 1 ] == '.' ))
					{
						j++;
						workstring[ k ] = argv[ i ][ j ];
						k++;
					}
					workstring[ k ] = '\0';
					brio_threshold = (int)strtol( workstring, (char **)&workstring, 0 );
				}
				if(( argv[ i ][ j + 1 ] == '/' ) || ( argv[ i ][ j + 1 ] == ',' ))
				{
					j++;
					if(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
						( argv[ i ][ j + 1 ] == '.' ))
					{
						k = 0;
						while(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
							   ( argv[ i ][ j + 1 ] == '.' ))
						{
							j++;
							workstring[ k ] = argv[ i ][ j ];
							k++;
						}
						workstring[ k ] = '\0';
						brio_ratio = (REAL)strtod( workstring, (char **)NULL );
					}
				}
				if(( argv[ i ][ j + 1 ] == '/' ) || ( argv[ i ][ j + 1 ] == ',' ))
				{
					j++;
					if(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
						( argv[ i ][ j + 1 ] == '.' ) || ( argv[ i ][ j + 1 ] == '-' ))
					{
						k = 0;
						while(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
							   ( argv[ i ][ j + 1 ] == '.' ) || ( argv[ i ][ j + 1 ] == '-' ))
						{
							j++;
							workstring[ k ] = argv[ i ][ j ];
							k++;
						}
						workstring[ k ] = '\0';
						hilbert_limit = (int)strtol( workstring, (char **)&workstring, 0 );
					}
				}
				if(( argv[ i ][ j + 1 ] == '/' ) || ( argv[ i ][ j + 1 ] == ',' ))
				{
					j++;
					if(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
						( argv[ i ][ j + 1 ] == '.' ) || ( argv[ i ][ j + 1 ] == '-' ))
					{
						k = 0;
						while(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
							   ( argv[ i ][ j + 1 ] == '.' ) || ( argv[ i ][ j + 1 ] == '-' ))
						{
							j++;
							workstring[ k ] = argv[ i ][ j ];
							k++;
						}
						workstring[ k ] = '\0';
						hilbert_order = (REAL)strtod( workstring, (char **)NULL );
					}
				}
				if(brio_threshold == 0)
				{ // -b0
					brio_hilbert = 0; // Turn off BRIO-Hilbert sorting. 
				}
				if(brio_ratio >= 1.0)
				{ // -b/1
					no_sort = 1;
					brio_hilbert = 0; // Turn off BRIO-Hilbert sorting.
				}
			}
			else if(argv[ i ][ j ] == 'l')
			{
				incrflip = 1;
			}
			else if(argv[ i ][ j ] == 'L')
			{
				flipinsert = 1;
			}
			else if(argv[ i ][ j ] == 'm')
			{
				metric = 1;
			}
			else if(argv[ i ][ j ] == 'a')
			{
				if(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
					( argv[ i ][ j + 1 ] == '.' ))
				{
					fixedvolume = 1;
					k = 0;
					while(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
						   ( argv[ i ][ j + 1 ] == '.' ) || ( argv[ i ][ j + 1 ] == 'e' ) ||
						   ( argv[ i ][ j + 1 ] == '-' ) || ( argv[ i ][ j + 1 ] == '+' ))
					{
						j++;
						workstring[ k ] = argv[ i ][ j ];
						k++;
					}
					workstring[ k ] = '\0';
					maxvolume = (REAL)strtod( workstring, (char **)NULL );
				}
				else
				{
					varvolume = 1;
				}
			}
			else if(argv[ i ][ j ] == 'A')
			{
				regionattrib = 1;
			}
			else if(argv[ i ][ j ] == 'D')
			{
				cdtrefine = 1;
				if(( argv[ i ][ j + 1 ] >= '1' ) && ( argv[ i ][ j + 1 ] <= '3' ))
				{
					reflevel = ( argv[ i ][ j + 1 ] - '1' ) + 1;
					j++;
				}
			}
			else if(argv[ i ][ j ] == 'i')
			{
				insertaddpoints = 1;
			}
			else if(argv[ i ][ j ] == 'd')
			{
				diagnose = 1;
			}
			else if(argv[ i ][ j ] == 'c')
			{
				convex = 1;
			}
			else if(argv[ i ][ j ] == 'M')
			{
				nomergefacet = 1;
				nomergevertex = 1;
				if(( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '1' ))
				{
					nomergefacet = ( argv[ i ][ j + 1 ] - '0' );
					j++;
				}
				if(( argv[ i ][ j + 1 ] == '/' ) || ( argv[ i ][ j + 1 ] == ',' ))
				{
					j++;
					if(( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '1' ))
					{
						nomergevertex = ( argv[ i ][ j + 1 ] - '0' );
						j++;
					}
				}
			}
			else if(argv[ i ][ j ] == 'X')
			{
				if(argv[ i ][ j + 1 ] == '1')
				{
					nostaticfilter = 1;
					j++;
				}
				else
				{
					noexact = 1;
				}
			}
			else if(argv[ i ][ j ] == 'z')
			{
				if(argv[ i ][ j + 1 ] == '1')
				{  // -z1
					reversetetori = 1;
					j++;
				}
				else
				{
					zeroindex = 1; // -z
				}
			}
			else if(argv[ i ][ j ] == 'f')
			{
				facesout++;
			}
			else if(argv[ i ][ j ] == 'e')
			{
				edgesout++;
			}
			else if(argv[ i ][ j ] == 'n')
			{
				neighout++;
			}
			else if(argv[ i ][ j ] == 'v')
			{
				voroout = 1;
			}
			else if(argv[ i ][ j ] == 'g')
			{
				meditview = 1;
			}
			else if(argv[ i ][ j ] == 'k')
			{
				vtkview = 1;
			}
			else if(argv[ i ][ j ] == 'J')
			{
				nojettison = 1;
			}
			else if(argv[ i ][ j ] == 'B')
			{
				nobound = 1;
			}
			else if(argv[ i ][ j ] == 'N')
			{
				nonodewritten = 1;
			}
			else if(argv[ i ][ j ] == 'E')
			{
				noelewritten = 1;
			}
			else if(argv[ i ][ j ] == 'F')
			{
				nofacewritten = 1;
			}
			else if(argv[ i ][ j ] == 'I')
			{
				noiterationnum = 1;
			}
			else if(argv[ i ][ j ] == 'S')
			{
				if(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
					( argv[ i ][ j + 1 ] == '.' ))
				{
					k = 0;
					while(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
						   ( argv[ i ][ j + 1 ] == '.' ) || ( argv[ i ][ j + 1 ] == 'e' ) ||
						   ( argv[ i ][ j + 1 ] == '-' ) || ( argv[ i ][ j + 1 ] == '+' ))
					{
						j++;
						workstring[ k ] = argv[ i ][ j ];
						k++;
					}
					workstring[ k ] = '\0';
					steinerleft = (int)strtol( workstring, (char **)NULL, 0 );
				}
			}
			else if(argv[ i ][ j ] == 'o')
			{
				if(argv[ i ][ j + 1 ] == '2')
				{
					order = 2;
					j++;
				}
				if(( argv[ i ][ j + 1 ] == '/' ) || ( argv[ i ][ j + 1 ] == ',' ))
				{
					j++;
					if(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
						( argv[ i ][ j + 1 ] == '.' ))
					{
						k = 0;
						while(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
							   ( argv[ i ][ j + 1 ] == '.' ))
						{
							j++;
							workstring[ k ] = argv[ i ][ j ];
							k++;
						}
						workstring[ k ] = '\0';
						optmaxdihedral = (REAL)strtod( workstring, (char **)NULL );
					}
				}
			}
			else if(argv[ i ][ j ] == 'O')
			{
				if(( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ))
				{
					optlevel = ( argv[ i ][ j + 1 ] - '0' );
					j++;
				}
				if(( argv[ i ][ j + 1 ] == '/' ) || ( argv[ i ][ j + 1 ] == ',' ))
				{
					j++;
					if(( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '7' ))
					{
						optscheme = ( argv[ i ][ j + 1 ] - '0' );
						j++;
					}
				}
			}
			else if(argv[ i ][ j ] == 'T')
			{
				if(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
					( argv[ i ][ j + 1 ] == '.' ))
				{
					k = 0;
					while(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
						   ( argv[ i ][ j + 1 ] == '.' ) || ( argv[ i ][ j + 1 ] == 'e' ) ||
						   ( argv[ i ][ j + 1 ] == '-' ) || ( argv[ i ][ j + 1 ] == '+' ))
					{
						j++;
						workstring[ k ] = argv[ i ][ j ];
						k++;
					}
					workstring[ k ] = '\0';
					epsilon = (REAL)strtod( workstring, (char **)NULL );
				}
			}
			else if(argv[ i ][ j ] == 'C')
			{
				docheck++;
			}
			else if(argv[ i ][ j ] == 'Q')
			{
				quiet = 1;
			}
			else if(argv[ i ][ j ] == 'V')
			{
				verbose++;
			}
			else if(argv[ i ][ j ] == 'x')
			{
				if(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
					( argv[ i ][ j + 1 ] == '.' ))
				{
					k = 0;
					while(( ( argv[ i ][ j + 1 ] >= '0' ) && ( argv[ i ][ j + 1 ] <= '9' ) ) ||
						   ( argv[ i ][ j + 1 ] == '.' ) || ( argv[ i ][ j + 1 ] == 'e' ) ||
						   ( argv[ i ][ j + 1 ] == '-' ) || ( argv[ i ][ j + 1 ] == '+' ))
					{
						j++;
						workstring[ k ] = argv[ i ][ j ];
						k++;
					}
					workstring[ k ] = '\0';
					tetrahedraperblock = (int)strtol( workstring, (char **)NULL, 0 );
					if(tetrahedraperblock > 8188)
					{
						vertexperblock = tetrahedraperblock / 2;
						shellfaceperblock = vertexperblock / 2;
					}
					else
					{
						tetrahedraperblock = 8188;
					}
				}
			}
			else if(( argv[ i ][ j ] == 'h' ) || ( argv[ i ][ j ] == 'H' ) ||
					 ( argv[ i ][ j ] == '?' ))
			{
				usage( );
			}
			else
			{
				myprintf( "Warning:  Unknown switch -%c.\n", argv[ i ][ j ] );
			}
		}
	}

	if(startindex == 0)
	{
		// Set a temporary filename for debugging output.
		strcpy( infilename, "tetgen-tmpfile" );
	}
	else
	{
		if(infilename[ 0 ] == '\0')
		{
			// No input file name. Print the syntax and exit.
			syntax( );
			terminatetetgen( NULL, 0 );
		}
		// Recognize the object from file extension if it is available.
		if(!strcmp( &infilename[ strlen( infilename ) - 5 ], ".node" ))
		{
			infilename[ strlen( infilename ) - 5 ] = '\0';
			object = NODES;
		}
		else if(!strcmp( &infilename[ strlen( infilename ) - 5 ], ".poly" ))
		{
			infilename[ strlen( infilename ) - 5 ] = '\0';
			object = POLY;
			plc = 1;
		}
		else if(!strcmp( &infilename[ strlen( infilename ) - 6 ], ".smesh" ))
		{
			infilename[ strlen( infilename ) - 6 ] = '\0';
			object = POLY;
			plc = 1;
		}
		else if(!strcmp( &infilename[ strlen( infilename ) - 4 ], ".off" ))
		{
			infilename[ strlen( infilename ) - 4 ] = '\0';
			object = OFF;
			plc = 1;
		}
		else if(!strcmp( &infilename[ strlen( infilename ) - 4 ], ".ply" ))
		{
			infilename[ strlen( infilename ) - 4 ] = '\0';
			object = PLY;
			plc = 1;
		}
		else if(!strcmp( &infilename[ strlen( infilename ) - 4 ], ".stl" ))
		{
			infilename[ strlen( infilename ) - 4 ] = '\0';
			object = STL;
			plc = 1;
		}
		else if(!strcmp( &infilename[ strlen( infilename ) - 5 ], ".mesh" ))
		{
			infilename[ strlen( infilename ) - 5 ] = '\0';
			object = MEDIT;
			if(!refine) plc = 1;
		}
		else if(!strcmp( &infilename[ strlen( infilename ) - 4 ], ".vtk" ))
		{
			infilename[ strlen( infilename ) - 4 ] = '\0';
			object = VTK;
			plc = 1;
		}
		else if(!strcmp( &infilename[ strlen( infilename ) - 4 ], ".ele" ))
		{
			infilename[ strlen( infilename ) - 4 ] = '\0';
			object = MESH;
			refine = 1;
		}
	}

	if(nobisect && ( !plc && !refine ))
	{ // -Y
		plc = 1; // Default -p option.
	}
	if(quality && ( !plc && !refine ))
	{ // -q
		plc = 1; // Default -p option.
	}
	if(diagnose && !plc)
	{ // -d
		plc = 1;
	}
	if(refine && !quality)
	{ // -r only
		// Reconstruct a mesh, no mesh optimization.
		optlevel = 0;
	}
	if(insertaddpoints && ( optlevel == 0 ))
	{ // with -i option
		optlevel = 2;
	}
	if(coarsen && ( optlevel == 0 ))
	{ // with -R option
		optlevel = 2;
	}

	// Detect improper combinations of switches.
	if(( refine || plc ) && weighted)
	{
		myprintf( "Error:  Switches -w cannot use together with -p or -r.\n" );
		return false;
	}

	if(convex)
	{ // -c
		if(plc && !regionattrib)
		{
			// -A (region attribute) is needed for marking exterior tets (-1).
			regionattrib = 1;
		}
	}

	// Note: -A must not used together with -r option. 
	// Be careful not to add an extra attribute to each element unless the
	//   input supports it (PLC in, but not refining a preexisting mesh).
	if(refine || !plc)
	{
		regionattrib = 0;
	}
	// Be careful not to allocate space for element area constraints that 
	//   will never be assigned any value (other than the default -1.0).
	if(!refine && !plc)
	{
		varvolume = 0;
	}
	// If '-a' or '-aa' is in use, enable '-q' option too.
	if(fixedvolume || varvolume)
	{
		if(quality == 0)
		{
			quality = 1;
			if(!plc && !refine)
			{
				plc = 1; // enable -p.
			}
		}
	}
	// No user-specified dihedral angle bound. Use default ones.
	if(!quality)
	{
		if(optmaxdihedral < 179.0)
		{
			if(nobisect)
			{  // with -Y option
				optmaxdihedral = 179.0;
			}
			else
			{ // -p only
				optmaxdihedral = 179.999;
			}
		}
		if(optminsmtdihed < 179.999)
		{
			optminsmtdihed = 179.999;
		}
		if(optminslidihed < 179.999)
		{
			optminslidihed = 179.999;
		}
	}

	increment = 0;
	strcpy( workstring, infilename );
	j = 1;
	while(workstring[ j ] != '\0')
	{
		if(( workstring[ j ] == '.' ) && ( workstring[ j + 1 ] != '\0' ))
		{
			increment = j + 1;
		}
		j++;
	}
	meshnumber = 0;
	if(increment > 0)
	{
		j = increment;
		do
		{
			if(( workstring[ j ] >= '0' ) && ( workstring[ j ] <= '9' ))
			{
				meshnumber = meshnumber * 10 + (int)( workstring[ j ] - '0' );
			}
			else
			{
				increment = 0;
			}
			j++;
		}
		while(workstring[ j ] != '\0');
	}
	if(noiterationnum)
	{
		strcpy( outfilename, infilename );
	}
	else if(increment == 0)
	{
		strcpy( outfilename, infilename );
		strcat( outfilename, ".1" );
	}
	else
	{
		workstring[ increment ] = '%';
		workstring[ increment + 1 ] = 'd';
		workstring[ increment + 2 ] = '\0';
		sprintf( outfilename, workstring, meshnumber + 1 );
	}
	// Additional input file name has the end ".a".
	strcpy( addinfilename, infilename );
	strcat( addinfilename, ".a" );
	// Background filename has the form "*.b.ele", "*.b.node", ...
	strcpy( bgmeshfilename, infilename );
	strcat( bgmeshfilename, ".b" );

	return true;
}

////                                                                       ////
////                                                                       ////
//// behavior_cxx /////////////////////////////////////////////////////////////

//// mempool_cxx //////////////////////////////////////////////////////////////
////                                                                       ////
////                                                                       ////

// Initialize fast lookup tables for mesh maniplulation primitives.

int tetgenmesh::bondtbl[ 12 ][ 12 ] = { { 0, }, };
int tetgenmesh::enexttbl[ 12 ] = { 0, };
int tetgenmesh::eprevtbl[ 12 ] = { 0, };
int tetgenmesh::enextesymtbl[ 12 ] = { 0, };
int tetgenmesh::eprevesymtbl[ 12 ] = { 0, };
int tetgenmesh::eorgoppotbl[ 12 ] = { 0, };
int tetgenmesh::edestoppotbl[ 12 ] = { 0, };
int tetgenmesh::fsymtbl[ 12 ][ 12 ] = { { 0, }, };
int tetgenmesh::facepivot1[ 12 ] = { 0, };
int tetgenmesh::facepivot2[ 12 ][ 12 ] = { { 0, }, };
int tetgenmesh::tsbondtbl[ 12 ][ 6 ] = { { 0, }, };
int tetgenmesh::stbondtbl[ 12 ][ 6 ] = { { 0, }, };
int tetgenmesh::tspivottbl[ 12 ][ 6 ] = { { 0, }, };
int tetgenmesh::stpivottbl[ 12 ][ 6 ] = { { 0, }, };

// Table 'esymtbl' takes an directed edge (version) as input, returns the
//   inversed edge (version) of it.

int tetgenmesh::esymtbl[ 12 ] = { 9, 6, 11, 4, 3, 7, 1, 5, 10, 0, 8, 2 };

// The following four tables give the 12 permutations of the set {0,1,2,3}.
//   An offset 4 is added to each element for a direct access of the points
//   in the tetrahedron data structure.

int tetgenmesh::orgpivot[ 12 ] = { 7, 7, 5, 5, 6, 4, 4, 6, 5, 6, 7, 4 };
int tetgenmesh::destpivot[ 12 ] = { 6, 4, 4, 6, 5, 6, 7, 4, 7, 7, 5, 5 };
int tetgenmesh::apexpivot[ 12 ] = { 5, 6, 7, 4, 7, 7, 5, 5, 6, 4, 4, 6 };
int tetgenmesh::oppopivot[ 12 ] = { 4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7 };

// The twelve versions correspond to six undirected edges. The following two
//   tables map a version to an undirected edge and vice versa.

int tetgenmesh::ver2edge[ 12 ] = { 0, 1, 2, 3, 3, 5, 1, 5, 4, 0, 4, 2 };
int tetgenmesh::edge2ver[ 6 ] = { 0, 1, 2, 3, 8, 5 };

// Edge versions whose apex or opposite may be dummypoint.

int tetgenmesh::epivot[ 12 ] = { 4, 5, 2, 11, 4, 5, 2, 11, 4, 5, 2, 11 };


// Table 'snextpivot' takes an edge version as input, returns the next edge
//   version in the same edge ring.

int tetgenmesh::snextpivot[ 6 ] = { 2, 5, 4, 1, 0, 3 };

// The following three tables give the 6 permutations of the set {0,1,2}.
//   An offset 3 is added to each element for a direct access of the points
//   in the triangle data structure.

int tetgenmesh::sorgpivot[ 6 ] = { 3, 4, 4, 5, 5, 3 };
int tetgenmesh::sdestpivot[ 6 ] = { 4, 3, 5, 4, 3, 5 };
int tetgenmesh::sapexpivot[ 6 ] = { 5, 5, 3, 3, 4, 4 };

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// inittable()    Initialize the look-up tables.                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::inittables( )
{
	int soffset, toffset;
	int i, j;


	// i = t1.ver; j = t2.ver;
	for(i = 0; i < 12; i++)
	{
		for(j = 0; j < 12; j++)
		{
			bondtbl[ i ][ j ] = ( j & 3 ) + ( ( ( i & 12 ) + ( j & 12 ) ) % 12 );
		}
	}


	// i = t1.ver; j = t2.ver
	for(i = 0; i < 12; i++)
	{
		for(j = 0; j < 12; j++)
		{
			fsymtbl[ i ][ j ] = ( j + 12 - ( i & 12 ) ) % 12;
		}
	}


	for(i = 0; i < 12; i++)
	{
		facepivot1[ i ] = ( esymtbl[ i ] & 3 );
	}

	for(i = 0; i < 12; i++)
	{
		for(j = 0; j < 12; j++)
		{
			facepivot2[ i ][ j ] = fsymtbl[ esymtbl[ i ] ][ j ];
		}
	}

	for(i = 0; i < 12; i++)
	{
		enexttbl[ i ] = ( i + 4 ) % 12;
		eprevtbl[ i ] = ( i + 8 ) % 12;
	}

	for(i = 0; i < 12; i++)
	{
		enextesymtbl[ i ] = esymtbl[ enexttbl[ i ] ];
		eprevesymtbl[ i ] = esymtbl[ eprevtbl[ i ] ];
	}

	for(i = 0; i < 12; i++)
	{
		eorgoppotbl[ i ] = eprevtbl[ esymtbl[ enexttbl[ i ] ] ];
		edestoppotbl[ i ] = enexttbl[ esymtbl[ eprevtbl[ i ] ] ];
	}

	// i = t.ver, j = s.shver
	for(i = 0; i < 12; i++)
	{
		for(j = 0; j < 6; j++)
		{
			if(( j & 1 ) == 0)
			{
				soffset = ( 6 - ( ( i & 12 ) >> 1 ) ) % 6;
				toffset = ( 12 - ( ( j & 6 ) << 1 ) ) % 12;
			}
			else
			{
				soffset = ( i & 12 ) >> 1;
				toffset = ( j & 6 ) << 1;
			}
			tsbondtbl[ i ][ j ] = ( j & 1 ) + ( ( ( j & 6 ) + soffset ) % 6 );
			stbondtbl[ i ][ j ] = ( i & 3 ) + ( ( ( i & 12 ) + toffset ) % 12 );
		}
	}


	// i = t.ver, j = s.shver
	for(i = 0; i < 12; i++)
	{
		for(j = 0; j < 6; j++)
		{
			if(( j & 1 ) == 0)
			{
				soffset = ( i & 12 ) >> 1;
				toffset = ( j & 6 ) << 1;
			}
			else
			{
				soffset = ( 6 - ( ( i & 12 ) >> 1 ) ) % 6;
				toffset = ( 12 - ( ( j & 6 ) << 1 ) ) % 12;
			}
			tspivottbl[ i ][ j ] = ( j & 1 ) + ( ( ( j & 6 ) + soffset ) % 6 );
			stpivottbl[ i ][ j ] = ( i & 3 ) + ( ( ( i & 12 ) + toffset ) % 12 );
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// restart()    Deallocate all objects in this pool.                         //
//                                                                           //
// The pool returns to a fresh state, like after it was initialized, except  //
// that no memory is freed to the operating system.  Rather, the previously  //
// allocated blocks are ready to be used.                                    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::arraypool::restart( )
{
	objects = 0l;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// poolinit()    Initialize an arraypool for allocation of objects.          //
//                                                                           //
// Before the pool may be used, it must be initialized by this procedure.    //
// After initialization, memory can be allocated and freed in this pool.     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::arraypool::poolinit( int sizeofobject, int log2objperblk )
{
	// Each object must be at least one byte long.
	objectbytes = sizeofobject > 1 ? sizeofobject : 1;

	log2objectsperblock = log2objperblk;
	// Compute the number of objects in each block.
	objectsperblock = ( (int)1 ) << log2objectsperblock;
	objectsperblockmark = objectsperblock - 1;

	// No memory has been allocated.
	totalmemory = 0l;
	// The top array has not been allocated yet.
	toparray = (char **)NULL;
	toparraylen = 0;

	// Ready all indices to be allocated.
	restart( );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// arraypool()    The constructor and destructor.                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

tetgenmesh::arraypool::arraypool( int sizeofobject, int log2objperblk )
{
	poolinit( sizeofobject, log2objperblk );
}

tetgenmesh::arraypool::~arraypool( )
{
	int i;

	// Has anything been allocated at all?
	if(toparray != (char **)NULL)
	{
		// Walk through the top array.
		for(i = 0; i < toparraylen; i++)
		{
			// Check every pointer; NULLs may be scattered randomly.
			if(toparray[ i ] != (char *)NULL)
			{
				// Free an allocated block.
				free( (void *)toparray[ i ] );
			}
		}
		// Free the top array.
		free( (void *)toparray );
	}

	// The top array is no longer allocated.
	toparray = (char **)NULL;
	toparraylen = 0;
	objects = 0;
	totalmemory = 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// getblock()    Return (and perhaps create) the block containing the object //
//               with a given index.                                         //
//                                                                           //
// This function takes care of allocating or resizing the top array if nece- //
// ssary, and of allocating the block if it hasn't yet been allocated.       //
//                                                                           //
// Return a pointer to the beginning of the block (NOT the object).          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

char* tetgenmesh::arraypool::getblock( int objectindex )
{
	char **newarray;
	char *block;
	int newsize;
	int topindex;
	int i;

	// Compute the index in the top array (upper bits).
	topindex = objectindex >> log2objectsperblock;
	// Does the top array need to be allocated or resized?
	if(toparray == (char **)NULL)
	{
		// Allocate the top array big enough to hold 'topindex', and NULL out
		//   its contents.
		newsize = topindex + 128;
		toparray = (char **)malloc( (size_t)( newsize * sizeof( char * ) ) );
		toparraylen = newsize;
		for(i = 0; i < newsize; i++)
		{
			toparray[ i ] = (char *)NULL;
		}
		// Account for the memory.
		totalmemory = newsize * ( uintptr_t ) sizeof( char * );
	}
	else if(topindex >= toparraylen)
	{
		// Resize the top array, making sure it holds 'topindex'.
		newsize = 3 * toparraylen;
		if(topindex >= newsize)
		{
			newsize = topindex + 128;
		}
		// Allocate the new array, copy the contents, NULL out the rest, and
		//   free the old array.
		newarray = (char **)malloc( (size_t)( newsize * sizeof( char * ) ) );
		for(i = 0; i < toparraylen; i++)
		{
			newarray[ i ] = toparray[ i ];
		}
		for(i = toparraylen; i < newsize; i++)
		{
			newarray[ i ] = (char *)NULL;
		}
		free( toparray );
		// Account for the memory.
		totalmemory += ( newsize - toparraylen ) * sizeof( char * );
		toparray = newarray;
		toparraylen = newsize;
	}

	// Find the block, or learn that it hasn't been allocated yet.
	block = toparray[ topindex ];
	if(block == (char *)NULL)
	{
		// Allocate a block at this index.
		block = (char *)malloc( (size_t)( objectsperblock * objectbytes ) );
		toparray[ topindex ] = block;
		// Account for the memory.
		totalmemory += objectsperblock * objectbytes;
	}

	// Return a pointer to the block.
	return block;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// lookup()    Return the pointer to the object with a given index, or NULL  //
//             if the object's block doesn't exist yet.                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void* tetgenmesh::arraypool::lookup( int objectindex )
{
	char *block;
	int topindex;

	// Has the top array been allocated yet?
	if(toparray == (char **)NULL)
	{
		return (void *)NULL;
	}

	// Compute the index in the top array (upper bits).
	topindex = objectindex >> log2objectsperblock;
	// Does the top index fit in the top array?
	if(topindex >= toparraylen)
	{
		return (void *)NULL;
	}

	// Find the block, or learn that it hasn't been allocated yet.
	block = toparray[ topindex ];
	if(block == (char *)NULL)
	{
		return (void *)NULL;
	}

	// Compute a pointer to the object with the given index.  Note that
	//   'objectsperblock' is a power of two, so the & operation is a bit mask
	//   that preserves the lower bits.
	return (void *)( block + ( objectindex & ( objectsperblock - 1 ) ) * objectbytes );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// newindex()    Allocate space for a fresh object from the pool.            //
//                                                                           //
// 'newptr' returns a pointer to the new object (it must not be a NULL).     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::arraypool::newindex( void **newptr )
{
	// Allocate an object at index 'firstvirgin'.
	int newindex = objects;
	*newptr = (void *)( getblock( objects ) +
						( objects & ( objectsperblock - 1 ) ) * objectbytes );
	objects++;

	return newindex;
}


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// memorypool()   The constructors of memorypool.                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

tetgenmesh::memorypool::memorypool( )
{
	firstblock = nowblock = (void **)NULL;
	nextitem = (void *)NULL;
	deaditemstack = (void *)NULL;
	pathblock = (void **)NULL;
	pathitem = (void *)NULL;
	alignbytes = 0;
	itembytes = itemwords = 0;
	itemsperblock = 0;
	items = maxitems = 0l;
	unallocateditems = 0;
	pathitemsleft = 0;
}

tetgenmesh::memorypool::memorypool( int bytecount, int itemcount, int wsize,
									int alignment )
{
	poolinit( bytecount, itemcount, wsize, alignment );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// ~memorypool()   Free to the operating system all memory taken by a pool.  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

tetgenmesh::memorypool::~memorypool( )
{
	while(firstblock != (void **)NULL)
	{
		nowblock = (void **)*( firstblock );
		free( firstblock );
		firstblock = nowblock;
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// poolinit()    Initialize a pool of memory for allocation of items.        //
//                                                                           //
// A `pool' is created whose records have size at least `bytecount'.  Items  //
// will be allocated in `itemcount'-item blocks.  Each item is assumed to be //
// a collection of words, and either pointers or floating-point values are   //
// assumed to be the "primary" word type.  (The "primary" word type is used  //
// to determine alignment of items.)  If `alignment' isn't zero, all items   //
// will be `alignment'-byte aligned in memory.  `alignment' must be either a //
// multiple or a factor of the primary word size;  powers of two are safe.   //
// `alignment' is normally used to create a few unused bits at the bottom of //
// each item's pointer, in which information may be stored.                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::memorypool::poolinit( int bytecount, int itemcount, int wordsize,
									   int alignment )
{
	// Find the proper alignment, which must be at least as large as:
	//   - The parameter `alignment'.
	//   - The primary word type, to avoid unaligned accesses.
	//   - sizeof(void *), so the stack of dead items can be maintained
	//       without unaligned accesses.
	if(alignment > wordsize)
	{
		alignbytes = alignment;
	}
	else
	{
		alignbytes = wordsize;
	}
	if(( int ) sizeof( void * ) > alignbytes)
	{
		alignbytes = ( int ) sizeof( void * );
	}
	itemwords = ( ( bytecount + alignbytes - 1 ) / alignbytes )
		* ( alignbytes / wordsize );
	itembytes = itemwords * wordsize;
	itemsperblock = itemcount;

	// Allocate a block of items.  Space for `itemsperblock' items and one
	//   pointer (to point to the next block) are allocated, as well as space
	//   to ensure alignment of the items. 
	firstblock = (void **)malloc( itemsperblock * itembytes + sizeof(void *)
								  +alignbytes );
	if(firstblock == (void **)NULL)
	{
		terminatetetgen( NULL, 1 );
	}
	// Set the next block pointer to NULL.
	*( firstblock ) = (void *)NULL;
	restart( );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// restart()   Deallocate all items in this pool.                            //
//                                                                           //
// The pool is returned to its starting state, except that no memory is      //
// freed to the operating system.  Rather, the previously allocated blocks   //
// are ready to be reused.                                                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::memorypool::restart( )
{
	uintptr_t alignptr;

	items = 0;
	maxitems = 0;

	// Set the currently active block.
	nowblock = firstblock;
	// Find the first item in the pool.  Increment by the size of (void *).
	alignptr = (uintptr_t)( nowblock + 1 );
	// Align the item on an `alignbytes'-byte boundary.
	nextitem = (void *)
		( alignptr + (uintptr_t)alignbytes -
		( alignptr % (uintptr_t)alignbytes ) );
	// There are lots of unallocated items left in this block.
	unallocateditems = itemsperblock;
	// The stack of deallocated items is empty.
	deaditemstack = (void *)NULL;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// alloc()   Allocate space for an item.                                     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void* tetgenmesh::memorypool::alloc( )
{
	void *newitem;
	void **newblock;
	uintptr_t alignptr;

	// First check the linked list of dead items.  If the list is not 
	//   empty, allocate an item from the list rather than a fresh one.
	if(deaditemstack != (void *)NULL)
	{
		newitem = deaditemstack;                     // Take first item in list.
		deaditemstack = *(void **)deaditemstack;
	}
	else
	{
		// Check if there are any free items left in the current block.
		if(unallocateditems == 0)
		{
			// Check if another block must be allocated.
			if(*nowblock == (void *)NULL)
			{
				// Allocate a new block of items, pointed to by the previous block.
				newblock = (void **)malloc( itemsperblock * itembytes + sizeof(void *)
											+alignbytes );
				if(newblock == (void **)NULL)
				{
					terminatetetgen( NULL, 1 );
				}
				*nowblock = (void *)newblock;
				// The next block pointer is NULL.
				*newblock = (void *)NULL;
			}
			// Move to the new block.
			nowblock = (void **)*nowblock;
			// Find the first item in the block.
			//   Increment by the size of (void *).
			alignptr = (uintptr_t)( nowblock + 1 );
			// Align the item on an `alignbytes'-byte boundary.
			nextitem = (void *)
				( alignptr + (uintptr_t)alignbytes -
				( alignptr % (uintptr_t)alignbytes ) );
			// There are lots of unallocated items left in this block.
			unallocateditems = itemsperblock;
		}
		// Allocate a new item.
		newitem = nextitem;
		// Advance `nextitem' pointer to next free item in block.
		nextitem = (void *)( (uintptr_t)nextitem + itembytes );
		unallocateditems--;
		maxitems++;
	}
	items++;
	return newitem;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// dealloc()   Deallocate space for an item.                                 //
//                                                                           //
// The deallocated space is stored in a queue for later reuse.               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::memorypool::dealloc( void *dyingitem )
{
	// Push freshly killed item onto stack.
	*( (void **)dyingitem ) = deaditemstack;
	deaditemstack = dyingitem;
	items--;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// traversalinit()   Prepare to traverse the entire list of items.           //
//                                                                           //
// This routine is used in conjunction with traverse().                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::memorypool::traversalinit( )
{
	uintptr_t alignptr;

	// Begin the traversal in the first block.
	pathblock = firstblock;
	// Find the first item in the block.  Increment by the size of (void *).
	alignptr = (uintptr_t)( pathblock + 1 );
	// Align with item on an `alignbytes'-byte boundary.
	pathitem = (void *)
		( alignptr + (uintptr_t)alignbytes -
		( alignptr % (uintptr_t)alignbytes ) );
	// Set the number of items left in the current block.
	pathitemsleft = itemsperblock;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// traverse()   Find the next item in the list.                              //
//                                                                           //
// This routine is used in conjunction with traversalinit().  Be forewarned  //
// that this routine successively returns all items in the list, including   //
// deallocated ones on the deaditemqueue. It's up to you to figure out which //
// ones are actually dead.  It can usually be done more space-efficiently by //
// a routine that knows something about the structure of the item.           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void* tetgenmesh::memorypool::traverse( )
{
	void *newitem;
	uintptr_t alignptr;

	// Stop upon exhausting the list of items.
	if(pathitem == nextitem)
	{
		return (void *)NULL;
	}
	// Check whether any untraversed items remain in the current block.
	if(pathitemsleft == 0)
	{
		// Find the next block.
		pathblock = (void **)*pathblock;
		// Find the first item in the block.  Increment by the size of (void *).
		alignptr = (uintptr_t)( pathblock + 1 );
		// Align with item on an `alignbytes'-byte boundary.
		pathitem = (void *)
			( alignptr + (uintptr_t)alignbytes -
			( alignptr % (uintptr_t)alignbytes ) );
		// Set the number of items left in the current block.
		pathitemsleft = itemsperblock;
	}
	newitem = pathitem;
	// Find the next item in the block.
	pathitem = (void *)( (uintptr_t)pathitem + itembytes );
	pathitemsleft--;
	return newitem;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// makeindex2pointmap()    Create a map from index to vertices.              //
//                                                                           //
// 'idx2verlist' returns the created map.  Traverse all vertices, a pointer  //
// to each vertex is set into the array.  The pointer to the first vertex is //
// saved in 'idx2verlist[in->firstnumber]'.                                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::makeindex2pointmap( point*& idx2verlist )
{
	point pointloop;
	int idx;

	if(b->verbose > 1)
	{
		myprintf( "  Constructing mapping from indices to points.\n" );
	}

	idx2verlist = new point[ points->items + 1 ];

	points->traversalinit( );
	pointloop = pointtraverse( );
	idx = in->firstnumber;
	while(pointloop != (point)NULL)
	{
		idx2verlist[ idx++ ] = pointloop;
		pointloop = pointtraverse( );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// makesubfacemap()    Create a map from vertex to subfaces incident at it.  //
//                                                                           //
// The map is returned in two arrays 'idx2faclist' and 'facperverlist'.  All //
// subfaces incident at i-th vertex (i is counted from 0) are found in the   //
// array facperverlist[j], where idx2faclist[i] <= j < idx2faclist[i + 1].   //
// Each entry in facperverlist[j] is a subface whose origin is the vertex.   //
//                                                                           //
// NOTE: These two arrays will be created inside this routine, don't forget  //
// to free them after using.                                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::makepoint2submap( memorypool* pool, int*& idx2faclist,
								   face*& facperverlist )
{
	face shloop;
	int i, j, k;

	if(b->verbose > 1)
	{
		myprintf( "  Making a map from points to subfaces.\n" );
	}

	// Initialize 'idx2faclist'.
	idx2faclist = new int[ points->items + 1 ];
	for(i = 0; i < points->items + 1; i++) idx2faclist[ i ] = 0;

	// Loop all subfaces, counter the number of subfaces incident at a vertex.
	pool->traversalinit( );
	shloop.sh = shellfacetraverse( pool );
	while(shloop.sh != (shellface *)NULL)
	{
		// Increment the number of incident subfaces for each vertex.
		j = pointmark( (point)shloop.sh[ 3 ] ) - in->firstnumber;
		idx2faclist[ j ]++;
		j = pointmark( (point)shloop.sh[ 4 ] ) - in->firstnumber;
		idx2faclist[ j ]++;
		// Skip the third corner if it is a segment.
		if(shloop.sh[ 5 ] != NULL)
		{
			j = pointmark( (point)shloop.sh[ 5 ] ) - in->firstnumber;
			idx2faclist[ j ]++;
		}
		shloop.sh = shellfacetraverse( pool );
	}

	// Calculate the total length of array 'facperverlist'.
	j = idx2faclist[ 0 ];
	idx2faclist[ 0 ] = 0;  // Array starts from 0 element.
	for(i = 0; i < points->items; i++)
	{
		k = idx2faclist[ i + 1 ];
		idx2faclist[ i + 1 ] = idx2faclist[ i ] + j;
		j = k;
	}

	// The total length is in the last unit of idx2faclist.
	facperverlist = new face[ idx2faclist[ i ] ];

	// Loop all subfaces again, remember the subfaces at each vertex.
	pool->traversalinit( );
	shloop.sh = shellfacetraverse( pool );
	while(shloop.sh != (shellface *)NULL)
	{
		j = pointmark( (point)shloop.sh[ 3 ] ) - in->firstnumber;
		shloop.shver = 0; // save the origin.
		facperverlist[ idx2faclist[ j ] ] = shloop;
		idx2faclist[ j ]++;
		// Is it a subface or a subsegment?
		if(shloop.sh[ 5 ] != NULL)
		{
			j = pointmark( (point)shloop.sh[ 4 ] ) - in->firstnumber;
			shloop.shver = 2; // save the origin.
			facperverlist[ idx2faclist[ j ] ] = shloop;
			idx2faclist[ j ]++;
			j = pointmark( (point)shloop.sh[ 5 ] ) - in->firstnumber;
			shloop.shver = 4; // save the origin.
			facperverlist[ idx2faclist[ j ] ] = shloop;
			idx2faclist[ j ]++;
		}
		else
		{
			j = pointmark( (point)shloop.sh[ 4 ] ) - in->firstnumber;
			shloop.shver = 1; // save the origin.
			facperverlist[ idx2faclist[ j ] ] = shloop;
			idx2faclist[ j ]++;
		}
		shloop.sh = shellfacetraverse( pool );
	}

	// Contents in 'idx2faclist' are shifted, now shift them back.
	for(i = points->items - 1; i >= 0; i--)
	{
		idx2faclist[ i + 1 ] = idx2faclist[ i ];
	}
	idx2faclist[ 0 ] = 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// tetrahedrondealloc()    Deallocate space for a tet., marking it dead.     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::tetrahedrondealloc( tetrahedron *dyingtetrahedron )
{
	// Set tetrahedron's vertices to NULL. This makes it possible to detect
	//   dead tetrahedra when traversing the list of all tetrahedra.
	dyingtetrahedron[ 4 ] = (tetrahedron)NULL;

	// Dealloc the space to subfaces/subsegments.
	if(dyingtetrahedron[ 8 ] != NULL)
	{
		tet2segpool->dealloc( (shellface *)dyingtetrahedron[ 8 ] );
	}
	if(dyingtetrahedron[ 9 ] != NULL)
	{
		tet2subpool->dealloc( (shellface *)dyingtetrahedron[ 9 ] );
	}

	tetrahedrons->dealloc( (void *)dyingtetrahedron );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// tetrahedrontraverse()    Traverse the tetrahedra, skipping dead ones.     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

tetgenmesh::tetrahedron* tetgenmesh::tetrahedrontraverse( )
{
	tetrahedron *newtetrahedron;

	do
	{
		newtetrahedron = (tetrahedron *)tetrahedrons->traverse( );
		if(newtetrahedron == (tetrahedron *)NULL)
		{
			return (tetrahedron *)NULL;
		}
	}
	while(( newtetrahedron[ 4 ] == (tetrahedron)NULL ) ||
	( (point)newtetrahedron[ 7 ] == dummypoint ));
	return newtetrahedron;
}

tetgenmesh::tetrahedron* tetgenmesh::alltetrahedrontraverse( )
{
	tetrahedron *newtetrahedron;

	do
	{
		newtetrahedron = (tetrahedron *)tetrahedrons->traverse( );
		if(newtetrahedron == (tetrahedron *)NULL)
		{
			return (tetrahedron *)NULL;
		}
	}
	while(newtetrahedron[ 4 ] == (tetrahedron)NULL); // Skip dead ones.
	return newtetrahedron;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// shellfacedealloc()    Deallocate space for a shellface, marking it dead.  //
//                       Used both for dealloc a subface and subsegment.     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::shellfacedealloc( memorypool *pool, shellface *dyingsh )
{
	// Set shellface's vertices to NULL. This makes it possible to detect dead
	//   shellfaces when traversing the list of all shellfaces.
	dyingsh[ 3 ] = (shellface)NULL;
	pool->dealloc( (void *)dyingsh );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// shellfacetraverse()    Traverse the subfaces, skipping dead ones. Used    //
//                        for both subfaces and subsegments pool traverse.   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

tetgenmesh::shellface* tetgenmesh::shellfacetraverse( memorypool *pool )
{
	shellface *newshellface;

	do
	{
		newshellface = (shellface *)pool->traverse( );
		if(newshellface == (shellface *)NULL)
		{
			return (shellface *)NULL;
		}
	}
	while(newshellface[ 3 ] == (shellface)NULL);          // Skip dead ones.
	return newshellface;
}


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// pointdealloc()    Deallocate space for a point, marking it dead.          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::pointdealloc( point dyingpoint )
{
	// Mark the point as dead. This  makes it possible to detect dead points
	//   when traversing the list of all points.
	setpointtype( dyingpoint, DEADVERTEX );
	points->dealloc( (void *)dyingpoint );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// pointtraverse()    Traverse the points, skipping dead ones.               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

tetgenmesh::point tetgenmesh::pointtraverse( )
{
	point newpoint;

	do
	{
		newpoint = (point)points->traverse( );
		if(newpoint == (point)NULL)
		{
			return (point)NULL;
		}
	}
	while(pointtype( newpoint ) == DEADVERTEX);            // Skip dead ones.
	return newpoint;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// maketetrahedron()    Create a new tetrahedron.                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::maketetrahedron( triface *newtet )
{
	newtet->tet = (tetrahedron *)tetrahedrons->alloc( );

	// Initialize the four adjoining tetrahedra to be "outer space".
	newtet->tet[ 0 ] = NULL;
	newtet->tet[ 1 ] = NULL;
	newtet->tet[ 2 ] = NULL;
	newtet->tet[ 3 ] = NULL;
	// Four NULL vertices.
	newtet->tet[ 4 ] = NULL;
	newtet->tet[ 5 ] = NULL;
	newtet->tet[ 6 ] = NULL;
	newtet->tet[ 7 ] = NULL;
	// No attached segments and subfaces yet.
	newtet->tet[ 8 ] = NULL;
	newtet->tet[ 9 ] = NULL;
	// Initialize the marker (clear all flags).
	setelemmarker( newtet->tet, 0 );
	for(int i = 0; i < numelemattrib; i++)
	{
		setelemattribute( newtet->tet, i, 0.0 );
	}
	if(b->varvolume)
	{
		setvolumebound( newtet->tet, -1.0 );
	}

	// Initialize the version to be Zero.
	newtet->ver = 11;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// makeshellface()    Create a new shellface with version zero. Used for     //
//                    both subfaces and subsegments.                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::makeshellface( memorypool *pool, face *newface )
{
	newface->sh = (shellface *)pool->alloc( );

	// No adjointing subfaces.
	newface->sh[ 0 ] = NULL;
	newface->sh[ 1 ] = NULL;
	newface->sh[ 2 ] = NULL;
	// Three NULL vertices.
	newface->sh[ 3 ] = NULL;
	newface->sh[ 4 ] = NULL;
	newface->sh[ 5 ] = NULL;
	// No adjoining subsegments.
	newface->sh[ 6 ] = NULL;
	newface->sh[ 7 ] = NULL;
	newface->sh[ 8 ] = NULL;
	// No adjoining tetrahedra.
	newface->sh[ 9 ] = NULL;
	newface->sh[ 10 ] = NULL;
	if(checkconstraints)
	{
		// Initialize the maximum area bound.
		setareabound( *newface, 0.0 );
	}
	// Set the boundary marker to zero.
	setshellmark( *newface, 0 );
	// Clear the infection and marktest bits.
	( (int *)( newface->sh ) )[ shmarkindex + 1 ] = 0;
	if(useinsertradius)
	{
		setfacetindex( *newface, 0 );
	}

	newface->shver = 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// makepoint()    Create a new point.                                        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::makepoint( point* pnewpoint, enum verttype vtype )
{
	int i;

	*pnewpoint = (point)points->alloc( );

	// Initialize the point attributes.
	for(i = 0; i < numpointattrib; i++)
	{
		( *pnewpoint )[ 3 + i ] = 0.0;
	}
	// Initialize the metric tensor.
	for(i = 0; i < sizeoftensor; i++)
	{
		( *pnewpoint )[ pointmtrindex + i ] = 0.0;
	}
	setpoint2tet( *pnewpoint, NULL );
	setpoint2ppt( *pnewpoint, NULL );
	if(b->plc || b->refine)
	{
		// Initialize the point-to-simplex field.
		setpoint2sh( *pnewpoint, NULL );
		if(b->metric && ( bgm != NULL ))
		{
			setpoint2bgmtet( *pnewpoint, NULL );
		}
	}
	// Initialize the point marker (starting from in->firstnumber).
	setpointmark( *pnewpoint, (int)( points->items ) - ( !in->firstnumber ) );
	// Clear all flags.
	( (int *)( *pnewpoint ) )[ pointmarkindex + 1 ] = 0;
	// Initialize (set) the point type. 
	setpointtype( *pnewpoint, vtype );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// initializepools()    Calculate the sizes of the point, tetrahedron, and   //
//                      subface. Initialize their memory pools.              //
//                                                                           //
// This routine also computes the indices 'pointmarkindex', 'point2simindex',//
// 'point2pbcptindex', 'elemattribindex', and 'volumeboundindex'.  They are  //
// used to find values within each point and tetrahedron, respectively.      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::initializepools( )
{
	int pointsize = 0, elesize = 0, shsize = 0;
	int i;

	if(b->verbose)
	{
		myprintf( "  Initializing memorypools.\n" );
		myprintf( "  tetrahedron per block: %d.\n", b->tetrahedraperblock );
	}

	inittables( );

	// There are three input point lists available, which are in, addin,
	//   and bgm->in. These point lists may have different number of 
	//   attributes. Decide the maximum number.
	numpointattrib = in->numberofpointattributes;
	if(bgm != NULL)
	{
		if(bgm->in->numberofpointattributes > numpointattrib)
		{
			numpointattrib = bgm->in->numberofpointattributes;
		}
	}
	if(addin != NULL)
	{
		if(addin->numberofpointattributes > numpointattrib)
		{
			numpointattrib = addin->numberofpointattributes;
		}
	}
	if(b->weighted || b->flipinsert)
	{ // -w or -L.
		// The internal number of point attribute needs to be at least 1
		//   (for storing point weights).
		if(numpointattrib == 0)
		{
			numpointattrib = 1;
		}
	}

	// Default varconstraint = 0;
	if(in->segmentconstraintlist || in->facetconstraintlist)
	{
		checkconstraints = 1;
	}
	if(b->plc || b->refine)
	{
		// Save the insertion radius for Steiner points if boundaries
		//   are allowed be split.
		if(!b->nobisect || checkconstraints)
		{
			useinsertradius = 1;
		}
	}

	// The index within each point at which its metric tensor is found. 
	// Each vertex has three coordinates.
	if(b->psc)
	{
		// '-s' option (PSC), the u,v coordinates are provided.
		pointmtrindex = 3 + tetgenio::pointparam::maxuvs + numpointattrib;
		// The index within each point at which its u, v coordinates are found.
		// Comment: They are saved after the list of point attributes.
		pointparamindex = pointmtrindex - tetgenio::pointparam::maxuvs;
	}
	else
	{
		pointmtrindex = 3 + numpointattrib;
	}
	// For '-m' option. A tensor field is provided (*.mtr or *.b.mtr file).
	if(b->metric)
	{
		// Decide the size (1, 3, or 6) of the metric tensor.
		if(bgm != (tetgenmesh *)NULL)
		{
			// A background mesh is allocated. It may not exist though.
			sizeoftensor = ( bgm->in != (tetgenio *)NULL ) ?
				bgm->in->numberofpointmtrs : in->numberofpointmtrs;
		}
		else
		{
			// No given background mesh - Itself is a background mesh.
			sizeoftensor = in->numberofpointmtrs;
		}
		// Make sure sizeoftensor is at least 1.
		sizeoftensor = ( sizeoftensor > 0 ) ? sizeoftensor : 1;
	}
	else
	{
		// For '-q' option. Make sure to have space for saving a scalar value.
		sizeoftensor = b->quality ? 1 : 0;
	}
	if(useinsertradius)
	{
		// Increase a space (REAL) for saving point insertion radius, it is
		//   saved directly after the metric. 
		sizeoftensor++;
	}
	pointinsradiusindex = pointmtrindex + sizeoftensor - 1;
	// The index within each point at which an element pointer is found, where
	//   the index is measured in pointers. Ensure the index is aligned to a
	//   sizeof(tetrahedron)-byte address.
	point2simindex = ( ( pointmtrindex + sizeoftensor ) * sizeof(REAL)
					   +sizeof(tetrahedron)-1 ) / sizeof( tetrahedron );
	if(b->plc || b->refine || b->voroout)
	{
		// Increase the point size by three pointers, which are:
		//   - a pointer to a tet, read by point2tet();
		//   - a pointer to a parent point, read by point2ppt()).
		//   - a pointer to a subface or segment, read by point2sh();
		if(b->metric && ( bgm != (tetgenmesh *)NULL ))
		{
			// Increase one pointer into the background mesh, point2bgmtet().
			pointsize = ( point2simindex + 4 ) * sizeof( tetrahedron );
		}
		else
		{
			pointsize = ( point2simindex + 3 ) * sizeof( tetrahedron );
		}
	}
	else
	{
		// Increase the point size by two pointer, which are:
		//   - a pointer to a tet, read by point2tet();
		//   - a pointer to a parent point, read by point2ppt()). -- Used by btree.
		pointsize = ( point2simindex + 2 ) * sizeof( tetrahedron );
	}
	// The index within each point at which the boundary marker is found,
	//   Ensure the point marker is aligned to a sizeof(int)-byte address.
	pointmarkindex = ( pointsize + sizeof(int)-1 ) / sizeof( int );
	// Now point size is the ints (indicated by pointmarkindex) plus:
	//   - an integer for boundary marker;
	//   - an integer for vertex type;
	//   - an integer for geometry tag (optional, -s option).
	pointsize = ( pointmarkindex + 2 + ( b->psc ? 1 : 0 ) ) * sizeof( tetrahedron );

	// Initialize the pool of vertices.
	points = new memorypool( pointsize, b->vertexperblock, sizeof( REAL ), 0 );

	if(b->verbose)
	{
		myprintf( "  Size of a point: %d bytes.\n", points->itembytes );
	}

	// Initialize the infinite vertex.
	dummypoint = ( point ) new char[ pointsize ];
	// Initialize all fields of this point.
	dummypoint[ 0 ] = 0.0;
	dummypoint[ 1 ] = 0.0;
	dummypoint[ 2 ] = 0.0;
	for(i = 0; i < numpointattrib; i++)
	{
		dummypoint[ 3 + i ] = 0.0;
	}
	// Initialize the metric tensor.
	for(i = 0; i < sizeoftensor; i++)
	{
		dummypoint[ pointmtrindex + i ] = 0.0;
	}
	setpoint2tet( dummypoint, NULL );
	setpoint2ppt( dummypoint, NULL );
	if(b->plc || b->psc || b->refine)
	{
		// Initialize the point-to-simplex field.
		setpoint2sh( dummypoint, NULL );
		if(b->metric && ( bgm != NULL ))
		{
			setpoint2bgmtet( dummypoint, NULL );
		}
	}
	// Initialize the point marker (starting from in->firstnumber).
	setpointmark( dummypoint, -1 ); // The unique marker for dummypoint.
	// Clear all flags.
	( (int *)( dummypoint ) )[ pointmarkindex + 1 ] = 0;
	// Initialize (set) the point type. 
	setpointtype( dummypoint, UNUSEDVERTEX ); // Does not matter.

	// The number of bytes occupied by a tetrahedron is varying by the user-
	//   specified options. The contents of the first 12 pointers are listed
	//   in the following table:
	//     [0]  |__ neighbor at f0 __|
	//     [1]  |__ neighbor at f1 __|
	//     [2]  |__ neighbor at f2 __|
	//     [3]  |__ neighbor at f3 __|
	//     [4]  |_____ vertex p0 ____|
	//     [5]  |_____ vertex p1 ____|
	//     [6]  |_____ vertex p2 ____|
	//     [7]  |_____ vertex p3 ____|
	//     [8]  |__ segments array __| (used by -p)
	//     [9]  |__ subfaces array __| (used by -p)
	//    [10]  |_____ reserved _____|
	//    [11]  |___ elem marker ____| (used as an integer)

	elesize = 12 * sizeof( tetrahedron );

	// The index to find the element markers. An integer containing varies
	//   flags and element counter. 
	if(!( sizeof( int ) <= sizeof( tetrahedron ) ) ||
		( ( sizeof( tetrahedron ) % sizeof( int ) ) ))
	{
		terminatetetgen( this, 2 );
	}
	elemmarkerindex = ( elesize - sizeof( tetrahedron ) ) / sizeof( int );

	// The actual number of element attributes. Note that if the
	//   `b->regionattrib' flag is set, an additional attribute will be added.
	numelemattrib = in->numberoftetrahedronattributes + ( b->regionattrib > 0 );

	// The index within each element at which its attributes are found, where
	//   the index is measured in REALs. 
	elemattribindex = ( elesize + sizeof(REAL)-1 ) / sizeof( REAL );
	// The index within each element at which the maximum volume bound is
	//   found, where the index is measured in REALs.
	volumeboundindex = elemattribindex + numelemattrib;
	// If element attributes or an constraint are needed, increase the number
	//   of bytes occupied by an element.
	if(b->varvolume)
	{
		elesize = ( volumeboundindex + 1 ) * sizeof( REAL );
	}
	else if(numelemattrib > 0)
	{
		elesize = volumeboundindex * sizeof( REAL );
	}


	// Having determined the memory size of an element, initialize the pool.
	tetrahedrons = new memorypool( elesize, b->tetrahedraperblock, sizeof( void * ),
								   16 );

	if(b->verbose)
	{
		myprintf( "  Size of a tetrahedron: %d (%d) bytes.\n", elesize,
				tetrahedrons->itembytes );
	}

	if(b->plc || b->refine)
	{ // if (b->useshelles) {
		// The number of bytes occupied by a subface.  The list of pointers
		//   stored in a subface are: three to other subfaces, three to corners,
		//   three to subsegments, two to tetrahedra.
		shsize = 11 * sizeof( shellface );
		// The index within each subface at which the maximum area bound is
		//   found, where the index is measured in REALs.
		areaboundindex = ( shsize + sizeof(REAL)-1 ) / sizeof( REAL );
		// If -q switch is in use, increase the number of bytes occupied by
		//   a subface for saving maximum area bound.
		if(checkconstraints)
		{
			shsize = ( areaboundindex + 1 ) * sizeof( REAL );
		}
		else
		{
			shsize = areaboundindex * sizeof( REAL );
		}
		// The index within subface at which the facet marker is found. Ensure
		//   the marker is aligned to a sizeof(int)-byte address.
		shmarkindex = ( shsize + sizeof(int)-1 ) / sizeof( int );
		// Increase the number of bytes by two or three integers, one for facet
		//   marker, one for shellface type and flags, and optionally one
		//   for storing facet index (for mesh refinement).
		shsize = ( shmarkindex + 2 + useinsertradius ) * sizeof( shellface );

		// Initialize the pool of subfaces. Each subface record is eight-byte
		//   aligned so it has room to store an edge version (from 0 to 5) in
		//   the least three bits.
		subfaces = new memorypool( shsize, b->shellfaceperblock, sizeof( void * ), 8 );

		if(b->verbose)
		{
			myprintf( "  Size of a shellface: %d (%d) bytes.\n", shsize,
					subfaces->itembytes );
		}

		// Initialize the pool of subsegments. The subsegment's record is same
		//   with subface.
		subsegs = new memorypool( shsize, b->shellfaceperblock, sizeof( void * ), 8 );

		// Initialize the pool for tet-subseg connections.
		tet2segpool = new memorypool( 6 * sizeof( shellface ), b->shellfaceperblock,
									  sizeof( void * ), 0 );
		// Initialize the pool for tet-subface connections.
		tet2subpool = new memorypool( 4 * sizeof( shellface ), b->shellfaceperblock,
									  sizeof( void * ), 0 );

		// Initialize arraypools for segment & facet recovery.
		subsegstack = new arraypool( sizeof( face ), 10 );
		subfacstack = new arraypool( sizeof( face ), 10 );
		subvertstack = new arraypool( sizeof( point ), 8 );

		// Initialize arraypools for surface point insertion/deletion.
		caveshlist = new arraypool( sizeof( face ), 8 );
		caveshbdlist = new arraypool( sizeof( face ), 8 );
		cavesegshlist = new arraypool( sizeof( face ), 4 );

		cavetetshlist = new arraypool( sizeof( face ), 8 );
		cavetetseglist = new arraypool( sizeof( face ), 8 );
		caveencshlist = new arraypool( sizeof( face ), 8 );
		caveencseglist = new arraypool( sizeof( face ), 8 );
	}

	// Initialize the pools for flips.
	flippool = new memorypool( sizeof( badface ), 1024, sizeof( void * ), 0 );
	unflipqueue = new arraypool( sizeof( badface ), 10 );

	// Initialize the arraypools for point insertion.
	cavetetlist = new arraypool( sizeof( triface ), 10 );
	cavebdrylist = new arraypool( sizeof( triface ), 10 );
	caveoldtetlist = new arraypool( sizeof( triface ), 10 );
	cavetetvertlist = new arraypool( sizeof( point ), 10 );
}

////                                                                       ////
////                                                                       ////
//// mempool_cxx //////////////////////////////////////////////////////////////

//// geom_cxx /////////////////////////////////////////////////////////////////
////                                                                       ////
////                                                                       ////

// PI is the ratio of a circle's circumference to its diameter.
REAL tetgenmesh::PIconstant = 3.14159265358979323846264338327950288419716939937510582;

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// insphere_s()    Insphere test with symbolic perturbation.                 //
//                                                                           //
// Given four points pa, pb, pc, and pd, test if the point pe lies inside or //
// outside the circumscribed sphere of the four points.                      //
//                                                                           //
// Here we assume that the 3d orientation of the point sequence {pa, pb, pc, //
// pd} is positive (NOT zero), i.e., pd lies above the plane passing through //
// points pa, pb, and pc. Otherwise, the returned sign is flipped.           //
//                                                                           //
// Return a positive value (> 0) if pe lies inside, a negative value (< 0)   //
// if pe lies outside the sphere, the returned value will not be zero.       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

REAL tetgenmesh::insphere_s( REAL* pa, REAL* pb, REAL* pc, REAL* pd, REAL* pe )
{
	REAL sign;

	sign = insphere( pa, pb, pc, pd, pe );
	if(sign != 0.0)
	{
		return sign;
	}

	// Symbolic perturbation.
	point pt[ 5 ], swappt;
	REAL oriA, oriB;
	int swaps, count;
	int n, i;

	pt[ 0 ] = pa;
	pt[ 1 ] = pb;
	pt[ 2 ] = pc;
	pt[ 3 ] = pd;
	pt[ 4 ] = pe;

	// Sort the five points such that their indices are in the increasing
	//   order. An optimized bubble sort algorithm is used, i.e., it has
	//   the worst case O(n^2) runtime, but it is usually much faster.
	swaps = 0; // Record the total number of swaps.
	n = 5;
	do
	{
		count = 0;
		n = n - 1;
		for(i = 0; i < n; i++)
		{
			if(pointmark( pt[ i ] ) > pointmark( pt[ i + 1 ] ))
			{
				swappt = pt[ i ]; pt[ i ] = pt[ i + 1 ]; pt[ i + 1 ] = swappt;
				count++;
			}
		}
		swaps += count;
	}
	while(count > 0); // Continue if some points are swapped.

	oriA = orient3d( pt[ 1 ], pt[ 2 ], pt[ 3 ], pt[ 4 ] );
	if(oriA != 0.0)
	{
		// Flip the sign if there are odd number of swaps.
		if(( swaps % 2 ) != 0) oriA = -oriA;
		return oriA;
	}

	oriB = -orient3d( pt[ 0 ], pt[ 2 ], pt[ 3 ], pt[ 4 ] );
	if(oriB == 0.0)
	{
		terminatetetgen( this, 2 );
	}
	// Flip the sign if there are odd number of swaps.
	if(( swaps % 2 ) != 0) oriB = -oriB;
	return oriB;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// orient4d_s()    4d orientation test with symbolic perturbation.           //
//                                                                           //
// Given four lifted points pa', pb', pc', and pd' in R^4,test if the lifted //
// point pe' in R^4 lies below or above the hyperplane passing through the   //
// four points pa', pb', pc', and pd'.                                       //
//                                                                           //
// Here we assume that the 3d orientation of the point sequence {pa, pb, pc, //
// pd} is positive (NOT zero), i.e., pd lies above the plane passing through //
// the points pa, pb, and pc. Otherwise, the returned sign is flipped.       //
//                                                                           //
// Return a positive value (> 0) if pe' lies below, a negative value (< 0)   //
// if pe' lies above the hyperplane, the returned value should not be zero.  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

REAL tetgenmesh::orient4d_s( REAL* pa, REAL* pb, REAL* pc, REAL* pd, REAL* pe,
							 REAL aheight, REAL bheight, REAL cheight,
							 REAL dheight, REAL eheight )
{
	REAL sign;

	sign = orient4d( pa, pb, pc, pd, pe,
					 aheight, bheight, cheight, dheight, eheight );
	if(sign != 0.0)
	{
		return sign;
	}

	// Symbolic perturbation.
	point pt[ 5 ], swappt;
	REAL oriA, oriB;
	int swaps, count;
	int n, i;

	pt[ 0 ] = pa;
	pt[ 1 ] = pb;
	pt[ 2 ] = pc;
	pt[ 3 ] = pd;
	pt[ 4 ] = pe;

	// Sort the five points such that their indices are in the increasing
	//   order. An optimized bubble sort algorithm is used, i.e., it has
	//   the worst case O(n^2) runtime, but it is usually much faster.
	swaps = 0; // Record the total number of swaps.
	n = 5;
	do
	{
		count = 0;
		n = n - 1;
		for(i = 0; i < n; i++)
		{
			if(pointmark( pt[ i ] ) > pointmark( pt[ i + 1 ] ))
			{
				swappt = pt[ i ]; pt[ i ] = pt[ i + 1 ]; pt[ i + 1 ] = swappt;
				count++;
			}
		}
		swaps += count;
	}
	while(count > 0); // Continue if some points are swapped.

	oriA = orient3d( pt[ 1 ], pt[ 2 ], pt[ 3 ], pt[ 4 ] );
	if(oriA != 0.0)
	{
		// Flip the sign if there are odd number of swaps.
		if(( swaps % 2 ) != 0) oriA = -oriA;
		return oriA;
	}

	oriB = -orient3d( pt[ 0 ], pt[ 2 ], pt[ 3 ], pt[ 4 ] );
	if(oriB == 0.0)
	{
		terminatetetgen( this, 2 );
	}
	// Flip the sign if there are odd number of swaps.
	if(( swaps % 2 ) != 0) oriB = -oriB;
	return oriB;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// tri_edge_test()    Triangle-edge intersection test.                       //
//                                                                           //
// This routine takes a triangle T (with vertices A, B, C) and an edge E (P, //
// Q) in 3D, and tests if they intersect each other.                         //
//                                                                           //
// If the point 'R' is not NULL, it lies strictly above the plane defined by //
// A, B, C. It is used in test when T and E are coplanar.                    //
//                                                                           //
// If T and E intersect each other, they may intersect in different ways. If //
// 'level' > 0, their intersection type will be reported 'types' and 'pos'.  //
//                                                                           //
// The return value indicates one of the following cases:                    //
//   - 0, T and E are disjoint.                                              //
//   - 1, T and E intersect each other.                                      //
//   - 2, T and E are not coplanar. They intersect at a single point.        //
//   - 4, T and E are coplanar. They intersect at a single point or a line   //
//        segment (if types[1] != DISJOINT).                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#define SETVECTOR3(V, a0, a1, a2) (V)[0] = (a0); (V)[1] = (a1); (V)[2] = (a2)

#define SWAP2(a0, a1, tmp) (tmp) = (a0); (a0) = (a1); (a1) = (tmp)

int tetgenmesh::tri_edge_2d( point A, point B, point C, point P, point Q,
							 point R, int level, int *types, int *pos )
{
	point U[ 3 ], V[ 3 ];  // The permuted vectors of points.
	int pu[ 3 ], pv[ 3 ];  // The original positions of points.
	REAL abovept[ 3 ];
	REAL sA, sB, sC;
	REAL s1, s2, s3, s4;
	int z1;

	if(R == NULL)
	{
		// Calculate a lift point.
		if(1)
		{
			REAL n[ 3 ], len;
			// Calculate a lift point, saved in dummypoint.
			facenormal( A, B, C, n, 1, NULL );
			len = sqrt( dot( n, n ) );
			if(len != 0)
			{
				n[ 0 ] /= len;
				n[ 1 ] /= len;
				n[ 2 ] /= len;
				len = distance( A, B );
				len += distance( B, C );
				len += distance( C, A );
				len /= 3.0;
				R = abovept; //dummypoint;
				R[ 0 ] = A[ 0 ] + len * n[ 0 ];
				R[ 1 ] = A[ 1 ] + len * n[ 1 ];
				R[ 2 ] = A[ 2 ] + len * n[ 2 ];
			}
			else
			{
				// The triangle [A,B,C] is (nearly) degenerate, i.e., it is (close)
				//   to a line.  We need a line-line intersection test.
				// !!! A non-save return value.!!!
				return 0;  // DISJOINT
			}
		}
	}

	// Test A's, B's, and C's orientations wrt plane PQR. 
	sA = orient3d( P, Q, R, A );
	sB = orient3d( P, Q, R, B );
	sC = orient3d( P, Q, R, C );


	if(sA < 0)
	{
		if(sB < 0)
		{
			if(sC < 0)
			{ // (---).
				return 0;
			}
			else
			{
				if(sC > 0)
				{ // (--+).
					// All points are in the right positions.
					SETVECTOR3( U, A, B, C );  // I3
					SETVECTOR3( V, P, Q, R );  // I2
					SETVECTOR3( pu, 0, 1, 2 );
					SETVECTOR3( pv, 0, 1, 2 );
					z1 = 0;
				}
				else
				{ // (--0).
					SETVECTOR3( U, A, B, C );  // I3
					SETVECTOR3( V, P, Q, R );  // I2
					SETVECTOR3( pu, 0, 1, 2 );
					SETVECTOR3( pv, 0, 1, 2 );
					z1 = 1;
				}
			}
		}
		else
		{
			if(sB > 0)
			{
				if(sC < 0)
				{ // (-+-).
					SETVECTOR3( U, C, A, B );  // PT = ST
					SETVECTOR3( V, P, Q, R );  // I2
					SETVECTOR3( pu, 2, 0, 1 );
					SETVECTOR3( pv, 0, 1, 2 );
					z1 = 0;
				}
				else
				{
					if(sC > 0)
					{ // (-++).
						SETVECTOR3( U, B, C, A );  // PT = ST x ST
						SETVECTOR3( V, Q, P, R );  // PL = SL
						SETVECTOR3( pu, 1, 2, 0 );
						SETVECTOR3( pv, 1, 0, 2 );
						z1 = 0;
					}
					else
					{ // (-+0).
						SETVECTOR3( U, C, A, B );  // PT = ST
						SETVECTOR3( V, P, Q, R );  // I2
						SETVECTOR3( pu, 2, 0, 1 );
						SETVECTOR3( pv, 0, 1, 2 );
						z1 = 2;
					}
				}
			}
			else
			{
				if(sC < 0)
				{ // (-0-).
					SETVECTOR3( U, C, A, B );  // PT = ST
					SETVECTOR3( V, P, Q, R );  // I2
					SETVECTOR3( pu, 2, 0, 1 );
					SETVECTOR3( pv, 0, 1, 2 );
					z1 = 1;
				}
				else
				{
					if(sC > 0)
					{ // (-0+).
						SETVECTOR3( U, B, C, A );  // PT = ST x ST
						SETVECTOR3( V, Q, P, R );  // PL = SL
						SETVECTOR3( pu, 1, 2, 0 );
						SETVECTOR3( pv, 1, 0, 2 );
						z1 = 2;
					}
					else
					{ // (-00).
						SETVECTOR3( U, B, C, A );  // PT = ST x ST
						SETVECTOR3( V, Q, P, R );  // PL = SL
						SETVECTOR3( pu, 1, 2, 0 );
						SETVECTOR3( pv, 1, 0, 2 );
						z1 = 3;
					}
				}
			}
		}
	}
	else
	{
		if(sA > 0)
		{
			if(sB < 0)
			{
				if(sC < 0)
				{ // (+--).
					SETVECTOR3( U, B, C, A );  // PT = ST x ST
					SETVECTOR3( V, P, Q, R );  // I2
					SETVECTOR3( pu, 1, 2, 0 );
					SETVECTOR3( pv, 0, 1, 2 );
					z1 = 0;
				}
				else
				{
					if(sC > 0)
					{ // (+-+).
						SETVECTOR3( U, C, A, B );  // PT = ST
						SETVECTOR3( V, Q, P, R );  // PL = SL
						SETVECTOR3( pu, 2, 0, 1 );
						SETVECTOR3( pv, 1, 0, 2 );
						z1 = 0;
					}
					else
					{ // (+-0).
						SETVECTOR3( U, C, A, B );  // PT = ST
						SETVECTOR3( V, Q, P, R );  // PL = SL
						SETVECTOR3( pu, 2, 0, 1 );
						SETVECTOR3( pv, 1, 0, 2 );
						z1 = 2;
					}
				}
			}
			else
			{
				if(sB > 0)
				{
					if(sC < 0)
					{ // (++-).
						SETVECTOR3( U, A, B, C );  // I3
						SETVECTOR3( V, Q, P, R );  // PL = SL
						SETVECTOR3( pu, 0, 1, 2 );
						SETVECTOR3( pv, 1, 0, 2 );
						z1 = 0;
					}
					else
					{
						if(sC > 0)
						{ // (+++).
							return 0;
						}
						else
						{ // (++0).
							SETVECTOR3( U, A, B, C );  // I3
							SETVECTOR3( V, Q, P, R );  // PL = SL
							SETVECTOR3( pu, 0, 1, 2 );
							SETVECTOR3( pv, 1, 0, 2 );
							z1 = 1;
						}
					}
				}
				else
				{ // (+0#)
					if(sC < 0)
					{ // (+0-).
						SETVECTOR3( U, B, C, A );  // PT = ST x ST
						SETVECTOR3( V, P, Q, R );  // I2
						SETVECTOR3( pu, 1, 2, 0 );
						SETVECTOR3( pv, 0, 1, 2 );
						z1 = 2;
					}
					else
					{
						if(sC > 0)
						{ // (+0+).
							SETVECTOR3( U, C, A, B );  // PT = ST
							SETVECTOR3( V, Q, P, R );  // PL = SL
							SETVECTOR3( pu, 2, 0, 1 );
							SETVECTOR3( pv, 1, 0, 2 );
							z1 = 1;
						}
						else
						{ // (+00).
							SETVECTOR3( U, B, C, A );  // PT = ST x ST
							SETVECTOR3( V, P, Q, R );  // I2
							SETVECTOR3( pu, 1, 2, 0 );
							SETVECTOR3( pv, 0, 1, 2 );
							z1 = 3;
						}
					}
				}
			}
		}
		else
		{
			if(sB < 0)
			{
				if(sC < 0)
				{ // (0--).
					SETVECTOR3( U, B, C, A );  // PT = ST x ST
					SETVECTOR3( V, P, Q, R );  // I2
					SETVECTOR3( pu, 1, 2, 0 );
					SETVECTOR3( pv, 0, 1, 2 );
					z1 = 1;
				}
				else
				{
					if(sC > 0)
					{ // (0-+).
						SETVECTOR3( U, A, B, C );  // I3
						SETVECTOR3( V, P, Q, R );  // I2
						SETVECTOR3( pu, 0, 1, 2 );
						SETVECTOR3( pv, 0, 1, 2 );
						z1 = 2;
					}
					else
					{ // (0-0).
						SETVECTOR3( U, C, A, B );  // PT = ST
						SETVECTOR3( V, Q, P, R );  // PL = SL
						SETVECTOR3( pu, 2, 0, 1 );
						SETVECTOR3( pv, 1, 0, 2 );
						z1 = 3;
					}
				}
			}
			else
			{
				if(sB > 0)
				{
					if(sC < 0)
					{ // (0+-).
						SETVECTOR3( U, A, B, C );  // I3
						SETVECTOR3( V, Q, P, R );  // PL = SL
						SETVECTOR3( pu, 0, 1, 2 );
						SETVECTOR3( pv, 1, 0, 2 );
						z1 = 2;
					}
					else
					{
						if(sC > 0)
						{ // (0++).
							SETVECTOR3( U, B, C, A );  // PT = ST x ST
							SETVECTOR3( V, Q, P, R );  // PL = SL
							SETVECTOR3( pu, 1, 2, 0 );
							SETVECTOR3( pv, 1, 0, 2 );
							z1 = 1;
						}
						else
						{ // (0+0).
							SETVECTOR3( U, C, A, B );  // PT = ST
							SETVECTOR3( V, P, Q, R );  // I2
							SETVECTOR3( pu, 2, 0, 1 );
							SETVECTOR3( pv, 0, 1, 2 );
							z1 = 3;
						}
					}
				}
				else
				{ // (00#)
					if(sC < 0)
					{ // (00-).
						SETVECTOR3( U, A, B, C );  // I3
						SETVECTOR3( V, Q, P, R );  // PL = SL
						SETVECTOR3( pu, 0, 1, 2 );
						SETVECTOR3( pv, 1, 0, 2 );
						z1 = 3;
					}
					else
					{
						if(sC > 0)
						{ // (00+).
							SETVECTOR3( U, A, B, C );  // I3
							SETVECTOR3( V, P, Q, R );  // I2
							SETVECTOR3( pu, 0, 1, 2 );
							SETVECTOR3( pv, 0, 1, 2 );
							z1 = 3;
						}
						else
						{ // (000)
							// Not possible unless ABC is degenerate.
							// Avoiding compiler warnings.
							SETVECTOR3( U, A, B, C );  // I3
							SETVECTOR3( V, P, Q, R );  // I2
							SETVECTOR3( pu, 0, 1, 2 );
							SETVECTOR3( pv, 0, 1, 2 );
							z1 = 4;
						}
					}
				}
			}
		}
	}

	s1 = orient3d( U[ 0 ], U[ 2 ], R, V[ 1 ] );  // A, C, R, Q
	s2 = orient3d( U[ 1 ], U[ 2 ], R, V[ 0 ] );  // B, C, R, P

	if(s1 > 0)
	{
		return 0;
	}
	if(s2 < 0)
	{
		return 0;
	}

	if(level == 0)
	{
		return 1;  // They are intersected.
	}


	if(z1 == 1)
	{
		if(s1 == 0)
		{  // (0###)
			// C = Q.
			types[ 0 ] = (int)SHAREVERT;
			pos[ 0 ] = pu[ 2 ]; // C
			pos[ 1 ] = pv[ 1 ]; // Q
			types[ 1 ] = (int)DISJOINT;
		}
		else
		{
			if(s2 == 0)
			{ // (#0##)
				// C = P.
				types[ 0 ] = (int)SHAREVERT;
				pos[ 0 ] = pu[ 2 ]; // C
				pos[ 1 ] = pv[ 0 ]; // P
				types[ 1 ] = (int)DISJOINT;
			}
			else
			{ // (-+##)
				// C in [P, Q].
				types[ 0 ] = (int)ACROSSVERT;
				pos[ 0 ] = pu[ 2 ]; // C
				pos[ 1 ] = pv[ 0 ]; // [P, Q]
				types[ 1 ] = (int)DISJOINT;
			}
		}
		return 4;
	}

	s3 = orient3d( U[ 0 ], U[ 2 ], R, V[ 0 ] );  // A, C, R, P
	s4 = orient3d( U[ 1 ], U[ 2 ], R, V[ 1 ] );  // B, C, R, Q

	if(z1 == 0)
	{  // (tritri-03)
		if(s1 < 0)
		{
			if(s3 > 0)
			{
				if(s4 > 0)
				{
					// [P, Q] overlaps [k, l] (-+++).
					types[ 0 ] = (int)ACROSSEDGE;
					pos[ 0 ] = pu[ 2 ]; // [C, A]
					pos[ 1 ] = pv[ 0 ]; // [P, Q]
					types[ 1 ] = (int)TOUCHFACE;
					pos[ 2 ] = 3;     // [A, B, C]
					pos[ 3 ] = pv[ 1 ]; // Q
				}
				else
				{
					if(s4 == 0)
					{
						// Q = l, [P, Q] contains [k, l] (-++0).
						types[ 0 ] = (int)ACROSSEDGE;
						pos[ 0 ] = pu[ 2 ]; // [C, A]
						pos[ 1 ] = pv[ 0 ]; // [P, Q]
						types[ 1 ] = (int)TOUCHEDGE;
						pos[ 2 ] = pu[ 1 ]; // [B, C]
						pos[ 3 ] = pv[ 1 ]; // Q
					}
					else
					{ // s4 < 0
						// [P, Q] contains [k, l] (-++-).
						types[ 0 ] = (int)ACROSSEDGE;
						pos[ 0 ] = pu[ 2 ]; // [C, A]
						pos[ 1 ] = pv[ 0 ]; // [P, Q]
						types[ 1 ] = (int)ACROSSEDGE;
						pos[ 2 ] = pu[ 1 ]; // [B, C]
						pos[ 3 ] = pv[ 0 ]; // [P, Q]
					}
				}
			}
			else
			{
				if(s3 == 0)
				{
					if(s4 > 0)
					{
						// P = k, [P, Q] in [k, l] (-+0+).
						types[ 0 ] = (int)TOUCHEDGE;
						pos[ 0 ] = pu[ 2 ]; // [C, A]
						pos[ 1 ] = pv[ 0 ]; // P
						types[ 1 ] = (int)TOUCHFACE;
						pos[ 2 ] = 3;     // [A, B, C]
						pos[ 3 ] = pv[ 1 ]; // Q
					}
					else
					{
						if(s4 == 0)
						{
							// [P, Q] = [k, l] (-+00).
							types[ 0 ] = (int)TOUCHEDGE;
							pos[ 0 ] = pu[ 2 ]; // [C, A]
							pos[ 1 ] = pv[ 0 ]; // P
							types[ 1 ] = (int)TOUCHEDGE;
							pos[ 2 ] = pu[ 1 ]; // [B, C]
							pos[ 3 ] = pv[ 1 ]; // Q
						}
						else
						{
							// P = k, [P, Q] contains [k, l] (-+0-).
							types[ 0 ] = (int)TOUCHEDGE;
							pos[ 0 ] = pu[ 2 ]; // [C, A]
							pos[ 1 ] = pv[ 0 ]; // P
							types[ 1 ] = (int)ACROSSEDGE;
							pos[ 2 ] = pu[ 1 ]; // [B, C]
							pos[ 3 ] = pv[ 0 ]; // [P, Q]
						}
					}
				}
				else
				{ // s3 < 0
					if(s2 > 0)
					{
						if(s4 > 0)
						{
							// [P, Q] in [k, l] (-+-+).
							types[ 0 ] = (int)TOUCHFACE;
							pos[ 0 ] = 3;     // [A, B, C]
							pos[ 1 ] = pv[ 0 ]; // P
							types[ 1 ] = (int)TOUCHFACE;
							pos[ 2 ] = 3;     // [A, B, C]
							pos[ 3 ] = pv[ 1 ]; // Q
						}
						else
						{
							if(s4 == 0)
							{
								// Q = l, [P, Q] in [k, l] (-+-0).
								types[ 0 ] = (int)TOUCHFACE;
								pos[ 0 ] = 3;     // [A, B, C]
								pos[ 1 ] = pv[ 0 ]; // P
								types[ 1 ] = (int)TOUCHEDGE;
								pos[ 2 ] = pu[ 1 ]; // [B, C]
								pos[ 3 ] = pv[ 1 ]; // Q
							}
							else
							{ // s4 < 0
								// [P, Q] overlaps [k, l] (-+--).
								types[ 0 ] = (int)TOUCHFACE;
								pos[ 0 ] = 3;     // [A, B, C]
								pos[ 1 ] = pv[ 0 ]; // P
								types[ 1 ] = (int)ACROSSEDGE;
								pos[ 2 ] = pu[ 1 ]; // [B, C]
								pos[ 3 ] = pv[ 0 ]; // [P, Q]
							}
						}
					}
					else
					{ // s2 == 0
						// P = l (#0##).
						types[ 0 ] = (int)TOUCHEDGE;
						pos[ 0 ] = pu[ 1 ]; // [B, C]
						pos[ 1 ] = pv[ 0 ]; // P
						types[ 1 ] = (int)DISJOINT;
					}
				}
			}
		}
		else
		{ // s1 == 0
			// Q = k (0####)
			types[ 0 ] = (int)TOUCHEDGE;
			pos[ 0 ] = pu[ 2 ]; // [C, A]
			pos[ 1 ] = pv[ 1 ]; // Q
			types[ 1 ] = (int)DISJOINT;
		}
	}
	else if(z1 == 2)
	{  // (tritri-23)
		if(s1 < 0)
		{
			if(s3 > 0)
			{
				if(s4 > 0)
				{
					// [P, Q] overlaps [A, l] (-+++).
					types[ 0 ] = (int)ACROSSVERT;
					pos[ 0 ] = pu[ 0 ]; // A
					pos[ 1 ] = pv[ 0 ]; // [P, Q]
					types[ 1 ] = (int)TOUCHFACE;
					pos[ 2 ] = 3;     // [A, B, C]
					pos[ 3 ] = pv[ 1 ]; // Q
				}
				else
				{
					if(s4 == 0)
					{
						// Q = l, [P, Q] contains [A, l] (-++0).
						types[ 0 ] = (int)ACROSSVERT;
						pos[ 0 ] = pu[ 0 ]; // A
						pos[ 1 ] = pv[ 0 ]; // [P, Q]
						types[ 1 ] = (int)TOUCHEDGE;
						pos[ 2 ] = pu[ 1 ]; // [B, C]
						pos[ 3 ] = pv[ 1 ]; // Q
					}
					else
					{ // s4 < 0
						// [P, Q] contains [A, l] (-++-).
						types[ 0 ] = (int)ACROSSVERT;
						pos[ 0 ] = pu[ 0 ]; // A
						pos[ 1 ] = pv[ 0 ]; // [P, Q]
						types[ 1 ] = (int)ACROSSEDGE;
						pos[ 2 ] = pu[ 1 ]; // [B, C]
						pos[ 3 ] = pv[ 0 ]; // [P, Q]
					}
				}
			}
			else
			{
				if(s3 == 0)
				{
					if(s4 > 0)
					{
						// P = A, [P, Q] in [A, l] (-+0+).
						types[ 0 ] = (int)SHAREVERT;
						pos[ 0 ] = pu[ 0 ]; // A
						pos[ 1 ] = pv[ 0 ]; // P
						types[ 1 ] = (int)TOUCHFACE;
						pos[ 2 ] = 3;     // [A, B, C]
						pos[ 3 ] = pv[ 1 ]; // Q
					}
					else
					{
						if(s4 == 0)
						{
							// [P, Q] = [A, l] (-+00).
							types[ 0 ] = (int)SHAREVERT;
							pos[ 0 ] = pu[ 0 ]; // A
							pos[ 1 ] = pv[ 0 ]; // P
							types[ 1 ] = (int)TOUCHEDGE;
							pos[ 2 ] = pu[ 1 ]; // [B, C]
							pos[ 3 ] = pv[ 1 ]; // Q
						}
						else
						{ // s4 < 0
							// Q = l, [P, Q] in [A, l] (-+0-).
							types[ 0 ] = (int)SHAREVERT;
							pos[ 0 ] = pu[ 0 ]; // A
							pos[ 1 ] = pv[ 0 ]; // P
							types[ 1 ] = (int)ACROSSEDGE;
							pos[ 2 ] = pu[ 1 ]; // [B, C]
							pos[ 3 ] = pv[ 0 ]; // [P, Q]
						}
					}
				}
				else
				{ // s3 < 0
					if(s2 > 0)
					{
						if(s4 > 0)
						{
							// [P, Q] in [A, l] (-+-+).
							types[ 0 ] = (int)TOUCHFACE;
							pos[ 0 ] = 3;     // [A, B, C]
							pos[ 1 ] = pv[ 0 ]; // P
							types[ 0 ] = (int)TOUCHFACE;
							pos[ 0 ] = 3;     // [A, B, C]
							pos[ 1 ] = pv[ 1 ]; // Q
						}
						else
						{
							if(s4 == 0)
							{
								// Q = l, [P, Q] in [A, l] (-+-0).
								types[ 0 ] = (int)TOUCHFACE;
								pos[ 0 ] = 3;     // [A, B, C]
								pos[ 1 ] = pv[ 0 ]; // P
								types[ 0 ] = (int)TOUCHEDGE;
								pos[ 0 ] = pu[ 1 ]; // [B, C]
								pos[ 1 ] = pv[ 1 ]; // Q
							}
							else
							{ // s4 < 0
								// [P, Q] overlaps [A, l] (-+--).
								types[ 0 ] = (int)TOUCHFACE;
								pos[ 0 ] = 3;     // [A, B, C]
								pos[ 1 ] = pv[ 0 ]; // P
								types[ 0 ] = (int)ACROSSEDGE;
								pos[ 0 ] = pu[ 1 ]; // [B, C]
								pos[ 1 ] = pv[ 0 ]; // [P, Q]
							}
						}
					}
					else
					{ // s2 == 0
						// P = l (#0##).
						types[ 0 ] = (int)TOUCHEDGE;
						pos[ 0 ] = pu[ 1 ]; // [B, C]
						pos[ 1 ] = pv[ 0 ]; // P
						types[ 1 ] = (int)DISJOINT;
					}
				}
			}
		}
		else
		{ // s1 == 0
			// Q = A (0###).
			types[ 0 ] = (int)SHAREVERT;
			pos[ 0 ] = pu[ 0 ]; // A
			pos[ 1 ] = pv[ 1 ]; // Q
			types[ 1 ] = (int)DISJOINT;
		}
	}
	else if(z1 == 3)
	{  // (tritri-33)
		if(s1 < 0)
		{
			if(s3 > 0)
			{
				if(s4 > 0)
				{
					// [P, Q] overlaps [A, B] (-+++).
					types[ 0 ] = (int)ACROSSVERT;
					pos[ 0 ] = pu[ 0 ]; // A
					pos[ 1 ] = pv[ 0 ]; // [P, Q]
					types[ 1 ] = (int)TOUCHEDGE;
					pos[ 2 ] = pu[ 0 ]; // [A, B]
					pos[ 3 ] = pv[ 1 ]; // Q
				}
				else
				{
					if(s4 == 0)
					{
						// Q = B, [P, Q] contains [A, B] (-++0).
						types[ 0 ] = (int)ACROSSVERT;
						pos[ 0 ] = pu[ 0 ]; // A
						pos[ 1 ] = pv[ 0 ]; // [P, Q]
						types[ 1 ] = (int)SHAREVERT;
						pos[ 2 ] = pu[ 1 ]; // B
						pos[ 3 ] = pv[ 1 ]; // Q
					}
					else
					{ // s4 < 0
						// [P, Q] contains [A, B] (-++-).
						types[ 0 ] = (int)ACROSSVERT;
						pos[ 0 ] = pu[ 0 ]; // A
						pos[ 1 ] = pv[ 0 ]; // [P, Q]
						types[ 1 ] = (int)ACROSSVERT;
						pos[ 2 ] = pu[ 1 ]; // B
						pos[ 3 ] = pv[ 0 ]; // [P, Q]
					}
				}
			}
			else
			{
				if(s3 == 0)
				{
					if(s4 > 0)
					{
						// P = A, [P, Q] in [A, B] (-+0+).
						types[ 0 ] = (int)SHAREVERT;
						pos[ 0 ] = pu[ 0 ]; // A
						pos[ 1 ] = pv[ 0 ]; // P
						types[ 1 ] = (int)TOUCHEDGE;
						pos[ 2 ] = pu[ 0 ]; // [A, B]
						pos[ 3 ] = pv[ 1 ]; // Q
					}
					else
					{
						if(s4 == 0)
						{
							// [P, Q] = [A, B] (-+00).
							types[ 0 ] = (int)SHAREEDGE;
							pos[ 0 ] = pu[ 0 ]; // [A, B]
							pos[ 1 ] = pv[ 0 ]; // [P, Q]
							types[ 1 ] = (int)DISJOINT;
						}
						else
						{ // s4 < 0
							// P= A, [P, Q] in [A, B] (-+0-).
							types[ 0 ] = (int)SHAREVERT;
							pos[ 0 ] = pu[ 0 ]; // A
							pos[ 1 ] = pv[ 0 ]; // P
							types[ 1 ] = (int)ACROSSVERT;
							pos[ 2 ] = pu[ 1 ]; // B
							pos[ 3 ] = pv[ 0 ]; // [P, Q]
						}
					}
				}
				else
				{ // s3 < 0
					if(s2 > 0)
					{
						if(s4 > 0)
						{
							// [P, Q] in [A, B] (-+-+).
							types[ 0 ] = (int)TOUCHEDGE;
							pos[ 0 ] = pu[ 0 ]; // [A, B]
							pos[ 1 ] = pv[ 0 ]; // P
							types[ 1 ] = (int)TOUCHEDGE;
							pos[ 2 ] = pu[ 0 ]; // [A, B]
							pos[ 3 ] = pv[ 1 ]; // Q
						}
						else
						{
							if(s4 == 0)
							{
								// Q = B, [P, Q] in [A, B] (-+-0).
								types[ 0 ] = (int)TOUCHEDGE;
								pos[ 0 ] = pu[ 0 ]; // [A, B]
								pos[ 1 ] = pv[ 0 ]; // P
								types[ 1 ] = (int)SHAREVERT;
								pos[ 2 ] = pu[ 1 ]; // B
								pos[ 3 ] = pv[ 1 ]; // Q
							}
							else
							{ // s4 < 0
								// [P, Q] overlaps [A, B] (-+--).
								types[ 0 ] = (int)TOUCHEDGE;
								pos[ 0 ] = pu[ 0 ]; // [A, B]
								pos[ 1 ] = pv[ 0 ]; // P
								types[ 1 ] = (int)ACROSSVERT;
								pos[ 2 ] = pu[ 1 ]; // B
								pos[ 3 ] = pv[ 0 ]; // [P, Q]
							}
						}
					}
					else
					{ // s2 == 0
						// P = B (#0##).
						types[ 0 ] = (int)SHAREVERT;
						pos[ 0 ] = pu[ 1 ]; // B
						pos[ 1 ] = pv[ 0 ]; // P
						types[ 1 ] = (int)DISJOINT;
					}
				}
			}
		}
		else
		{ // s1 == 0
			// Q = A (0###).
			types[ 0 ] = (int)SHAREVERT;
			pos[ 0 ] = pu[ 0 ]; // A
			pos[ 1 ] = pv[ 1 ]; // Q
			types[ 1 ] = (int)DISJOINT;
		}
	}

	return 4;
}

int tetgenmesh::tri_edge_tail( point A, point B, point C, point P, point Q, point R,
							   REAL sP, REAL sQ, int level, int *types, int *pos )
{
	point U[ 3 ], V[ 3 ]; //, Ptmp;
	int pu[ 3 ], pv[ 3 ]; //, itmp;
	REAL s1, s2, s3;
	int z1;


	if(sP < 0)
	{
		if(sQ < 0)
		{ // (--) disjoint
			return 0;
		}
		else
		{
			if(sQ > 0)
			{ // (-+)
				SETVECTOR3( U, A, B, C );
				SETVECTOR3( V, P, Q, R );
				SETVECTOR3( pu, 0, 1, 2 );
				SETVECTOR3( pv, 0, 1, 2 );
				z1 = 0;
			}
			else
			{ // (-0)
				SETVECTOR3( U, A, B, C );
				SETVECTOR3( V, P, Q, R );
				SETVECTOR3( pu, 0, 1, 2 );
				SETVECTOR3( pv, 0, 1, 2 );
				z1 = 1;
			}
		}
	}
	else
	{
		if(sP > 0)
		{ // (+-)
			if(sQ < 0)
			{
				SETVECTOR3( U, A, B, C );
				SETVECTOR3( V, Q, P, R );  // P and Q are flipped.
				SETVECTOR3( pu, 0, 1, 2 );
				SETVECTOR3( pv, 1, 0, 2 );
				z1 = 0;
			}
			else
			{
				if(sQ > 0)
				{ // (++) disjoint
					return 0;
				}
				else
				{ // (+0)
					SETVECTOR3( U, B, A, C ); // A and B are flipped.
					SETVECTOR3( V, P, Q, R );
					SETVECTOR3( pu, 1, 0, 2 );
					SETVECTOR3( pv, 0, 1, 2 );
					z1 = 1;
				}
			}
		}
		else
		{ // sP == 0
			if(sQ < 0)
			{ // (0-)
				SETVECTOR3( U, A, B, C );
				SETVECTOR3( V, Q, P, R );  // P and Q are flipped.
				SETVECTOR3( pu, 0, 1, 2 );
				SETVECTOR3( pv, 1, 0, 2 );
				z1 = 1;
			}
			else
			{
				if(sQ > 0)
				{ // (0+)
					SETVECTOR3( U, B, A, C );  // A and B are flipped.
					SETVECTOR3( V, Q, P, R );  // P and Q are flipped.
					SETVECTOR3( pu, 1, 0, 2 );
					SETVECTOR3( pv, 1, 0, 2 );
					z1 = 1;
				}
				else
				{ // (00)
					// A, B, C, P, and Q are coplanar.
					z1 = 2;
				}
			}
		}
	}

	if(z1 == 2)
	{
		// The triangle and the edge are coplanar.
		return tri_edge_2d( A, B, C, P, Q, R, level, types, pos );
	}

	s1 = orient3d( U[ 0 ], U[ 1 ], V[ 0 ], V[ 1 ] );
	if(s1 < 0)
	{
		return 0;
	}

	s2 = orient3d( U[ 1 ], U[ 2 ], V[ 0 ], V[ 1 ] );
	if(s2 < 0)
	{
		return 0;
	}

	s3 = orient3d( U[ 2 ], U[ 0 ], V[ 0 ], V[ 1 ] );
	if(s3 < 0)
	{
		return 0;
	}

	if(level == 0)
	{
		return 1;  // The are intersected.
	}

	types[ 1 ] = (int)DISJOINT; // No second intersection point.

	if(z1 == 0)
	{
		if(s1 > 0)
		{
			if(s2 > 0)
			{
				if(s3 > 0)
				{ // (+++)
					// [P, Q] passes interior of [A, B, C].
					types[ 0 ] = (int)ACROSSFACE;
					pos[ 0 ] = 3;  // interior of [A, B, C]
					pos[ 1 ] = 0;  // [P, Q]
				}
				else
				{ // s3 == 0 (++0)
					// [P, Q] intersects [C, A].
					types[ 0 ] = (int)ACROSSEDGE;
					pos[ 0 ] = pu[ 2 ];  // [C, A]
					pos[ 1 ] = 0;  // [P, Q]
				}
			}
			else
			{ // s2 == 0
				if(s3 > 0)
				{ // (+0+)
					// [P, Q] intersects [B, C].
					types[ 0 ] = (int)ACROSSEDGE;
					pos[ 0 ] = pu[ 1 ];  // [B, C]
					pos[ 1 ] = 0;  // [P, Q]
				}
				else
				{ // s3 == 0 (+00)
					// [P, Q] passes C.
					types[ 0 ] = (int)ACROSSVERT;
					pos[ 0 ] = pu[ 2 ];  // C
					pos[ 1 ] = 0;  // [P, Q]
				}
			}
		}
		else
		{ // s1 == 0
			if(s2 > 0)
			{
				if(s3 > 0)
				{ // (0++)
					// [P, Q] intersects [A, B].
					types[ 0 ] = (int)ACROSSEDGE;
					pos[ 0 ] = pu[ 0 ];  // [A, B]
					pos[ 1 ] = 0;  // [P, Q]
				}
				else
				{ // s3 == 0 (0+0)
					// [P, Q] passes A.
					types[ 0 ] = (int)ACROSSVERT;
					pos[ 0 ] = pu[ 0 ];  // A
					pos[ 1 ] = 0;  // [P, Q]
				}
			}
			else
			{ // s2 == 0
				if(s3 > 0)
				{ // (00+)
					// [P, Q] passes B.
					types[ 0 ] = (int)ACROSSVERT;
					pos[ 0 ] = pu[ 1 ];  // B
					pos[ 1 ] = 0;  // [P, Q]
				}
			}
		}
	}
	else
	{ // z1 == 1
		if(s1 > 0)
		{
			if(s2 > 0)
			{
				if(s3 > 0)
				{ // (+++)
					// Q lies in [A, B, C].
					types[ 0 ] = (int)TOUCHFACE;
					pos[ 0 ] = 0; // [A, B, C]
					pos[ 1 ] = pv[ 1 ]; // Q
				}
				else
				{ // s3 == 0 (++0)
					// Q lies on [C, A].
					types[ 0 ] = (int)TOUCHEDGE;
					pos[ 0 ] = pu[ 2 ]; // [C, A]
					pos[ 1 ] = pv[ 1 ]; // Q
				}
			}
			else
			{ // s2 == 0
				if(s3 > 0)
				{ // (+0+)
					// Q lies on [B, C].
					types[ 0 ] = (int)TOUCHEDGE;
					pos[ 0 ] = pu[ 1 ]; // [B, C]
					pos[ 1 ] = pv[ 1 ]; // Q
				}
				else
				{ // s3 == 0 (+00)
					// Q = C.
					types[ 0 ] = (int)SHAREVERT;
					pos[ 0 ] = pu[ 2 ]; // C
					pos[ 1 ] = pv[ 1 ]; // Q
				}
			}
		}
		else
		{ // s1 == 0
			if(s2 > 0)
			{
				if(s3 > 0)
				{ // (0++)
					// Q lies on [A, B].
					types[ 0 ] = (int)TOUCHEDGE;
					pos[ 0 ] = pu[ 0 ]; // [A, B]
					pos[ 1 ] = pv[ 1 ]; // Q
				}
				else
				{ // s3 == 0 (0+0)
					// Q = A.
					types[ 0 ] = (int)SHAREVERT;
					pos[ 0 ] = pu[ 0 ]; // A
					pos[ 1 ] = pv[ 1 ]; // Q
				}
			}
			else
			{ // s2 == 0
				if(s3 > 0)
				{ // (00+)
					// Q = B.
					types[ 0 ] = (int)SHAREVERT;
					pos[ 0 ] = pu[ 1 ]; // B
					pos[ 1 ] = pv[ 1 ]; // Q
				}
			}
		}
	}

	// T and E intersect in a single point.
	return 2;
}

int tetgenmesh::tri_edge_test( point A, point B, point C, point P, point Q,
							   point R, int level, int *types, int *pos )
{
	REAL sP, sQ;

	// Test the locations of P and Q with respect to ABC.
	sP = orient3d( A, B, C, P );
	sQ = orient3d( A, B, C, Q );

	return tri_edge_tail( A, B, C, P, Q, R, sP, sQ, level, types, pos );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// tri_tri_inter()    Test whether two triangle (abc) and (opq) are          //
//                    intersecting or not.                                   //
//                                                                           //
// Return 0 if they are disjoint. Otherwise, return 1. 'type' returns one of //
// the four cases: SHAREVERTEX, SHAREEDGE, SHAREFACE, and INTERSECT.         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::tri_edge_inter_tail( REAL* A, REAL* B, REAL* C, REAL* P,
									 REAL* Q, REAL s_p, REAL s_q )
{
	int types[ 2 ], pos[ 4 ];
	int ni;  // =0, 2, 4

	ni = tri_edge_tail( A, B, C, P, Q, NULL, s_p, s_q, 1, types, pos );

	if(ni > 0)
	{
		if(ni == 2)
		{
			// Get the intersection type.
			if(types[ 0 ] == (int)SHAREVERT)
			{
				return (int)SHAREVERT;
			}
			else
			{
				return (int)INTERSECT;
			}
		}
		else if(ni == 4)
		{
			// There may be two intersections.
			if(types[ 0 ] == (int)SHAREVERT)
			{
				if(types[ 1 ] == (int)DISJOINT)
				{
					return (int)SHAREVERT;
				}
				else
				{
					return (int)INTERSECT;
				}
			}
			else
			{
				if(types[ 0 ] == (int)SHAREEDGE)
				{
					return (int)SHAREEDGE;
				}
				else
				{
					return (int)INTERSECT;
				}
			}
		}
	}

	return (int)DISJOINT;
}

int tetgenmesh::tri_tri_inter( REAL* A, REAL* B, REAL* C, REAL* O, REAL* P, REAL* Q )
{
	REAL s_o, s_p, s_q;
	REAL s_a, s_b, s_c;

	s_o = orient3d( A, B, C, O );
	s_p = orient3d( A, B, C, P );
	s_q = orient3d( A, B, C, Q );
	if(( s_o * s_p > 0.0 ) && ( s_o * s_q > 0.0 ))
	{
		// o, p, q are all in the same halfspace of ABC.
		return 0; // DISJOINT;
	}

	s_a = orient3d( O, P, Q, A );
	s_b = orient3d( O, P, Q, B );
	s_c = orient3d( O, P, Q, C );
	if(( s_a * s_b > 0.0 ) && ( s_a * s_c > 0.0 ))
	{
		// a, b, c are all in the same halfspace of OPQ.
		return 0; // DISJOINT;
	}

	int abcop, abcpq, abcqo;
	int shareedge = 0;

	abcop = tri_edge_inter_tail( A, B, C, O, P, s_o, s_p );
	if(abcop == (int)INTERSECT)
	{
		return (int)INTERSECT;
	}
	else if(abcop == (int)SHAREEDGE)
	{
		shareedge++;
	}
	abcpq = tri_edge_inter_tail( A, B, C, P, Q, s_p, s_q );
	if(abcpq == (int)INTERSECT)
	{
		return (int)INTERSECT;
	}
	else if(abcpq == (int)SHAREEDGE)
	{
		shareedge++;
	}
	abcqo = tri_edge_inter_tail( A, B, C, Q, O, s_q, s_o );
	if(abcqo == (int)INTERSECT)
	{
		return (int)INTERSECT;
	}
	else if(abcqo == (int)SHAREEDGE)
	{
		shareedge++;
	}
	if(shareedge == 3)
	{
		// opq are coincident with abc.
		return (int)SHAREFACE;
	}

	// Continue to detect whether opq and abc are intersecting or not.
	int opqab, opqbc, opqca;

	opqab = tri_edge_inter_tail( O, P, Q, A, B, s_a, s_b );
	if(opqab == (int)INTERSECT)
	{
		return (int)INTERSECT;
	}
	opqbc = tri_edge_inter_tail( O, P, Q, B, C, s_b, s_c );
	if(opqbc == (int)INTERSECT)
	{
		return (int)INTERSECT;
	}
	opqca = tri_edge_inter_tail( O, P, Q, C, A, s_c, s_a );
	if(opqca == (int)INTERSECT)
	{
		return (int)INTERSECT;
	}

	// At this point, two triangles are not intersecting and not coincident.
	//   They may be share an edge, or share a vertex, or disjoint.
	if(abcop == (int)SHAREEDGE)
	{
		// op is coincident with an edge of abc.
		return (int)SHAREEDGE;
	}
	if(abcpq == (int)SHAREEDGE)
	{
		// pq is coincident with an edge of abc.
		return (int)SHAREEDGE;
	}
	if(abcqo == (int)SHAREEDGE)
	{
		// qo is coincident with an edge of abc.
		return (int)SHAREEDGE;
	}

	// They may share a vertex or disjoint.
	if(abcop == (int)SHAREVERT)
	{
		return (int)SHAREVERT;
	}
	if(abcpq == (int)SHAREVERT)
	{
		// q is the coincident vertex.
		return (int)SHAREVERT;
	}

	// They are disjoint.
	return (int)DISJOINT;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// lu_decmp()    Compute the LU decomposition of a matrix.                   //
//                                                                           //
// Compute the LU decomposition of a (non-singular) square matrix A using    //
// partial pivoting and implicit row exchanges.  The result is:              //
//     A = P * L * U,                                                        //
// where P is a permutation matrix, L is unit lower triangular, and U is     //
// upper triangular.  The factored form of A is used in combination with     //
// 'lu_solve()' to solve linear equations: Ax = b, or invert a matrix.       //
//                                                                           //
// The inputs are a square matrix 'lu[N..n+N-1][N..n+N-1]', it's size is 'n'.//
// On output, 'lu' is replaced by the LU decomposition of a rowwise permuta- //
// tion of itself, 'ps[N..n+N-1]' is an output vector that records the row   //
// permutation effected by the partial pivoting, effectively,  'ps' array    //
// tells the user what the permutation matrix P is; 'd' is output as +1/-1   //
// depending on whether the number of row interchanges was even or odd,      //
// respectively.                                                             //
//                                                                           //
// Return true if the LU decomposition is successfully computed, otherwise,  //
// return false in case that A is a singular matrix.                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

bool tetgenmesh::lu_decmp( REAL lu[ 4 ][ 4 ], int n, int* ps, REAL* d, int N )
{
	REAL scales[ 4 ];
	REAL pivot, biggest, mult, tempf;
	int pivotindex = 0;
	int i, j, k;

	*d = 1.0;                                      // No row interchanges yet.

	for(i = N; i < n + N; i++)
	{                             // For each row.
		// Find the largest element in each row for row equilibration
		biggest = 0.0;
		for(j = N; j < n + N; j++)
		if(biggest < ( tempf = fabs( lu[ i ][ j ] ) ))
			biggest = tempf;
		if(biggest != 0.0)
			scales[ i ] = 1.0 / biggest;
		else
		{
			scales[ i ] = 0.0;
			return false;                            // Zero row: singular matrix.
		}
		ps[ i ] = i;                                 // Initialize pivot sequence.
	}

	for(k = N; k < n + N - 1; k++)
	{                      // For each column.
		// Find the largest element in each column to pivot around.
		biggest = 0.0;
		for(i = k; i < n + N; i++)
		{
			if(biggest < ( tempf = fabs( lu[ ps[ i ] ][ k ] ) * scales[ ps[ i ] ] ))
			{
				biggest = tempf;
				pivotindex = i;
			}
		}
		if(biggest == 0.0)
		{
			return false;                         // Zero column: singular matrix.
		}
		if(pivotindex != k)
		{                         // Update pivot sequence.
			j = ps[ k ];
			ps[ k ] = ps[ pivotindex ];
			ps[ pivotindex ] = j;
			*d = -( *d );                          // ...and change the parity of d.
		}

		// Pivot, eliminating an extra variable  each time
		pivot = lu[ ps[ k ] ][ k ];
		for(i = k + 1; i < n + N; i++)
		{
			lu[ ps[ i ] ][ k ] = mult = lu[ ps[ i ] ][ k ] / pivot;
			if(mult != 0.0)
			{
				for(j = k + 1; j < n + N; j++)
					lu[ ps[ i ] ][ j ] -= mult * lu[ ps[ k ] ][ j ];
			}
		}
	}

	// (lu[ps[n + N - 1]][n + N - 1] == 0.0) ==> A is singular.
	return lu[ ps[ n + N - 1 ] ][ n + N - 1 ] != 0.0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// lu_solve()    Solves the linear equation:  Ax = b,  after the matrix A    //
//               has been decomposed into the lower and upper triangular     //
//               matrices L and U, where A = LU.                             //
//                                                                           //
// 'lu[N..n+N-1][N..n+N-1]' is input, not as the matrix 'A' but rather as    //
// its LU decomposition, computed by the routine 'lu_decmp'; 'ps[N..n+N-1]'  //
// is input as the permutation vector returned by 'lu_decmp';  'b[N..n+N-1]' //
// is input as the right-hand side vector, and returns with the solution     //
// vector. 'lu', 'n', and 'ps' are not modified by this routine and can be   //
// left in place for successive calls with different right-hand sides 'b'.   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::lu_solve( REAL lu[ 4 ][ 4 ], int n, int* ps, REAL* b, int N )
{
	int i, j;
	REAL X[ 4 ], dot;

	for(i = N; i < n + N; i++) X[ i ] = 0.0;

	// Vector reduction using U triangular matrix.
	for(i = N; i < n + N; i++)
	{
		dot = 0.0;
		for(j = N; j < i + N; j++)
			dot += lu[ ps[ i ] ][ j ] * X[ j ];
		X[ i ] = b[ ps[ i ] ] - dot;
	}

	// Back substitution, in L triangular matrix.
	for(i = n + N - 1; i >= N; i--)
	{
		dot = 0.0;
		for(j = i + 1; j < n + N; j++)
			dot += lu[ ps[ i ] ][ j ] * X[ j ];
		X[ i ] = ( X[ i ] - dot ) / lu[ ps[ i ] ][ i ];
	}

	for(i = N; i < n + N; i++) b[ i ] = X[ i ];
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// incircle3d()    3D in-circle test.                                        //
//                                                                           //
// Return a negative value if pd is inside the circumcircle of the triangle  //
// pa, pb, and pc.                                                           //
//                                                                           //
// IMPORTANT: It assumes that [a,b] is the common edge, i.e., the two input  //
// triangles are [a,b,c] and [b,a,d].                                        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

REAL tetgenmesh::incircle3d( point pa, point pb, point pc, point pd )
{
	REAL area2[ 2 ], n1[ 3 ], n2[ 3 ], c[ 3 ];
	REAL sign, r, d;

	// Calculate the areas of the two triangles [a, b, c] and [b, a, d].
	facenormal( pa, pb, pc, n1, 1, NULL );
	area2[ 0 ] = dot( n1, n1 );
	facenormal( pb, pa, pd, n2, 1, NULL );
	area2[ 1 ] = dot( n2, n2 );

	if(area2[ 0 ] > area2[ 1 ])
	{
		// Choose [a, b, c] as the base triangle.
		circumsphere( pa, pb, pc, NULL, c, &r );
		d = distance( c, pd );
	}
	else
	{
		// Choose [b, a, d] as the base triangle.
		if(area2[ 1 ] > 0)
		{
			circumsphere( pb, pa, pd, NULL, c, &r );
			d = distance( c, pc );
		}
		else
		{
			// The four points are collinear. This case only happens on the boundary.
			return 0; // Return "not inside".
		}
	}

	sign = d - r;
	if(fabs( sign ) / r < b->epsilon)
	{
		sign = 0;
	}

	return sign;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// facenormal()    Calculate the normal of the face.                         //
//                                                                           //
// The normal of the face abc can be calculated by the cross product of 2 of //
// its 3 edge vectors.  A better choice of two edge vectors will reduce the  //
// numerical error during the calculation.  Burdakov proved that the optimal //
// basis problem is equivalent to the minimum spanning tree problem with the //
// edge length be the functional, see Burdakov, "A greedy algorithm for the  //
// optimal basis problem", BIT 37:3 (1997), 591-599. If 'pivot' > 0, the two //
// short edges in abc are chosen for the calculation.                        //
//                                                                           //
// If 'lav' is not NULL and if 'pivot' is set, the average edge length of    //
// the edges of the face [a,b,c] is returned.                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::facenormal( point pa, point pb, point pc, REAL *n, int pivot,
							 REAL* lav )
{
	REAL v1[ 3 ], v2[ 3 ], v3[ 3 ], *pv1, *pv2;
	REAL L1, L2, L3;

	v1[ 0 ] = pb[ 0 ] - pa[ 0 ];  // edge vector v1: a->b
	v1[ 1 ] = pb[ 1 ] - pa[ 1 ];
	v1[ 2 ] = pb[ 2 ] - pa[ 2 ];
	v2[ 0 ] = pa[ 0 ] - pc[ 0 ];  // edge vector v2: c->a
	v2[ 1 ] = pa[ 1 ] - pc[ 1 ];
	v2[ 2 ] = pa[ 2 ] - pc[ 2 ];

	// Default, normal is calculated by: v1 x (-v2) (see Fig. fnormal).
	if(pivot > 0)
	{
		// Choose edge vectors by Burdakov's algorithm.
		v3[ 0 ] = pc[ 0 ] - pb[ 0 ];  // edge vector v3: b->c
		v3[ 1 ] = pc[ 1 ] - pb[ 1 ];
		v3[ 2 ] = pc[ 2 ] - pb[ 2 ];
		L1 = dot( v1, v1 );
		L2 = dot( v2, v2 );
		L3 = dot( v3, v3 );
		// Sort the three edge lengths.
		if(L1 < L2)
		{
			if(L2 < L3)
			{
				pv1 = v1; pv2 = v2; // n = v1 x (-v2).
			}
			else
			{
				pv1 = v3; pv2 = v1; // n = v3 x (-v1).
			}
		}
		else
		{
			if(L1 < L3)
			{
				pv1 = v1; pv2 = v2; // n = v1 x (-v2).
			}
			else
			{
				pv1 = v2; pv2 = v3; // n = v2 x (-v3).
			}
		}
		if(lav)
		{
			// return the average edge length.
			*lav = ( sqrt( L1 ) + sqrt( L2 ) + sqrt( L3 ) ) / 3.0;
		}
	}
	else
	{
		pv1 = v1; pv2 = v2; // n = v1 x (-v2).
	}

	// Calculate the face normal.
	cross( pv1, pv2, n );
	// Inverse the direction;
	n[ 0 ] = -n[ 0 ];
	n[ 1 ] = -n[ 1 ];
	n[ 2 ] = -n[ 2 ];
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// shortdistance()    Returns the shortest distance from point p to a line   //
//                    defined by two points e1 and e2.                       //
//                                                                           //
// First compute the projection length l_p of the vector v1 = p - e1 along   //
// the vector v2 = e2 - e1. Then Pythagoras' Theorem is used to compute the  //
// shortest distance.                                                        //
//                                                                           //
// This routine allows that p is collinear with the line. In this case, the  //
// return value is zero. The two points e1 and e2 should not be identical.   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

REAL tetgenmesh::shortdistance( REAL* p, REAL* e1, REAL* e2 )
{
	REAL v1[ 3 ], v2[ 3 ];
	REAL len, l_p;

	v1[ 0 ] = e2[ 0 ] - e1[ 0 ];
	v1[ 1 ] = e2[ 1 ] - e1[ 1 ];
	v1[ 2 ] = e2[ 2 ] - e1[ 2 ];
	v2[ 0 ] = p[ 0 ] - e1[ 0 ];
	v2[ 1 ] = p[ 1 ] - e1[ 1 ];
	v2[ 2 ] = p[ 2 ] - e1[ 2 ];

	len = sqrt( dot( v1, v1 ) );

	v1[ 0 ] /= len;
	v1[ 1 ] /= len;
	v1[ 2 ] /= len;
	l_p = dot( v1, v2 );

	return sqrt( dot( v2, v2 ) - l_p * l_p );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// triarea()    Return the area of a triangle.                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

REAL tetgenmesh::triarea( REAL* pa, REAL* pb, REAL* pc )
{
	REAL A[ 4 ][ 4 ];

	// Compute the coefficient matrix A (3x3).
	A[ 0 ][ 0 ] = pb[ 0 ] - pa[ 0 ];
	A[ 0 ][ 1 ] = pb[ 1 ] - pa[ 1 ];
	A[ 0 ][ 2 ] = pb[ 2 ] - pa[ 2 ]; // vector V1 (pa->pb)
	A[ 1 ][ 0 ] = pc[ 0 ] - pa[ 0 ];
	A[ 1 ][ 1 ] = pc[ 1 ] - pa[ 1 ];
	A[ 1 ][ 2 ] = pc[ 2 ] - pa[ 2 ]; // vector V2 (pa->pc)

	cross( A[ 0 ], A[ 1 ], A[ 2 ] ); // vector V3 (V1 X V2)

	return 0.5 * sqrt( dot( A[ 2 ], A[ 2 ] ) ); // The area of [a,b,c].
}

REAL tetgenmesh::orient3dfast( REAL *pa, REAL *pb, REAL *pc, REAL *pd )
{
	REAL adx, bdx, cdx;
	REAL ady, bdy, cdy;
	REAL adz, bdz, cdz;

	adx = pa[ 0 ] - pd[ 0 ];
	bdx = pb[ 0 ] - pd[ 0 ];
	cdx = pc[ 0 ] - pd[ 0 ];
	ady = pa[ 1 ] - pd[ 1 ];
	bdy = pb[ 1 ] - pd[ 1 ];
	cdy = pc[ 1 ] - pd[ 1 ];
	adz = pa[ 2 ] - pd[ 2 ];
	bdz = pb[ 2 ] - pd[ 2 ];
	cdz = pc[ 2 ] - pd[ 2 ];

	return adx * ( bdy * cdz - bdz * cdy )
		+ bdx * ( cdy * adz - cdz * ady )
		+ cdx * ( ady * bdz - adz * bdy );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// interiorangle()    Return the interior angle (0 - 2 * PI) between vectors //
//                    o->p1 and o->p2.                                       //
//                                                                           //
// 'n' is the normal of the plane containing face (o, p1, p2).  The interior //
// angle is the total angle rotating from o->p1 around n to o->p2.  Exchange //
// the position of p1 and p2 will get the complement angle of the other one. //
// i.e., interiorangle(o, p1, p2) = 2 * PI - interiorangle(o, p2, p1).  Set  //
// 'n' be NULL if you only want the interior angle between 0 - PI.           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

REAL tetgenmesh::interiorangle( REAL* o, REAL* p1, REAL* p2, REAL* n )
{
	REAL v1[ 3 ], v2[ 3 ], np[ 3 ];
	REAL theta, costheta, lenlen;
	REAL ori, len1, len2;

	// Get the interior angle (0 - PI) between o->p1, and o->p2.
	v1[ 0 ] = p1[ 0 ] - o[ 0 ];
	v1[ 1 ] = p1[ 1 ] - o[ 1 ];
	v1[ 2 ] = p1[ 2 ] - o[ 2 ];
	v2[ 0 ] = p2[ 0 ] - o[ 0 ];
	v2[ 1 ] = p2[ 1 ] - o[ 1 ];
	v2[ 2 ] = p2[ 2 ] - o[ 2 ];
	len1 = sqrt( dot( v1, v1 ) );
	len2 = sqrt( dot( v2, v2 ) );
	lenlen = len1 * len2;

	costheta = dot( v1, v2 ) / lenlen;
	if(costheta > 1.0)
	{
		costheta = 1.0; // Roundoff. 
	}
	else if(costheta < -1.0)
	{
		costheta = -1.0; // Roundoff. 
	}
	theta = acos( costheta );
	if(n != NULL)
	{
		// Get a point above the face (o, p1, p2);
		np[ 0 ] = o[ 0 ] + n[ 0 ];
		np[ 1 ] = o[ 1 ] + n[ 1 ];
		np[ 2 ] = o[ 2 ] + n[ 2 ];
		// Adjust theta (0 - 2 * PI).
		ori = orient3d( p1, o, np, p2 );
		if(ori > 0.0)
		{
			theta = 2 * PIconstant - theta;
		}
	}

	return theta;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// projpt2edge()    Return the projection point from a point to an edge.     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::projpt2edge( point p, point e1, point e2, point prj, REAL* uv )
{
	REAL v1[ 3 ], v2[ 3 ];
	REAL len, l_p;

	v1[ 0 ] = e2[ 0 ] - e1[ 0 ];
	v1[ 1 ] = e2[ 1 ] - e1[ 1 ];
	v1[ 2 ] = e2[ 2 ] - e1[ 2 ];
	v2[ 0 ] = p[ 0 ] - e1[ 0 ];
	v2[ 1 ] = p[ 1 ] - e1[ 1 ];
	v2[ 2 ] = p[ 2 ] - e1[ 2 ];

	len = sqrt( dot( v1, v1 ) );
	v1[ 0 ] /= len;
	v1[ 1 ] /= len;
	v1[ 2 ] /= len;
	l_p = dot( v1, v2 );

	prj[ 0 ] = e1[ 0 ] + l_p * v1[ 0 ];
	prj[ 1 ] = e1[ 1 ] + l_p * v1[ 1 ];
	prj[ 2 ] = e1[ 2 ] + l_p * v1[ 2 ];

	if(uv)
	{
		for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
		{
			REAL puv, e1uv, e2uv, v1uv, v2uv;

			puv = pointgeomuv( p, tt );
			e1uv = pointgeomuv( e1, tt );
			e2uv = pointgeomuv( e2, tt );

			v1uv = e2uv - e1uv;
			v2uv = puv - e1uv;

			v1uv /= len;

			uv[ tt ] = e1uv + l_p * v1uv;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// projpt2face()    Return the projection point from a point to a face.      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::projpt2face( REAL* p, REAL* f1, REAL* f2, REAL* f3, REAL* prj )
{
	REAL fnormal[ 3 ], v1[ 3 ];
	REAL len, dist;

	// Get the unit face normal.
	facenormal( f1, f2, f3, fnormal, 1, NULL );
	len = sqrt( fnormal[ 0 ] * fnormal[ 0 ] + fnormal[ 1 ] * fnormal[ 1 ] +
				fnormal[ 2 ] * fnormal[ 2 ] );
	fnormal[ 0 ] /= len;
	fnormal[ 1 ] /= len;
	fnormal[ 2 ] /= len;
	// Get the vector v1 = |p - f1|.
	v1[ 0 ] = p[ 0 ] - f1[ 0 ];
	v1[ 1 ] = p[ 1 ] - f1[ 1 ];
	v1[ 2 ] = p[ 2 ] - f1[ 2 ];
	// Get the project distance.
	dist = dot( fnormal, v1 );

	// Get the project point.
	prj[ 0 ] = p[ 0 ] - dist * fnormal[ 0 ];
	prj[ 1 ] = p[ 1 ] - dist * fnormal[ 1 ];
	prj[ 2 ] = p[ 2 ] - dist * fnormal[ 2 ];
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// tetalldihedral()    Get all (six) dihedral angles of a tet.               //
//                                                                           //
// If 'cosdd' is not NULL, it returns the cosines of the 6 dihedral angles,  //
// the edge indices are given in the global array 'edge2ver'. If 'cosmaxd'   //
// (or 'cosmind') is not NULL, it returns the cosine of the maximal (or      //
// minimal) dihedral angle.                                                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

bool tetgenmesh::tetalldihedral( point pa, point pb, point pc, point pd,
								 REAL* cosdd, REAL* cosmaxd, REAL* cosmind )
{
	REAL N[ 4 ][ 3 ], vol, cosd, len;
	int f1 = 0, f2 = 0, i, j;

	vol = 0; // Check if the tet is valid or not.

	// Get four normals of faces of the tet.
	tetallnormal( pa, pb, pc, pd, N, &vol );

	if(vol > 0)
	{
		// Normalize the normals.
		for(i = 0; i < 4; i++)
		{
			len = sqrt( dot( N[ i ], N[ i ] ) );
			if(len != 0.0)
			{
				for(j = 0; j < 3; j++) N[ i ][ j ] /= len;
			}
			else
			{
				// There are degeneracies, such as duplicated vertices.
				vol = 0; //assert(0);
			}
		}
	}

	if(vol <= 0)
	{ // if (vol == 0.0) {
		// A degenerated tet or an inverted tet.
		facenormal( pc, pb, pd, N[ 0 ], 1, NULL );
		facenormal( pa, pc, pd, N[ 1 ], 1, NULL );
		facenormal( pb, pa, pd, N[ 2 ], 1, NULL );
		facenormal( pa, pb, pc, N[ 3 ], 1, NULL );
		// Normalize the normals.
		for(i = 0; i < 4; i++)
		{
			len = sqrt( dot( N[ i ], N[ i ] ) );
			if(len != 0.0)
			{
				for(j = 0; j < 3; j++) N[ i ][ j ] /= len;
			}
			else
			{
				// There are degeneracies, such as duplicated vertices.
				break; // Not a valid normal.
			}
		}
		if(i < 4)
		{
			// Do not calculate dihedral angles.
			// Set all angles be 0 degree. There will be no quality optimization for
			//   this tet! Use volume optimization to correct it.
			if(cosdd != NULL)
			{
				for(i = 0; i < 6; i++)
				{
					cosdd[ i ] = -1.0; // 180 degree.
				}
			}
			// This tet has zero volume.
			if(cosmaxd != NULL)
			{
				*cosmaxd = -1.0; // 180 degree.
			}
			if(cosmind != NULL)
			{
				*cosmind = -1.0; // 180 degree.
			}
			return false;
		}
	}

	// Calculate the cosine of the dihedral angles of the edges.
	for(i = 0; i < 6; i++)
	{
		switch(i)
		{
		case 0: f1 = 0; f2 = 1; break; // [c,d].
		case 1: f1 = 1; f2 = 2; break; // [a,d].
		case 2: f1 = 2; f2 = 3; break; // [a,b].
		case 3: f1 = 0; f2 = 3; break; // [b,c].
		case 4: f1 = 2; f2 = 0; break; // [b,d].
		case 5: f1 = 1; f2 = 3; break; // [a,c].
		}
		cosd = -dot( N[ f1 ], N[ f2 ] );
		if(cosd < -1.0) cosd = -1.0; // Rounding.
		if(cosd >  1.0) cosd = 1.0; // Rounding.
		if(cosdd) cosdd[ i ] = cosd;
		if(cosmaxd || cosmind)
		{
			if(i == 0)
			{
				if(cosmaxd) *cosmaxd = cosd;
				if(cosmind) *cosmind = cosd;
			}
			else
			{
				if(cosmaxd) *cosmaxd = cosd < *cosmaxd ? cosd : *cosmaxd;
				if(cosmind) *cosmind = cosd > *cosmind ? cosd : *cosmind;
			}
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// tetallnormal()    Get the in-normals of the four faces of a given tet.    //
//                                                                           //
// Let tet be abcd. N[4][3] returns the four normals, which are: N[0] cbd,   //
// N[1] acd, N[2] bad, N[3] abc (exactly corresponding to the face indices   //
// of the mesh data structure). These normals are unnormalized.              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::tetallnormal( point pa, point pb, point pc, point pd,
							   REAL N[ 4 ][ 3 ], REAL* volume )
{
	REAL A[ 4 ][ 4 ], rhs[ 4 ], D;
	int indx[ 4 ];
	int i, j;

	// get the entries of A[3][3].
	for(i = 0; i < 3; i++) A[ 0 ][ i ] = pa[ i ] - pd[ i ];  // d->a vec
	for(i = 0; i < 3; i++) A[ 1 ][ i ] = pb[ i ] - pd[ i ];  // d->b vec
	for(i = 0; i < 3; i++) A[ 2 ][ i ] = pc[ i ] - pd[ i ];  // d->c vec

	// Compute the inverse of matrix A, to get 3 normals of the 4 faces.
	if(lu_decmp( A, 3, indx, &D, 0 ))
	{ // Decompose the matrix just once.
		if(volume != NULL)
		{
			// Get the volume of the tet.
			*volume = fabs( ( A[ indx[ 0 ] ][ 0 ] * A[ indx[ 1 ] ][ 1 ] * A[ indx[ 2 ] ][ 2 ] ) ) / 6.0;
		}
		for(j = 0; j < 3; j++)
		{
			for(i = 0; i < 3; i++) rhs[ i ] = 0.0;
			rhs[ j ] = 1.0;  // Positive means the inside direction
			lu_solve( A, 3, indx, rhs, 0 );
			for(i = 0; i < 3; i++) N[ j ][ i ] = rhs[ i ];
		}
		// Get the fourth normal by summing up the first three.
		for(i = 0; i < 3; i++) N[ 3 ][ i ] = -N[ 0 ][ i ] - N[ 1 ][ i ] - N[ 2 ][ i ];
	}
	else
	{
		// The tet is degenerated.
		if(volume != NULL)
		{
			*volume = 0;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// tetaspectratio()    Calculate the aspect ratio of the tetrahedron.        //
//                                                                           //
// The aspect ratio of a tet is L/h, where L is the longest edge length, and //
// h is the shortest height of the tet.                                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

REAL tetgenmesh::tetaspectratio( point pa, point pb, point pc, point pd )
{
	REAL V[ 6 ][ 3 ], edgelength[ 6 ], longlen;
	REAL vda[ 3 ], vdb[ 3 ], vdc[ 3 ];
	REAL N[ 4 ][ 3 ], A[ 4 ][ 4 ], rhs[ 4 ], D;
	REAL H[ 4 ], volume, minheightinv;
	int indx[ 4 ];
	int i, j;

	// Set the edge vectors: V[0], ..., V[5]
	for(i = 0; i < 3; i++) V[ 0 ][ i ] = pa[ i ] - pd[ i ];
	for(i = 0; i < 3; i++) V[ 1 ][ i ] = pb[ i ] - pd[ i ];
	for(i = 0; i < 3; i++) V[ 2 ][ i ] = pc[ i ] - pd[ i ];
	for(i = 0; i < 3; i++) V[ 3 ][ i ] = pb[ i ] - pa[ i ];
	for(i = 0; i < 3; i++) V[ 4 ][ i ] = pc[ i ] - pb[ i ];
	for(i = 0; i < 3; i++) V[ 5 ][ i ] = pa[ i ] - pc[ i ];

	// Get the squares of the edge lengths.
	for(i = 0; i < 6; i++) edgelength[ i ] = dot( V[ i ], V[ i ] );

	// Calculate the longest and shortest edge length.
	longlen = edgelength[ 0 ];
	for(i = 1; i < 6; i++)
	{
		longlen = edgelength[ i ] > longlen ? edgelength[ i ] : longlen;
	}

	// Set the matrix A = [vda, vdb, vdc]^T.
	for(i = 0; i < 3; i++) A[ 0 ][ i ] = vda[ i ] = pa[ i ] - pd[ i ];
	for(i = 0; i < 3; i++) A[ 1 ][ i ] = vdb[ i ] = pb[ i ] - pd[ i ];
	for(i = 0; i < 3; i++) A[ 2 ][ i ] = vdc[ i ] = pc[ i ] - pd[ i ];
	// Lu-decompose the matrix A.
	lu_decmp( A, 3, indx, &D, 0 );
	// Get the volume of abcd.
	volume = ( A[ indx[ 0 ] ][ 0 ] * A[ indx[ 1 ] ][ 1 ] * A[ indx[ 2 ] ][ 2 ] ) / 6.0;
	// Check if it is zero.
	if(volume == 0.0) return 1.0e+200; // A degenerate tet.

	// Compute the 4 face normals (N[0], ..., N[3]).
	for(j = 0; j < 3; j++)
	{
		for(i = 0; i < 3; i++) rhs[ i ] = 0.0;
		rhs[ j ] = 1.0;  // Positive means the inside direction
		lu_solve( A, 3, indx, rhs, 0 );
		for(i = 0; i < 3; i++) N[ j ][ i ] = rhs[ i ];
	}
	// Get the fourth normal by summing up the first three.
	for(i = 0; i < 3; i++) N[ 3 ][ i ] = -N[ 0 ][ i ] - N[ 1 ][ i ] - N[ 2 ][ i ];
	// Normalized the normals.
	for(i = 0; i < 4; i++)
	{
		// H[i] is the inverse of the height of its corresponding face.
		H[ i ] = sqrt( dot( N[ i ], N[ i ] ) );
		// if (H[i] > 0.0) {
		//   for (j = 0; j < 3; j++) N[i][j] /= H[i];
		// }
	}
	// Get the radius of the inscribed sphere.
	// insradius = 1.0 / (H[0] + H[1] + H[2] + H[3]);
	// Get the biggest H[i] (corresponding to the smallest height).
	minheightinv = H[ 0 ];
	for(i = 1; i < 4; i++)
	{
		if(H[ i ] > minheightinv) minheightinv = H[ i ];
	}

	return sqrt( longlen ) * minheightinv;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// circumsphere()    Calculate the smallest circumsphere (center and radius) //
//                   of the given three or four points.                      //
//                                                                           //
// The circumsphere of four points (a tetrahedron) is unique if they are not //
// degenerate. If 'pd = NULL', the smallest circumsphere of three points is  //
// the diametral sphere of the triangle if they are not degenerate.          //
//                                                                           //
// Return TRUE if the input points are not degenerate and the circumcenter   //
// and circumradius are returned in 'cent' and 'radius' respectively if they //
// are not NULLs.  Otherwise, return FALSE, the four points are co-planar.   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

bool tetgenmesh::circumsphere( REAL* pa, REAL* pb, REAL* pc, REAL* pd,
							   REAL* cent, REAL* radius )
{
	REAL A[ 4 ][ 4 ], rhs[ 4 ], D;
	int indx[ 4 ];

	// Compute the coefficient matrix A (3x3).
	A[ 0 ][ 0 ] = pb[ 0 ] - pa[ 0 ];
	A[ 0 ][ 1 ] = pb[ 1 ] - pa[ 1 ];
	A[ 0 ][ 2 ] = pb[ 2 ] - pa[ 2 ];
	A[ 1 ][ 0 ] = pc[ 0 ] - pa[ 0 ];
	A[ 1 ][ 1 ] = pc[ 1 ] - pa[ 1 ];
	A[ 1 ][ 2 ] = pc[ 2 ] - pa[ 2 ];
	if(pd != NULL)
	{
		A[ 2 ][ 0 ] = pd[ 0 ] - pa[ 0 ];
		A[ 2 ][ 1 ] = pd[ 1 ] - pa[ 1 ];
		A[ 2 ][ 2 ] = pd[ 2 ] - pa[ 2 ];
	}
	else
	{
		cross( A[ 0 ], A[ 1 ], A[ 2 ] );
	}

	// Compute the right hand side vector b (3x1).
	rhs[ 0 ] = 0.5 * dot( A[ 0 ], A[ 0 ] );
	rhs[ 1 ] = 0.5 * dot( A[ 1 ], A[ 1 ] );
	if(pd != NULL)
	{
		rhs[ 2 ] = 0.5 * dot( A[ 2 ], A[ 2 ] );
	}
	else
	{
		rhs[ 2 ] = 0.0;
	}

	// Solve the 3 by 3 equations use LU decomposition with partial pivoting
	//   and backward and forward substitute..
	if(!lu_decmp( A, 3, indx, &D, 0 ))
	{
		if(radius != (REAL *)NULL) *radius = 0.0;
		return false;
	}
	lu_solve( A, 3, indx, rhs, 0 );
	if(cent != (REAL *)NULL)
	{
		cent[ 0 ] = pa[ 0 ] + rhs[ 0 ];
		cent[ 1 ] = pa[ 1 ] + rhs[ 1 ];
		cent[ 2 ] = pa[ 2 ] + rhs[ 2 ];
	}
	if(radius != (REAL *)NULL)
	{
		*radius = sqrt( rhs[ 0 ] * rhs[ 0 ] + rhs[ 1 ] * rhs[ 1 ] + rhs[ 2 ] * rhs[ 2 ] );
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// orthosphere()    Calulcate the orthosphere of four weighted points.       //
//                                                                           //
// A weighted point (p, P^2) can be interpreted as a sphere centered at the  //
// point 'p' with a radius 'P'. The 'height' of 'p' is pheight = p[0]^2 +    //
// p[1]^2 + p[2]^2 - P^2.                                                    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

bool tetgenmesh::orthosphere( REAL* pa, REAL* pb, REAL* pc, REAL* pd,
							  REAL  aheight, REAL bheight, REAL cheight,
							  REAL  dheight, REAL* orthocent, REAL* radius )
{
	REAL A[ 4 ][ 4 ], rhs[ 4 ], D;
	int indx[ 4 ];

	// Set the coefficient matrix A (4 x 4).
	A[ 0 ][ 0 ] = 1.0; A[ 0 ][ 1 ] = pa[ 0 ]; A[ 0 ][ 2 ] = pa[ 1 ]; A[ 0 ][ 3 ] = pa[ 2 ];
	A[ 1 ][ 0 ] = 1.0; A[ 1 ][ 1 ] = pb[ 0 ]; A[ 1 ][ 2 ] = pb[ 1 ]; A[ 1 ][ 3 ] = pb[ 2 ];
	A[ 2 ][ 0 ] = 1.0; A[ 2 ][ 1 ] = pc[ 0 ]; A[ 2 ][ 2 ] = pc[ 1 ]; A[ 2 ][ 3 ] = pc[ 2 ];
	A[ 3 ][ 0 ] = 1.0; A[ 3 ][ 1 ] = pd[ 0 ]; A[ 3 ][ 2 ] = pd[ 1 ]; A[ 3 ][ 3 ] = pd[ 2 ];

	// Set the right hand side vector (4 x 1).
	rhs[ 0 ] = 0.5 * aheight;
	rhs[ 1 ] = 0.5 * bheight;
	rhs[ 2 ] = 0.5 * cheight;
	rhs[ 3 ] = 0.5 * dheight;

	// Solve the 4 by 4 equations use LU decomposition with partial pivoting
	//   and backward and forward substitute..
	if(!lu_decmp( A, 4, indx, &D, 0 ))
	{
		if(radius != (REAL *)NULL) *radius = 0.0;
		return false;
	}
	lu_solve( A, 4, indx, rhs, 0 );

	if(orthocent != (REAL *)NULL)
	{
		orthocent[ 0 ] = rhs[ 1 ];
		orthocent[ 1 ] = rhs[ 2 ];
		orthocent[ 2 ] = rhs[ 3 ];
	}
	if(radius != (REAL *)NULL)
	{
		// rhs[0] = - rheight / 2;
		// rheight  = - 2 * rhs[0];
		//          =  r[0]^2 + r[1]^2 + r[2]^2 - radius^2
		// radius^2 = r[0]^2 + r[1]^2 + r[2]^2 -rheight
		//          = r[0]^2 + r[1]^2 + r[2]^2 + 2 * rhs[0]
		*radius = sqrt( rhs[ 1 ] * rhs[ 1 ] + rhs[ 2 ] * rhs[ 2 ] + rhs[ 3 ] * rhs[ 3 ]
						+ 2.0 * rhs[ 0 ] );
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// planelineint()    Calculate the intersection of a line and a plane.       //
//                                                                           //
// The equation of a plane (points P are on the plane with normal N and P3   //
// on the plane) can be written as: N dot (P - P3) = 0. The equation of the  //
// line (points P on the line passing through P1 and P2) can be written as:  //
// P = P1 + u (P2 - P1). The intersection of these two occurs when:          //
//   N dot (P1 + u (P2 - P1)) = N dot P3.                                    //
// Solving for u gives:                                                      //
//         N dot (P3 - P1)                                                   //
//   u = ------------------.                                                 //
//         N dot (P2 - P1)                                                   //
// If the denominator is 0 then N (the normal to the plane) is perpendicular //
// to the line.  Thus the line is either parallel to the plane and there are //
// no solutions or the line is on the plane in which case there are an infi- //
// nite number of solutions.                                                 //
//                                                                           //
// The plane is given by three points pa, pb, and pc, e1 and e2 defines the  //
// line. If u is non-zero, The intersection point (if exists) returns in ip. //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::planelineint( REAL* pa, REAL* pb, REAL* pc, REAL* e1, REAL* e2,
							   REAL* ip, REAL* u )
{
	REAL n[ 3 ], det, det1;

	// Calculate N.
	facenormal( pa, pb, pc, n, 1, NULL );
	// Calculate N dot (e2 - e1).
	det = n[ 0 ] * ( e2[ 0 ] - e1[ 0 ] ) + n[ 1 ] * ( e2[ 1 ] - e1[ 1 ] )
		+ n[ 2 ] * ( e2[ 2 ] - e1[ 2 ] );
	if(det != 0.0)
	{
		// Calculate N dot (pa - e1)
		det1 = n[ 0 ] * ( pa[ 0 ] - e1[ 0 ] ) + n[ 1 ] * ( pa[ 1 ] - e1[ 1 ] )
			+ n[ 2 ] * ( pa[ 2 ] - e1[ 2 ] );
		*u = det1 / det;
		ip[ 0 ] = e1[ 0 ] + *u * ( e2[ 0 ] - e1[ 0 ] );
		ip[ 1 ] = e1[ 1 ] + *u * ( e2[ 1 ] - e1[ 1 ] );
		ip[ 2 ] = e1[ 2 ] + *u * ( e2[ 2 ] - e1[ 2 ] );
	}
	else
	{
		*u = 0.0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// linelineint()    Calculate the intersection(s) of two line segments.      //
//                                                                           //
// Calculate the line segment [P, Q] that is the shortest route between two  //
// lines from A to B and C to D. Calculate also the values of tp and tq      //
// where: P = A + tp (B - A), and Q = C + tq (D - C).                        //
//                                                                           //
// Return 1 if the line segment exists. Otherwise, return 0.                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::linelineint( REAL* A, REAL* B, REAL* C, REAL* D, REAL* P,
							 REAL* Q, REAL* tp, REAL* tq )
{
	REAL vab[ 3 ], vcd[ 3 ], vca[ 3 ];
	REAL vab_vab, vcd_vcd, vab_vcd;
	REAL vca_vab, vca_vcd;
	REAL det, eps;
	int i;

	for(i = 0; i < 3; i++)
	{
		vab[ i ] = B[ i ] - A[ i ];
		vcd[ i ] = D[ i ] - C[ i ];
		vca[ i ] = A[ i ] - C[ i ];
	}

	vab_vab = dot( vab, vab );
	vcd_vcd = dot( vcd, vcd );
	vab_vcd = dot( vab, vcd );

	det = vab_vab * vcd_vcd - vab_vcd * vab_vcd;
	// Round the result.
	eps = det / ( fabs( vab_vab * vcd_vcd ) + fabs( vab_vcd * vab_vcd ) );
	if(eps < b->epsilon)
	{
		return 0;
	}

	vca_vab = dot( vca, vab );
	vca_vcd = dot( vca, vcd );

	*tp = ( vcd_vcd * ( -vca_vab ) + vab_vcd * vca_vcd ) / det;
	*tq = ( vab_vcd * ( -vca_vab ) + vab_vab * vca_vcd ) / det;

	for(i = 0; i < 3; i++) P[ i ] = A[ i ] + ( *tp ) * vab[ i ];
	for(i = 0; i < 3; i++) Q[ i ] = C[ i ] + ( *tq ) * vcd[ i ];

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// tetprismvol()    Calculate the volume of a tetrahedral prism in 4D.       //
//                                                                           //
// A tetrahedral prism is a convex uniform polychoron (four dimensional poly-//
// tope). It has 6 polyhedral cells: 2 tetrahedra connected by 4 triangular  //
// prisms. It has 14 faces: 8 triangular and 6 square. It has 16 edges and 8 //
// vertices. (Wikipedia).                                                    //
//                                                                           //
// Let 'p0', ..., 'p3' be four affinely independent points in R^3. They form //
// the lower tetrahedral facet of the prism.  The top tetrahedral facet is   //
// formed by four vertices, 'p4', ..., 'p7' in R^4, which is obtained by     //
// lifting each vertex of the lower facet into R^4 by a weight (height).  A  //
// canonical choice of the weights is the square of Euclidean norm of of the //
// points (vectors).                                                         //
//                                                                           //
//                                                                           //
// The return value is (4!) 24 times of the volume of the tetrahedral prism. //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

REAL tetgenmesh::tetprismvol( REAL* p0, REAL* p1, REAL* p2, REAL* p3 )
{
	REAL *p4, *p5, *p6, *p7;
	REAL w4, w5, w6, w7;
	REAL vol[ 4 ];

	p4 = p0;
	p5 = p1;
	p6 = p2;
	p7 = p3;

	// TO DO: these weights can be pre-calculated!
	w4 = dot( p0, p0 );
	w5 = dot( p1, p1 );
	w6 = dot( p2, p2 );
	w7 = dot( p3, p3 );

	// Calculate the volume of the tet-prism.
	vol[ 0 ] = orient4d( p5, p6, p4, p3, p7, w5, w6, w4, 0, w7 );
	vol[ 1 ] = orient4d( p3, p6, p2, p0, p1, 0, w6, 0, 0, 0 );
	vol[ 2 ] = orient4d( p4, p6, p3, p0, p1, w4, w6, 0, 0, 0 );
	vol[ 3 ] = orient4d( p6, p5, p4, p3, p1, w6, w5, w4, 0, 0 );

	return fabs( vol[ 0 ] ) + fabs( vol[ 1 ] ) + fabs( vol[ 2 ] ) + fabs( vol[ 3 ] );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// calculateabovepoint()    Calculate a point above a facet in 'dummypoint'. //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

bool tetgenmesh::calculateabovepoint( arraypool *facpoints, point *ppa,
									  point *ppb, point *ppc )
{
	point *ppt, pa, pb, pc;
	REAL v1[ 3 ], v2[ 3 ], n[ 3 ];
	REAL lab, len, A, area;
	REAL x, y, z;
	int i;

	ppt = (point *)fastlookup( facpoints, 0 );
	pa = *ppt; // a is the first point.
	pb = pc = NULL; // Avoid compiler warnings.

	// Get a point b s.t. the length of [a, b] is maximal.
	lab = 0;
	for(i = 1; i < facpoints->objects; i++)
	{
		ppt = (point *)fastlookup( facpoints, i );
		x = ( *ppt )[ 0 ] - pa[ 0 ];
		y = ( *ppt )[ 1 ] - pa[ 1 ];
		z = ( *ppt )[ 2 ] - pa[ 2 ];
		len = x * x + y * y + z * z;
		if(len > lab)
		{
			lab = len;
			pb = *ppt;
		}
	}
	lab = sqrt( lab );
	if(lab == 0)
	{
		if(!b->quiet)
		{
			myprintf( "Warning:  All points of a facet are coincident with %d.\n",
					pointmark( pa ) );
		}
		return false;
	}

	// Get a point c s.t. the area of [a, b, c] is maximal.
	v1[ 0 ] = pb[ 0 ] - pa[ 0 ];
	v1[ 1 ] = pb[ 1 ] - pa[ 1 ];
	v1[ 2 ] = pb[ 2 ] - pa[ 2 ];
	A = 0;
	for(i = 1; i < facpoints->objects; i++)
	{
		ppt = (point *)fastlookup( facpoints, i );
		v2[ 0 ] = ( *ppt )[ 0 ] - pa[ 0 ];
		v2[ 1 ] = ( *ppt )[ 1 ] - pa[ 1 ];
		v2[ 2 ] = ( *ppt )[ 2 ] - pa[ 2 ];
		cross( v1, v2, n );
		area = dot( n, n );
		if(area > A)
		{
			A = area;
			pc = *ppt;
		}
	}
	if(A == 0)
	{
		// All points are collinear. No above point.
		if(!b->quiet)
		{
			myprintf( "Warning:  All points of a facet are collinaer with [%d, %d].\n",
					pointmark( pa ), pointmark( pb ) );
		}
		return false;
	}

	// Calculate an above point of this facet.
	facenormal( pa, pb, pc, n, 1, NULL );
	len = sqrt( dot( n, n ) );
	n[ 0 ] /= len;
	n[ 1 ] /= len;
	n[ 2 ] /= len;
	lab /= 2.0; // Half the maximal length.
	dummypoint[ 0 ] = pa[ 0 ] + lab * n[ 0 ];
	dummypoint[ 1 ] = pa[ 1 ] + lab * n[ 1 ];
	dummypoint[ 2 ] = pa[ 2 ] + lab * n[ 2 ];

	if(ppa != NULL)
	{
		// Return the three points.
		*ppa = pa;
		*ppb = pb;
		*ppc = pc;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Calculate an above point. It lies above the plane containing  the subface //
//   [a,b,c], and save it in dummypoint. Moreover, the vector pa->dummypoint //
//   is the normal of the plane.                                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::calculateabovepoint4( point pa, point pb, point pc, point pd )
{
	REAL n1[ 3 ], n2[ 3 ], *norm;
	REAL len, len1, len2;

	// Select a base.
	facenormal( pa, pb, pc, n1, 1, NULL );
	len1 = sqrt( dot( n1, n1 ) );
	facenormal( pa, pb, pd, n2, 1, NULL );
	len2 = sqrt( dot( n2, n2 ) );
	if(len1 > len2)
	{
		norm = n1;
		len = len1;
	}
	else
	{
		norm = n2;
		len = len2;
	}
	norm[ 0 ] /= len;
	norm[ 1 ] /= len;
	norm[ 2 ] /= len;
	len = distance( pa, pb );
	dummypoint[ 0 ] = pa[ 0 ] + len * norm[ 0 ];
	dummypoint[ 1 ] = pa[ 1 ] + len * norm[ 1 ];
	dummypoint[ 2 ] = pa[ 2 ] + len * norm[ 2 ];
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// report_overlapping_facets()    Report two overlapping facets.             //
//                                                                           //
// Two subfaces, f1 [a, b, c] and f2 [a, b, d], share the same edge [a, b].  //
// 'dihedang' is the dihedral angle between these two facets. It must less   //
// than the variable 'b->facet_overlap_angle_tol'.                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::report_overlapping_facets( face *f1, face *f2, REAL dihedang )
{
	point pa, pb, pc, pd;

	pa = sorg( *f1 );
	pb = sdest( *f1 );
	pc = sapex( *f1 );
	pd = sapex( *f2 );

	if(pc != pd)
	{
		myprintf( "Found two %s self-intersecting facets.\n",
				dihedang > 0 ? "nearly" : "exactly" );
		myprintf( "  1st: [%d, %d, %d] #%d\n",
				pointmark( pa ), pointmark( pb ), pointmark( pc ), shellmark( *f1 ) );
		myprintf( "  2nd: [%d, %d, %d] #%d\n",
				pointmark( pa ), pointmark( pb ), pointmark( pd ), shellmark( *f2 ) );
		if(dihedang > 0)
		{
			myprintf( "The dihedral angle between them is %g degree.\n",
					dihedang / PIconstant * 180.0 );
			myprintf( "Hint:  You may use -p/# to decrease the dihedral angle" );
			myprintf( "  tolerance %g (degree).\n", b->facet_overlap_ang_tol );
		}
	}
	else
	{
		if(shellmark( *f1 ) != shellmark( *f2 ))
		{
			// Two identical faces from two different facet.
			myprintf( "Found two overlapping facets.\n" );
		}
		else
		{
			myprintf( "Found two duplicated facets.\n" );
		}
		myprintf( "  1st: [%d, %d, %d] #%d\n",
				pointmark( pa ), pointmark( pb ), pointmark( pc ), shellmark( *f1 ) );
		myprintf( "  2nd: [%d, %d, %d] #%d\n",
				pointmark( pa ), pointmark( pb ), pointmark( pd ), shellmark( *f2 ) );
	}

	terminatetetgen( this, 3 );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// report_selfint_edge()    Report a self-intersection at an edge.           //
//                                                                           //
// The edge 'e1'->'e2' and the tetrahedron 'itet' intersect. 'dir' indicates //
// that the edge intersects the tet at its origin vertex (ACROSSVERTEX), or  //
// its current face (ACROSSFACE), or its current edge (ACROSSEDGE).          //
// If 'iedge' is not NULL, it is either a segment or a subface that contains //
// the edge 'e1'->'e2'.  It is used to report the geometry entity.           //
//                                                                           //
// Since it is a self-intersection, the vertex, edge or face of 'itet' that  //
// is intersecting with this edge must be an input vertex, a segment, or a   //
// subface, respectively.                                                    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::report_selfint_edge( point e1, point e2, face *iedge,
									 triface* itet, enum interresult dir )
{
	point forg = NULL, fdest = NULL, fapex = NULL;
	int etype = 0, geomtag = 0, facemark = 0;

	if(iedge != NULL)
	{
		if(iedge->sh[ 5 ] != NULL)
		{
			etype = 2;  // A subface
			forg = e1;
			fdest = e2;
			fapex = sapex( *iedge );
			facemark = shellmark( *iedge );
		}
		else
		{
			etype = 1;  // A segment
			forg = farsorg( *iedge );
			fdest = farsdest( *iedge );
			// Get a facet containing this segment.
			face parentsh;
			spivot( *iedge, parentsh );
			if(parentsh.sh != NULL)
			{
				facemark = shellmark( parentsh );
			}
		}
		geomtag = shellmark( *iedge );
	}

	if(dir == SHAREEDGE)
	{
		// Two edges (segments) are coincide.
		face colseg;
		tsspivot1( *itet, colseg );
		if(etype == 1)
		{
			if(colseg.sh != iedge->sh)
			{
				face parentsh;
				spivot( colseg, parentsh );
				myprintf( "PLC Error:  Two segments are overlapping.\n" );
				myprintf( "  Segment 1: [%d, %d] #%d (%d)\n", pointmark( sorg( colseg ) ),
						pointmark( sdest( colseg ) ), shellmark( colseg ),
						parentsh.sh ? shellmark( parentsh ) : 0 );
				myprintf( "  Segment 2: [%d, %d] #%d (%d)\n", pointmark( forg ),
						pointmark( fdest ), geomtag, facemark );
			}
			else
			{
				// Two identical segments. Why report it?
				terminatetetgen( this, 2 );
			}
		}
		else if(etype == 2)
		{
			myprintf( "PLC Error:  A segment lies in a facet.\n" );
			myprintf( "  Segment: [%d, %d] #%d\n", pointmark( sorg( colseg ) ),
					pointmark( sdest( colseg ) ), shellmark( colseg ) );
			myprintf( "  Facet:   [%d,%d,%d] #%d\n", pointmark( forg ),
					pointmark( fdest ), pointmark( fapex ), geomtag );
		}
	}
	else if(dir == SHAREFACE)
	{
		// Two triangles (subfaces) are coincide.
		face colface;
		tspivot( *itet, colface );
		if(etype == 2)
		{
			if(colface.sh != iedge->sh)
			{
				myprintf( "PLC Error:  Two facets are overlapping.\n" );
				myprintf( "  Facet 1:  [%d,%d,%d] #%d\n", pointmark( forg ),
						pointmark( fdest ), pointmark( fapex ), geomtag );
				myprintf( "  Facet 2:  [%d,%d,%d] #%d\n", pointmark( sorg( colface ) ),
						pointmark( sdest( colface ) ), pointmark( sapex( colface ) ),
						shellmark( colface ) );
			}
			else
			{
				// Two identical subfaces. Why report it?
				terminatetetgen( this, 2 );
			}
		}
		else
		{
			terminatetetgen( this, 2 );
		}
	}
	else if(dir == ACROSSVERT)
	{
		point pp = dest( *itet );
		if(( pointtype( pp ) == RIDGEVERTEX ) || ( pointtype( pp ) == FACETVERTEX )
			|| ( pointtype( pp ) == VOLVERTEX ))
		{
			if(etype == 1)
			{
				myprintf( "PLC Error:  A vertex lies in a segment.\n" );
				myprintf( "  Vertex:  [%d] (%g,%g,%g).\n", pointmark( pp ), pp[ 0 ], pp[ 1 ], pp[ 2 ] );
				myprintf( "  Segment: [%d, %d] #%d (%d)\n", pointmark( forg ),
						pointmark( fdest ), geomtag, facemark );
			}
			else if(etype == 2)
			{
				myprintf( "PLC Error:  A vertex lies in a facet.\n" );
				myprintf( "  Vertex: [%d] (%g,%g,%g).\n", pointmark( pp ), pp[ 0 ], pp[ 1 ], pp[ 2 ] );
				myprintf( "  Facet:  [%d,%d,%d] #%d\n", pointmark( forg ), pointmark( fdest ),
						pointmark( fapex ), geomtag );
			}
		}
		else if(pointtype( pp ) == FREESEGVERTEX)
		{
			face parentseg, parentsh;
			sdecode( point2sh( pp ), parentseg );
			spivot( parentseg, parentsh );
			if(parentseg.sh != NULL)
			{
				point p1 = farsorg( parentseg );
				point p2 = farsdest( parentseg );
				if(etype == 1)
				{
					myprintf( "PLC Error:  Two segments intersect at point (%g,%g,%g).\n",
							pp[ 0 ], pp[ 1 ], pp[ 2 ] );
					myprintf( "  Segment 1: [%d, %d], #%d (%d)\n", pointmark( forg ),
							pointmark( fdest ), geomtag, facemark );
					myprintf( "  Segment 2: [%d, %d], #%d (%d)\n", pointmark( p1 ),
							pointmark( p2 ), shellmark( parentseg ),
							parentsh.sh ? shellmark( parentsh ) : 0 );
				}
				else if(etype == 2)
				{
					myprintf( "PLC Error:  A segment and a facet intersect at point" );
					myprintf( " (%g,%g,%g).\n", pp[ 0 ], pp[ 1 ], pp[ 2 ] );
					myprintf( "  Segment: [%d, %d], #%d (%d)\n", pointmark( p1 ),
							pointmark( p2 ), shellmark( parentseg ),
							parentsh.sh ? shellmark( parentsh ) : 0 );
					myprintf( "  Facet:   [%d,%d,%d] #%d\n", pointmark( forg ),
							pointmark( fdest ), pointmark( fapex ), geomtag );
				}
			}
			else
			{
				terminatetetgen( this, 2 ); // Report a bug.
			}
		}
		else if(pointtype( pp ) == FREEFACETVERTEX)
		{
			face parentsh;
			sdecode( point2sh( pp ), parentsh );
			if(parentsh.sh != NULL)
			{
				point p1 = sorg( parentsh );
				point p2 = sdest( parentsh );
				point p3 = sapex( parentsh );
				if(etype == 1)
				{
					myprintf( "PLC Error:  A segment and a facet intersect at point" );
					myprintf( " (%g,%g,%g).\n", pp[ 0 ], pp[ 1 ], pp[ 2 ] );
					myprintf( "  Segment : [%d, %d], #%d (%d)\n", pointmark( forg ),
							pointmark( fdest ), geomtag, facemark );
					myprintf( "  Facet   : [%d, %d, %d]  #%d.\n", pointmark( p1 ),
							pointmark( p2 ), pointmark( p3 ), shellmark( parentsh ) );
				}
				else if(etype == 2)
				{
					myprintf( "PLC Error:  Two facets intersect at point (%g,%g,%g).\n",
							pp[ 0 ], pp[ 1 ], pp[ 2 ] );
					myprintf( "  Facet 1: [%d, %d, %d] #%d.\n", pointmark( forg ),
							pointmark( fdest ), pointmark( fapex ), geomtag );
					myprintf( "  Facet 2: [%d, %d, %d] #%d.\n", pointmark( p1 ),
							pointmark( p2 ), pointmark( p3 ), shellmark( parentsh ) );
				}
			}
			else
			{
				terminatetetgen( this, 2 ); // Report a bug.
			}
		}
		else if(pointtype( pp ) == FREEVOLVERTEX)
		{
			// This is not a PLC error. 
			// We should shift the vertex.
			// not down yet.
			terminatetetgen( this, 2 ); // Report a bug.
		}
		else
		{
			terminatetetgen( this, 2 ); // Report a bug.
		}
		//terminatetetgen( this, 3 );
	}
	else if(dir == ACROSSEDGE)
	{
		if(issubseg( *itet ))
		{
			face checkseg;
			tsspivot1( *itet, checkseg );
			face parentsh;
			spivot( checkseg, parentsh );
			// Calulcate the intersecting point.
			point p1 = sorg( checkseg );
			point p2 = sdest( checkseg );
			REAL P[ 3 ], Q[ 3 ], tp = 0, tq = 0;
			linelineint( e1, e2, p1, p2, P, Q, &tp, &tq );
			if(etype == 1)
			{
				myprintf( "PLC Error:  Two segments intersect at point (%g,%g,%g).\n",
						P[ 0 ], P[ 1 ], P[ 2 ] );
				myprintf( "  Segment 1: [%d, %d] #%d (%d)\n", pointmark( forg ),
						pointmark( fdest ), geomtag, facemark );
				myprintf( "  Segment 2: [%d, %d] #%d (%d)\n", pointmark( p1 ),
						pointmark( p2 ), shellmark( checkseg ),
						parentsh.sh ? shellmark( parentsh ) : 0 );
			}
			else if(etype == 2)
			{
				myprintf( "PLC Error:  A segment and a facet intersect at point" );
				myprintf( " (%g,%g,%g).\n", P[ 0 ], P[ 1 ], P[ 2 ] );
				myprintf( "  Segment: [%d, %d] #%d (%d)\n", pointmark( p1 ),
						pointmark( p2 ), shellmark( checkseg ),
						parentsh.sh ? shellmark( parentsh ) : 0 );
				myprintf( "  Facet:   [%d, %d, %d] #%d.\n", pointmark( forg ),
						pointmark( fdest ), pointmark( fapex ), geomtag );
			}
			//terminatetetgen( this, 3 );
		}
	}
	else if(dir == ACROSSFACE)
	{
		if(issubface( *itet ))
		{
			face checksh;
			tspivot( *itet, checksh );
			point p1 = sorg( checksh );
			point p2 = sdest( checksh );
			point p3 = sapex( checksh );
			REAL ip[ 3 ], u = 0;
			planelineint( p1, p2, p3, e1, e2, ip, &u );
			if(etype == 1)
			{
				myprintf( "PLC Error:  A segment and a facet intersect at point" );
				myprintf( " (%g,%g,%g).\n", ip[ 0 ], ip[ 1 ], ip[ 2 ] );
				myprintf( "  Segment: [%d, %d] #%d (%d)\n", pointmark( forg ),
						pointmark( fdest ), geomtag, facemark );
				myprintf( "  Facet:   [%d, %d, %d] #%d.\n", pointmark( p1 ),
						pointmark( p2 ), pointmark( p3 ), shellmark( checksh ) );
			}
			else if(etype == 2)
			{
				myprintf( "PLC Error:  Two facets intersect at point (%g,%g,%g).\n",
						ip[ 0 ], ip[ 1 ], ip[ 2 ] );
				myprintf( "  Facet 1: [%d, %d, %d] #%d.\n", pointmark( forg ),
						pointmark( fdest ), pointmark( fapex ), geomtag );
				myprintf( "  Facet 2: [%d, %d, %d] #%d.\n", pointmark( p1 ),
						pointmark( p2 ), pointmark( p3 ), shellmark( checksh ) );
			}
			terminatetetgen( this, 3 );
		}
	}
	else
	{
		// An unknown 'dir'.
		terminatetetgen( this, 2 );
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// report_selfint_face()    Report a self-intersection at a facet.           //
//                                                                           //
// The triangle with vertices 'p1', 'p2', and 'p3' intersects with the edge  //
// of the tetrahedra 'iedge'. The intersection type is reported by 'intflag',//
// 'types', and 'poss'.                                                      //
//                                                                           //
// This routine ASSUMES (1) the triangle (p1,p2,p3) must belong to a facet,  //
// 'sface' is a subface of the same facet; and (2) 'iedge' must be either a  //
// segment or an edge of another facet.                                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::report_selfint_face( point p1, point p2, point p3, face* sface,
									 triface* iedge, int intflag, int* types, int* poss )
{
	face iface;
	point e1 = NULL, e2 = NULL, e3 = NULL;
	int etype = 0, geomtag = 0, facemark = 0;

	geomtag = shellmark( *sface );

	if(issubface( *iedge ))
	{
		tspivot( *iedge, iface );
		e1 = sorg( iface );
		e2 = sdest( iface );
		e3 = sapex( iface );
		etype = 2;
		facemark = geomtag;
	}
	else if(issubseg( *iedge ))
	{
		tsspivot1( *iedge, iface );
		e1 = farsorg( iface );
		e2 = farsdest( iface );
		etype = 1;
		face parentsh;
		spivot( iface, parentsh );
		facemark = shellmark( parentsh );
	}
	else
	{
		terminatetetgen( this, 2 );
	}

	if(intflag == 2)
	{
		// The triangle and the edge intersect only at one point.
		REAL ip[ 3 ], u = 0;
		planelineint( p1, p2, p3, e1, e2, ip, &u );
		if(( types[ 0 ] == (int)ACROSSFACE ) ||
			( types[ 0 ] == (int)ACROSSEDGE ))
		{
			// The triangle and the edge intersect in their interiors. 
			if(etype == 1)
			{
				myprintf( "PLC Error:  A segment and a facet intersect at point" );
				myprintf( " (%g,%g,%g).\n", ip[ 0 ], ip[ 1 ], ip[ 2 ] );
				myprintf( "  Segment: [%d,%d] #%d (%d)\n", pointmark( e1 ), pointmark( e2 ),
						shellmark( iface ), facemark );
				myprintf( "  Facet:   [%d,%d,%d] #%d\n", pointmark( p1 ),
						pointmark( p2 ), pointmark( p3 ), geomtag );
			}
			else
			{
				myprintf( "PLC Error:  Two facets intersect at point" );
				myprintf( " (%g,%g,%g).\n", ip[ 0 ], ip[ 1 ], ip[ 2 ] );
				myprintf( "  Facet 1: [%d,%d,%d] #%d\n", pointmark( e1 ), pointmark( e2 ),
						pointmark( sorg( iface ) ), shellmark( iface ) );
				myprintf( "  Facet 2: [%d,%d,%d] #%d\n", pointmark( p1 ),
						pointmark( p2 ), pointmark( p3 ), geomtag );
			}
		}
		else if(types[ 0 ] == (int)ACROSSVERT)
		{
			// A vertex of the triangle and the edge intersect.
			point crosspt = NULL;
			if(poss[ 0 ] == 0)
			{
				crosspt = p1;
			}
			else if(poss[ 0 ] == 1)
			{
				crosspt = p2;
			}
			else if(poss[ 0 ] == 2)
			{
				crosspt = p3;
			}
			else
			{
				terminatetetgen( this, 2 );
			}
			if(!issteinerpoint( crosspt ))
			{
				if(etype == 1)
				{
					myprintf( "PLC Error:  A vertex and a segment intersect at (%g,%g,%g)\n",
							crosspt[ 0 ], crosspt[ 1 ], crosspt[ 2 ] );
					myprintf( "  Vertex:  #%d\n", pointmark( crosspt ) );
					myprintf( "  Segment: [%d,%d] #%d (%d)\n", pointmark( e1 ), pointmark( e2 ),
							shellmark( iface ), facemark );
				}
				else
				{
					myprintf( "PLC Error:  A vertex and a facet intersect at (%g,%g,%g)\n",
							crosspt[ 0 ], crosspt[ 1 ], crosspt[ 2 ] );
					myprintf( "  Vertex:  #%d\n", pointmark( crosspt ) );
					myprintf( "  Facet:   [%d,%d,%d] #%d\n", pointmark( p1 ),
							pointmark( p2 ), pointmark( p3 ), geomtag );
				}
			}
			else
			{
				// It is a Steiner point. To be processed.
				terminatetetgen( this, 2 );
			}
		}
		else if(( types[ 0 ] == (int)TOUCHFACE ) ||
				 ( types[ 0 ] == (int)TOUCHEDGE ))
		{
			// The triangle and a vertex of the edge intersect.
			point touchpt = NULL;
			if(poss[ 1 ] == 0)
			{
				touchpt = org( *iedge );
			}
			else if(poss[ 1 ] == 1)
			{
				touchpt = dest( *iedge );
			}
			else
			{
				terminatetetgen( this, 2 );
			}
			if(!issteinerpoint( touchpt ))
			{
				myprintf( "PLC Error:  A vertex and a facet intersect at (%g,%g,%g)\n",
						touchpt[ 0 ], touchpt[ 1 ], touchpt[ 2 ] );
				myprintf( "  Vertex:  #%d\n", pointmark( touchpt ) );
				myprintf( "  Facet:   [%d,%d,%d] #%d\n", pointmark( p1 ),
						pointmark( p2 ), pointmark( p3 ), geomtag );
			}
			else
			{
				// It is a Steiner point. To be processed.
				terminatetetgen( this, 2 );
			}
		}
		else if(types[ 0 ] == (int)SHAREVERT)
		{
			terminatetetgen( this, 2 );
		}
		else
		{
			terminatetetgen( this, 2 );
		}
	}
	else if(intflag == 4)
	{
		if(types[ 0 ] == (int)SHAREFACE)
		{
			myprintf( "PLC Error:  Two facets are overlapping.\n" );
			myprintf( "  Facet 1:   [%d,%d,%d] #%d\n", pointmark( e1 ),
					pointmark( e2 ), pointmark( e3 ), facemark );
			myprintf( "  Facet 2:   [%d,%d,%d] #%d\n", pointmark( p1 ),
					pointmark( p2 ), pointmark( p3 ), geomtag );
		}
		else
		{
			terminatetetgen( this, 2 );
		}
	}
	else
	{
		terminatetetgen( this, 2 );
	}

	terminatetetgen( this, 3 );
	return 0;
}

////                                                                       ////
////                                                                       ////
//// geom_cxx /////////////////////////////////////////////////////////////////

//// flip_cxx /////////////////////////////////////////////////////////////////
////                                                                       ////
////                                                                       ////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// flip23()    Perform a 2-to-3 flip (face-to-edge flip).                    //
//                                                                           //
// 'fliptets' is an array of three tets (handles), where the [0] and [1] are  //
// [a,b,c,d] and [b,a,c,e].  The three new tets: [e,d,a,b], [e,d,b,c], and   //
// [e,d,c,a] are returned in [0], [1], and [2] of 'fliptets'.  As a result,  //
// The face [a,b,c] is removed, and the edge [d,e] is created.               //
//                                                                           //
// If 'hullflag' > 0, hull tets may be involved in this flip, i.e., one of   //
// the five vertices may be 'dummypoint'. There are two canonical cases:     //
//   (1) d is 'dummypoint', then all three new tets are hull tets.  If e is  //
//       'dummypoint', we reconfigure e to d, i.e., turn it up-side down.    //
//   (2) c is 'dummypoint', then two new tets: [e,d,b,c] and [e,d,c,a], are  //
//       hull tets. If a or b is 'dummypoint', we reconfigure it to c, i.e., //
//       rotate the three input tets counterclockwisely (right-hand rule)    //
//       until a or b is in c's position.                                    //
//                                                                           //
// If 'fc->enqflag' is set, convex hull faces will be queued for flipping.   //
// In particular, if 'fc->enqflag' is 1, it is called by incrementalflip()   //
// after the insertion of a new point.  It is assumed that 'd' is the new    //
// point. IN this case, only link faces of 'd' are queued.                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::flip23( triface* fliptets, int hullflag, flipconstraints *fc )
{
	triface topcastets[ 3 ], botcastets[ 3 ];
	triface newface, casface;
	point pa, pb, pc, pd, pe;
	REAL attrib, volume;
	int dummyflag = 0;  // range = {-1, 0, 1, 2}.
	int i;

	if(hullflag > 0)
	{
		// Check if e is dummypoint.
		if(oppo( fliptets[ 1 ] ) == dummypoint)
		{
			// Swap the two old tets.
			newface = fliptets[ 0 ];
			fliptets[ 0 ] = fliptets[ 1 ];
			fliptets[ 1 ] = newface;
			dummyflag = -1;  // d is dummypoint.
		}
		else
		{
			// Check if either a or b is dummypoint.
			if(org( fliptets[ 0 ] ) == dummypoint)
			{
				dummyflag = 1; // a is dummypoint.
				enextself( fliptets[ 0 ] );
				eprevself( fliptets[ 1 ] );
			}
			else if(dest( fliptets[ 0 ] ) == dummypoint)
			{
				dummyflag = 2; // b is dummypoint.
				eprevself( fliptets[ 0 ] );
				enextself( fliptets[ 1 ] );
			}
			else
			{
				dummyflag = 0; // either c or d may be dummypoint.
			}
		}
	}

	pa = org( fliptets[ 0 ] );
	pb = dest( fliptets[ 0 ] );
	pc = apex( fliptets[ 0 ] );
	pd = oppo( fliptets[ 0 ] );
	pe = oppo( fliptets[ 1 ] );

	flip23count++;

	// Get the outer boundary faces.
	for(i = 0; i < 3; i++)
	{
		fnext( fliptets[ 0 ], topcastets[ i ] );
		enextself( fliptets[ 0 ] );
	}
	for(i = 0; i < 3; i++)
	{
		fnext( fliptets[ 1 ], botcastets[ i ] );
		eprevself( fliptets[ 1 ] );
	}

	// Re-use fliptets[0] and fliptets[1].
	fliptets[ 0 ].ver = 11;
	fliptets[ 1 ].ver = 11;
	setelemmarker( fliptets[ 0 ].tet, 0 ); // Clear all flags.
	setelemmarker( fliptets[ 1 ].tet, 0 );
	// NOTE: the element attributes and volume constraint remain unchanged.
	if(checksubsegflag)
	{
		// Dealloc the space to subsegments.
		if(fliptets[ 0 ].tet[ 8 ] != NULL)
		{
			tet2segpool->dealloc( (shellface *)fliptets[ 0 ].tet[ 8 ] );
			fliptets[ 0 ].tet[ 8 ] = NULL;
		}
		if(fliptets[ 1 ].tet[ 8 ] != NULL)
		{
			tet2segpool->dealloc( (shellface *)fliptets[ 1 ].tet[ 8 ] );
			fliptets[ 1 ].tet[ 8 ] = NULL;
		}
	}
	if(checksubfaceflag)
	{
		// Dealloc the space to subfaces.
		if(fliptets[ 0 ].tet[ 9 ] != NULL)
		{
			tet2subpool->dealloc( (shellface *)fliptets[ 0 ].tet[ 9 ] );
			fliptets[ 0 ].tet[ 9 ] = NULL;
		}
		if(fliptets[ 1 ].tet[ 9 ] != NULL)
		{
			tet2subpool->dealloc( (shellface *)fliptets[ 1 ].tet[ 9 ] );
			fliptets[ 1 ].tet[ 9 ] = NULL;
		}
	}
	// Create a new tet.
	maketetrahedron( &( fliptets[ 2 ] ) );
	// The new tet have the same attributes from the old tet.
	for(i = 0; i < numelemattrib; i++)
	{
		attrib = elemattribute( fliptets[ 0 ].tet, i );
		setelemattribute( fliptets[ 2 ].tet, i, attrib );
	}
	if(b->varvolume)
	{
		volume = volumebound( fliptets[ 0 ].tet );
		setvolumebound( fliptets[ 2 ].tet, volume );
	}

	if(hullflag > 0)
	{
		// Check if d is dummytet.
		if(pd != dummypoint)
		{
			setvertices( fliptets[ 0 ], pe, pd, pa, pb ); // [e,d,a,b] *
			setvertices( fliptets[ 1 ], pe, pd, pb, pc ); // [e,d,b,c] *
			// Check if c is dummypoint.
			if(pc != dummypoint)
			{
				setvertices( fliptets[ 2 ], pe, pd, pc, pa );  // [e,d,c,a] *
			}
			else
			{
				setvertices( fliptets[ 2 ], pd, pe, pa, pc ); // [d,e,a,c]
				esymself( fliptets[ 2 ] );                    // [e,d,c,a] *
			}
			// The hullsize does not change.
		}
		else
		{
			// d is dummypoint.
			setvertices( fliptets[ 0 ], pa, pb, pe, pd ); // [a,b,e,d]
			setvertices( fliptets[ 1 ], pb, pc, pe, pd ); // [b,c,e,d]
			setvertices( fliptets[ 2 ], pc, pa, pe, pd ); // [c,a,e,d]
			// Adjust the faces to [e,d,a,b], [e,d,b,c], [e,d,c,a] *
			for(i = 0; i < 3; i++)
			{
				eprevesymself( fliptets[ i ] );
				enextself( fliptets[ i ] );
			}
			// We deleted one hull tet, and created three hull tets.
			hullsize += 2;
		}
	}
	else
	{
		setvertices( fliptets[ 0 ], pe, pd, pa, pb ); // [e,d,a,b] *
		setvertices( fliptets[ 1 ], pe, pd, pb, pc ); // [e,d,b,c] *
		setvertices( fliptets[ 2 ], pe, pd, pc, pa ); // [e,d,c,a] *
	}

	if(fc->remove_ndelaunay_edge)
	{ // calc_tetprism_vol
		REAL volneg[ 2 ], volpos[ 3 ], vol_diff;
		if(pd != dummypoint)
		{
			if(pc != dummypoint)
			{
				volpos[ 0 ] = tetprismvol( pe, pd, pa, pb );
				volpos[ 1 ] = tetprismvol( pe, pd, pb, pc );
				volpos[ 2 ] = tetprismvol( pe, pd, pc, pa );
				volneg[ 0 ] = tetprismvol( pa, pb, pc, pd );
				volneg[ 1 ] = tetprismvol( pb, pa, pc, pe );
			}
			else
			{ // pc == dummypoint
				volpos[ 0 ] = tetprismvol( pe, pd, pa, pb );
				volpos[ 1 ] = 0.;
				volpos[ 2 ] = 0.;
				volneg[ 0 ] = 0.;
				volneg[ 1 ] = 0.;
			}
		}
		else
		{ // pd == dummypoint.
			volpos[ 0 ] = 0.;
			volpos[ 1 ] = 0.;
			volpos[ 2 ] = 0.;
			volneg[ 0 ] = 0.;
			volneg[ 1 ] = tetprismvol( pb, pa, pc, pe );
		}
		vol_diff = volpos[ 0 ] + volpos[ 1 ] + volpos[ 2 ] - volneg[ 0 ] - volneg[ 1 ];
		fc->tetprism_vol_sum += vol_diff; // Update the total sum.
	}

	// Bond three new tets together.
	for(i = 0; i < 3; i++)
	{
		esym( fliptets[ i ], newface );
		bond( newface, fliptets[ ( i + 1 ) % 3 ] );
	}
	// Bond to top outer boundary faces (at [a,b,c,d]).
	for(i = 0; i < 3; i++)
	{
		eorgoppo( fliptets[ i ], newface ); // At edges [b,a], [c,b], [a,c].
		bond( newface, topcastets[ i ] );
	}
	// Bond bottom outer boundary faces (at [b,a,c,e]).
	for(i = 0; i < 3; i++)
	{
		edestoppo( fliptets[ i ], newface ); // At edges [a,b], [b,c], [c,a].
		bond( newface, botcastets[ i ] );
	}

	if(checksubsegflag)
	{
		// Bond subsegments if there are.
		// Each new tet has 5 edges to be checked (except the edge [e,d]). 
		face checkseg;
		// The middle three: [a,b], [b,c], [c,a].
		for(i = 0; i < 3; i++)
		{
			if(issubseg( topcastets[ i ] ))
			{
				tsspivot1( topcastets[ i ], checkseg );
				eorgoppo( fliptets[ i ], newface );
				tssbond1( newface, checkseg );
				sstbond1( checkseg, newface );
				if(fc->chkencflag & 1)
				{
					enqueuesubface( badsubsegs, &checkseg );
				}
			}
		}
		// The top three: [d,a], [d,b], [d,c]. Two tets per edge.
		for(i = 0; i < 3; i++)
		{
			eprev( topcastets[ i ], casface );
			if(issubseg( casface ))
			{
				tsspivot1( casface, checkseg );
				enext( fliptets[ i ], newface );
				tssbond1( newface, checkseg );
				sstbond1( checkseg, newface );
				esym( fliptets[ ( i + 2 ) % 3 ], newface );
				eprevself( newface );
				tssbond1( newface, checkseg );
				sstbond1( checkseg, newface );
				if(fc->chkencflag & 1)
				{
					enqueuesubface( badsubsegs, &checkseg );
				}
			}
		}
		// The bot three: [a,e], [b,e], [c,e]. Two tets per edge.
		for(i = 0; i < 3; i++)
		{
			enext( botcastets[ i ], casface );
			if(issubseg( casface ))
			{
				tsspivot1( casface, checkseg );
				eprev( fliptets[ i ], newface );
				tssbond1( newface, checkseg );
				sstbond1( checkseg, newface );
				esym( fliptets[ ( i + 2 ) % 3 ], newface );
				enextself( newface );
				tssbond1( newface, checkseg );
				sstbond1( checkseg, newface );
				if(fc->chkencflag & 1)
				{
					enqueuesubface( badsubsegs, &checkseg );
				}
			}
		}
	} // if (checksubsegflag)

	if(checksubfaceflag)
	{
		// Bond 6 subfaces if there are.
		face checksh;
		for(i = 0; i < 3; i++)
		{
			if(issubface( topcastets[ i ] ))
			{
				tspivot( topcastets[ i ], checksh );
				eorgoppo( fliptets[ i ], newface );
				sesymself( checksh );
				tsbond( newface, checksh );
				if(fc->chkencflag & 2)
				{
					enqueuesubface( badsubfacs, &checksh );
				}
			}
		}
		for(i = 0; i < 3; i++)
		{
			if(issubface( botcastets[ i ] ))
			{
				tspivot( botcastets[ i ], checksh );
				edestoppo( fliptets[ i ], newface );
				sesymself( checksh );
				tsbond( newface, checksh );
				if(fc->chkencflag & 2)
				{
					enqueuesubface( badsubfacs, &checksh );
				}
			}
		}
	} // if (checksubfaceflag)

	if(fc->chkencflag & 4)
	{
		// Put three new tets into check list.
		for(i = 0; i < 3; i++)
		{
			enqueuetetrahedron( &( fliptets[ i ] ) );
		}
	}

	// Update the point-to-tet map.
	setpoint2tet( pa, (tetrahedron)fliptets[ 0 ].tet );
	setpoint2tet( pb, (tetrahedron)fliptets[ 0 ].tet );
	setpoint2tet( pc, (tetrahedron)fliptets[ 1 ].tet );
	setpoint2tet( pd, (tetrahedron)fliptets[ 0 ].tet );
	setpoint2tet( pe, (tetrahedron)fliptets[ 0 ].tet );

	if(hullflag > 0)
	{
		if(dummyflag != 0)
		{
			// Restore the original position of the points (for flipnm()).
			if(dummyflag == -1)
			{
				// Reverse the edge.
				for(i = 0; i < 3; i++)
				{
					esymself( fliptets[ i ] );
				}
				// Swap the last two new tets.
				newface = fliptets[ 1 ];
				fliptets[ 1 ] = fliptets[ 2 ];
				fliptets[ 2 ] = newface;
			}
			else
			{
				// either a or b were swapped.
				if(dummyflag == 1)
				{
					// a is dummypoint.
					newface = fliptets[ 0 ];
					fliptets[ 0 ] = fliptets[ 2 ];
					fliptets[ 2 ] = fliptets[ 1 ];
					fliptets[ 1 ] = newface;
				}
				else
				{ // dummyflag == 2
					// b is dummypoint.
					newface = fliptets[ 0 ];
					fliptets[ 0 ] = fliptets[ 1 ];
					fliptets[ 1 ] = fliptets[ 2 ];
					fliptets[ 2 ] = newface;
				}
			}
		}
	}

	if(fc->enqflag > 0)
	{
		// Queue faces which may be locally non-Delaunay.  
		for(i = 0; i < 3; i++)
		{
			eprevesym( fliptets[ i ], newface );
			flippush( flipstack, &newface );
		}
		if(fc->enqflag > 1)
		{
			for(i = 0; i < 3; i++)
			{
				enextesym( fliptets[ i ], newface );
				flippush( flipstack, &newface );
			}
		}
	}

	recenttet = fliptets[ 0 ];
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// flip32()    Perform a 3-to-2 flip (edge-to-face flip).                    //
//                                                                           //
// 'fliptets' is an array of three tets (handles),  which are [e,d,a,b],     //
// [e,d,b,c], and [e,d,c,a].  The two new tets: [a,b,c,d] and [b,a,c,e] are  //
// returned in [0] and [1] of 'fliptets'.  As a result, the edge [e,d] is    //
// replaced by the face [a,b,c].                                             //
//                                                                           //
// If 'hullflag' > 0, hull tets may be involved in this flip, i.e., one of   //
// the five vertices may be 'dummypoint'. There are two canonical cases:     //
//   (1) d is 'dummypoint', then [a,b,c,d] is hull tet. If e is 'dummypoint',//
//       we reconfigure e to d, i.e., turnover it.                           //
//   (2) c is 'dummypoint' then both [a,b,c,d] and [b,a,c,e] are hull tets.  //
//       If a or b is 'dummypoint', we reconfigure it to c, i.e., rotate the //
//       three old tets counterclockwisely (right-hand rule) until a or b    //
//       is in c's position.                                                 //
//                                                                           //
// If 'fc->enqflag' is set, convex hull faces will be queued for flipping.   //
// In particular, if 'fc->enqflag' is 1, it is called by incrementalflip()   //
// after the insertion of a new point.  It is assumed that 'a' is the new    //
// point. In this case, only link faces of 'a' are queued.                   //
//                                                                           //
// If 'checksubfaceflag' is on (global variable), and assume [e,d] is not a  //
// segment. There may be two (interior) subfaces sharing at [e,d], which are //
// [e,d,p] and [e,d,q], where the pair (p,q) may be either (a,b), or (b,c),  //
// or (c,a)  In such case, a 2-to-2 flip is performed on these two subfaces  //
// and two new subfaces [p,q,e] and [p,q,d] are created. They are inserted   //
// back into the tetrahedralization.                                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::flip32( triface* fliptets, int hullflag, flipconstraints *fc )
{
	triface topcastets[ 3 ], botcastets[ 3 ];
	triface newface, casface;
	face flipshs[ 3 ];
	face checkseg;
	point pa, pb, pc, pd, pe;
	REAL attrib, volume;
	int dummyflag = 0;  // Rangle = {-1, 0, 1, 2}
	int spivot = -1, scount = 0; // for flip22()
	int t1ver;
	int i, j;

	if(hullflag > 0)
	{
		// Check if e is 'dummypoint'.
		if(org( fliptets[ 0 ] ) == dummypoint)
		{
			// Reverse the edge.
			for(i = 0; i < 3; i++)
			{
				esymself( fliptets[ i ] );
			}
			// Swap the last two tets.
			newface = fliptets[ 1 ];
			fliptets[ 1 ] = fliptets[ 2 ];
			fliptets[ 2 ] = newface;
			dummyflag = -1; // e is dummypoint.
		}
		else
		{
			// Check if a or b is the 'dummypoint'.
			if(apex( fliptets[ 0 ] ) == dummypoint)
			{
				dummyflag = 1;  // a is dummypoint.
				newface = fliptets[ 0 ];
				fliptets[ 0 ] = fliptets[ 1 ];
				fliptets[ 1 ] = fliptets[ 2 ];
				fliptets[ 2 ] = newface;
			}
			else if(apex( fliptets[ 1 ] ) == dummypoint)
			{
				dummyflag = 2;  // b is dummypoint.
				newface = fliptets[ 0 ];
				fliptets[ 0 ] = fliptets[ 2 ];
				fliptets[ 2 ] = fliptets[ 1 ];
				fliptets[ 1 ] = newface;
			}
			else
			{
				dummyflag = 0;  // either c or d may be dummypoint.
			}
		}
	}

	pa = apex( fliptets[ 0 ] );
	pb = apex( fliptets[ 1 ] );
	pc = apex( fliptets[ 2 ] );
	pd = dest( fliptets[ 0 ] );
	pe = org( fliptets[ 0 ] );

	flip32count++;

	// Get the outer boundary faces.
	for(i = 0; i < 3; i++)
	{
		eorgoppo( fliptets[ i ], casface );
		fsym( casface, topcastets[ i ] );
	}
	for(i = 0; i < 3; i++)
	{
		edestoppo( fliptets[ i ], casface );
		fsym( casface, botcastets[ i ] );
	}

	if(checksubfaceflag)
	{
		// Check if there are interior subfaces at the edge [e,d].
		for(i = 0; i < 3; i++)
		{
			tspivot( fliptets[ i ], flipshs[ i ] );
			if(flipshs[ i ].sh != NULL)
			{
				// Found an interior subface.
				stdissolve( flipshs[ i ] ); // Disconnect the sub-tet bond.
				scount++;
			}
			else
			{
				spivot = i;
			}
		}
	}

	// Re-use fliptets[0] and fliptets[1].
	fliptets[ 0 ].ver = 11;
	fliptets[ 1 ].ver = 11;
	setelemmarker( fliptets[ 0 ].tet, 0 ); // Clear all flags.
	setelemmarker( fliptets[ 1 ].tet, 0 );
	if(checksubsegflag)
	{
		// Dealloc the space to subsegments.
		if(fliptets[ 0 ].tet[ 8 ] != NULL)
		{
			tet2segpool->dealloc( (shellface *)fliptets[ 0 ].tet[ 8 ] );
			fliptets[ 0 ].tet[ 8 ] = NULL;
		}
		if(fliptets[ 1 ].tet[ 8 ] != NULL)
		{
			tet2segpool->dealloc( (shellface *)fliptets[ 1 ].tet[ 8 ] );
			fliptets[ 1 ].tet[ 8 ] = NULL;
		}
	}
	if(checksubfaceflag)
	{
		// Dealloc the space to subfaces.
		if(fliptets[ 0 ].tet[ 9 ] != NULL)
		{
			tet2subpool->dealloc( (shellface *)fliptets[ 0 ].tet[ 9 ] );
			fliptets[ 0 ].tet[ 9 ] = NULL;
		}
		if(fliptets[ 1 ].tet[ 9 ] != NULL)
		{
			tet2subpool->dealloc( (shellface *)fliptets[ 1 ].tet[ 9 ] );
			fliptets[ 1 ].tet[ 9 ] = NULL;
		}
	}
	if(checksubfaceflag)
	{
		if(scount > 0)
		{
			// The element attributes and volume constraint must be set correctly.
			// There are two subfaces involved in this flip. The three tets are
			//   separated into two different regions, one may be exterior. The
			//   first region has two tets, and the second region has only one.
			//   The two created tets must be in the same region as the first region. 
			//   The element attributes and volume constraint must be set correctly.
			//assert(spivot != -1);
			// The tet fliptets[spivot] is in the first region.
			for(j = 0; j < 2; j++)
			{
				for(i = 0; i < numelemattrib; i++)
				{
					attrib = elemattribute( fliptets[ spivot ].tet, i );
					setelemattribute( fliptets[ j ].tet, i, attrib );
				}
				if(b->varvolume)
				{
					volume = volumebound( fliptets[ spivot ].tet );
					setvolumebound( fliptets[ j ].tet, volume );
				}
			}
		}
	}
	// Delete an old tet.
	tetrahedrondealloc( fliptets[ 2 ].tet );

	if(hullflag > 0)
	{
		// Check if c is dummypointc.
		if(pc != dummypoint)
		{
			// Check if d is dummypoint.
			if(pd != dummypoint)
			{
				// No hull tet is involved.
			}
			else
			{
				// We deleted three hull tets, and created one hull tet.
				hullsize -= 2;
			}
			setvertices( fliptets[ 0 ], pa, pb, pc, pd );
			setvertices( fliptets[ 1 ], pb, pa, pc, pe );
		}
		else
		{
			// c is dummypoint. The two new tets are hull tets.
			setvertices( fliptets[ 0 ], pb, pa, pd, pc );
			setvertices( fliptets[ 1 ], pa, pb, pe, pc );
			// Adjust badc -> abcd.
			esymself( fliptets[ 0 ] );
			// Adjust abec -> bace.
			esymself( fliptets[ 1 ] );
			// The hullsize does not change.
		}
	}
	else
	{
		setvertices( fliptets[ 0 ], pa, pb, pc, pd );
		setvertices( fliptets[ 1 ], pb, pa, pc, pe );
	}

	if(fc->remove_ndelaunay_edge)
	{ // calc_tetprism_vol
		REAL volneg[ 3 ], volpos[ 2 ], vol_diff;
		if(pc != dummypoint)
		{
			if(pd != dummypoint)
			{
				volneg[ 0 ] = tetprismvol( pe, pd, pa, pb );
				volneg[ 1 ] = tetprismvol( pe, pd, pb, pc );
				volneg[ 2 ] = tetprismvol( pe, pd, pc, pa );
				volpos[ 0 ] = tetprismvol( pa, pb, pc, pd );
				volpos[ 1 ] = tetprismvol( pb, pa, pc, pe );
			}
			else
			{ // pd == dummypoint
				volneg[ 0 ] = 0.;
				volneg[ 1 ] = 0.;
				volneg[ 2 ] = 0.;
				volpos[ 0 ] = 0.;
				volpos[ 1 ] = tetprismvol( pb, pa, pc, pe );
			}
		}
		else
		{ // pc == dummypoint.
			volneg[ 0 ] = tetprismvol( pe, pd, pa, pb );
			volneg[ 1 ] = 0.;
			volneg[ 2 ] = 0.;
			volpos[ 0 ] = 0.;
			volpos[ 1 ] = 0.;
		}
		vol_diff = volpos[ 0 ] + volpos[ 1 ] - volneg[ 0 ] - volneg[ 1 ] - volneg[ 2 ];
		fc->tetprism_vol_sum += vol_diff; // Update the total sum.
	}

	// Bond abcd <==> bace.
	bond( fliptets[ 0 ], fliptets[ 1 ] );
	// Bond new faces to top outer boundary faces (at abcd).
	for(i = 0; i < 3; i++)
	{
		esym( fliptets[ 0 ], newface );
		bond( newface, topcastets[ i ] );
		enextself( fliptets[ 0 ] );
	}
	// Bond new faces to bottom outer boundary faces (at bace).
	for(i = 0; i < 3; i++)
	{
		esym( fliptets[ 1 ], newface );
		bond( newface, botcastets[ i ] );
		eprevself( fliptets[ 1 ] );
	}

	if(checksubsegflag)
	{
		// Bond 9 segments to new (flipped) tets.
		for(i = 0; i < 3; i++)
		{ // edges a->b, b->c, c->a.      
			if(issubseg( topcastets[ i ] ))
			{
				tsspivot1( topcastets[ i ], checkseg );
				tssbond1( fliptets[ 0 ], checkseg );
				sstbond1( checkseg, fliptets[ 0 ] );
				tssbond1( fliptets[ 1 ], checkseg );
				sstbond1( checkseg, fliptets[ 1 ] );
				if(fc->chkencflag & 1)
				{
					enqueuesubface( badsubsegs, &checkseg );
				}
			}
			enextself( fliptets[ 0 ] );
			eprevself( fliptets[ 1 ] );
		}
		// The three top edges.
		for(i = 0; i < 3; i++)
		{ // edges b->d, c->d, a->d.
			esym( fliptets[ 0 ], newface );
			eprevself( newface );
			enext( topcastets[ i ], casface );
			if(issubseg( casface ))
			{
				tsspivot1( casface, checkseg );
				tssbond1( newface, checkseg );
				sstbond1( checkseg, newface );
				if(fc->chkencflag & 1)
				{
					enqueuesubface( badsubsegs, &checkseg );
				}
			}
			enextself( fliptets[ 0 ] );
		}
		// The three bot edges.
		for(i = 0; i < 3; i++)
		{ // edges b<-e, c<-e, a<-e.
			esym( fliptets[ 1 ], newface );
			enextself( newface );
			eprev( botcastets[ i ], casface );
			if(issubseg( casface ))
			{
				tsspivot1( casface, checkseg );
				tssbond1( newface, checkseg );
				sstbond1( checkseg, newface );
				if(fc->chkencflag & 1)
				{
					enqueuesubface( badsubsegs, &checkseg );
				}
			}
			eprevself( fliptets[ 1 ] );
		}
	} // if (checksubsegflag)

	if(checksubfaceflag)
	{
		face checksh;
		// Bond the top three casing subfaces.
		for(i = 0; i < 3; i++)
		{ // At edges [b,a], [c,b], [a,c]
			if(issubface( topcastets[ i ] ))
			{
				tspivot( topcastets[ i ], checksh );
				esym( fliptets[ 0 ], newface );
				sesymself( checksh );
				tsbond( newface, checksh );
				if(fc->chkencflag & 2)
				{
					enqueuesubface( badsubfacs, &checksh );
				}
			}
			enextself( fliptets[ 0 ] );
		}
		// Bond the bottom three casing subfaces.
		for(i = 0; i < 3; i++)
		{ // At edges [a,b], [b,c], [c,a]
			if(issubface( botcastets[ i ] ))
			{
				tspivot( botcastets[ i ], checksh );
				esym( fliptets[ 1 ], newface );
				sesymself( checksh );
				tsbond( newface, checksh );
				if(fc->chkencflag & 2)
				{
					enqueuesubface( badsubfacs, &checksh );
				}
			}
			eprevself( fliptets[ 1 ] );
		}

		if(scount > 0)
		{
			face flipfaces[ 2 ];
			// Perform a 2-to-2 flip in subfaces.
			flipfaces[ 0 ] = flipshs[ ( spivot + 1 ) % 3 ];
			flipfaces[ 1 ] = flipshs[ ( spivot + 2 ) % 3 ];
			sesymself( flipfaces[ 1 ] );
			flip22( flipfaces, 0, fc->chkencflag );
			// Connect the flipped subfaces to flipped tets.
			// First go to the corresponding flipping edge.
			//   Re-use top- and botcastets[0].
			topcastets[ 0 ] = fliptets[ 0 ];
			botcastets[ 0 ] = fliptets[ 1 ];
			for(i = 0; i < ( ( spivot + 1 ) % 3 ); i++)
			{
				enextself( topcastets[ 0 ] );
				eprevself( botcastets[ 0 ] );
			}
			// Connect the top subface to the top tets.
			esymself( topcastets[ 0 ] );
			sesymself( flipfaces[ 0 ] );
			// Check if there already exists a subface.
			tspivot( topcastets[ 0 ], checksh );
			if(checksh.sh == NULL)
			{
				tsbond( topcastets[ 0 ], flipfaces[ 0 ] );
				fsymself( topcastets[ 0 ] );
				sesymself( flipfaces[ 0 ] );
				tsbond( topcastets[ 0 ], flipfaces[ 0 ] );
			}
			else
			{
				// An invalid 2-to-2 flip. Report a bug.
				terminatetetgen( this, 2 );
			}
			// Connect the bot subface to the bottom tets.
			esymself( botcastets[ 0 ] );
			sesymself( flipfaces[ 1 ] );
			// Check if there already exists a subface.
			tspivot( botcastets[ 0 ], checksh );
			if(checksh.sh == NULL)
			{
				tsbond( botcastets[ 0 ], flipfaces[ 1 ] );
				fsymself( botcastets[ 0 ] );
				sesymself( flipfaces[ 1 ] );
				tsbond( botcastets[ 0 ], flipfaces[ 1 ] );
			}
			else
			{
				// An invalid 2-to-2 flip. Report a bug.
				terminatetetgen( this, 2 );
			}
		} // if (scount > 0)
	} // if (checksubfaceflag)

	if(fc->chkencflag & 4)
	{
		// Put two new tets into check list.
		for(i = 0; i < 2; i++)
		{
			enqueuetetrahedron( &( fliptets[ i ] ) );
		}
	}

	setpoint2tet( pa, (tetrahedron)fliptets[ 0 ].tet );
	setpoint2tet( pb, (tetrahedron)fliptets[ 0 ].tet );
	setpoint2tet( pc, (tetrahedron)fliptets[ 0 ].tet );
	setpoint2tet( pd, (tetrahedron)fliptets[ 0 ].tet );
	setpoint2tet( pe, (tetrahedron)fliptets[ 1 ].tet );

	if(hullflag > 0)
	{
		if(dummyflag != 0)
		{
			// Restore the original position of the points (for flipnm()).
			if(dummyflag == -1)
			{
				// e were dummypoint. Swap the two new tets.
				newface = fliptets[ 0 ];
				fliptets[ 0 ] = fliptets[ 1 ];
				fliptets[ 1 ] = newface;
			}
			else
			{
				// a or b was dummypoint.
				if(dummyflag == 1)
				{
					eprevself( fliptets[ 0 ] );
					enextself( fliptets[ 1 ] );
				}
				else
				{ // dummyflag == 2
					enextself( fliptets[ 0 ] );
					eprevself( fliptets[ 1 ] );
				}
			}
		}
	}

	if(fc->enqflag > 0)
	{
		// Queue faces which may be locally non-Delaunay.
		// pa = org(fliptets[0]); // 'a' may be a new vertex.
		enextesym( fliptets[ 0 ], newface );
		flippush( flipstack, &newface );
		eprevesym( fliptets[ 1 ], newface );
		flippush( flipstack, &newface );
		if(fc->enqflag > 1)
		{
			//pb = dest(fliptets[0]);
			eprevesym( fliptets[ 0 ], newface );
			flippush( flipstack, &newface );
			enextesym( fliptets[ 1 ], newface );
			flippush( flipstack, &newface );
			//pc = apex(fliptets[0]);
			esym( fliptets[ 0 ], newface );
			flippush( flipstack, &newface );
			esym( fliptets[ 1 ], newface );
			flippush( flipstack, &newface );
		}
	}

	recenttet = fliptets[ 0 ];
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// flip41()    Perform a 4-to-1 flip (Remove a vertex).                      //
//                                                                           //
// 'fliptets' is an array of four tetrahedra in the star of the removing     //
// vertex 'p'. Let the four vertices in the star of p be a, b, c, and d. The //
// four tets in 'fliptets' are: [p,d,a,b], [p,d,b,c], [p,d,c,a], and [a,b,c, //
// p].  On return, 'fliptets[0]' is the new tet [a,b,c,d].                   //
//                                                                           //
// If 'hullflag' is set (> 0), one of the five vertices may be 'dummypoint'. //
// The 'hullsize' may be changed.  Note that p may be dummypoint.  In this   //
// case, four hull tets are replaced by one real tet.                        //
//                                                                           //
// If 'checksubface' flag is set (>0), it is possible that there are three   //
// interior subfaces connecting at p.  If so, a 3-to-1 flip is performed to  //
// to remove p from the surface triangulation.                               //
//                                                                           //
// If it is called by the routine incrementalflip(), we assume that d is the //
// newly inserted vertex.                                                    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::flip41( triface* fliptets, int hullflag, flipconstraints *fc )
{
	triface topcastets[ 3 ], botcastet;
	triface newface, neightet;
	face flipshs[ 4 ];
	point pa, pb, pc, pd, pp;
	int dummyflag = 0; // in {0, 1, 2, 3, 4}
	int spivot = -1, scount = 0;
	int t1ver;
	int i;

	pa = org( fliptets[ 3 ] );
	pb = dest( fliptets[ 3 ] );
	pc = apex( fliptets[ 3 ] );
	pd = dest( fliptets[ 0 ] );
	pp = org( fliptets[ 0 ] ); // The removing vertex.

	flip41count++;

	// Get the outer boundary faces.
	for(i = 0; i < 3; i++)
	{
		enext( fliptets[ i ], topcastets[ i ] );
		fnextself( topcastets[ i ] ); // [d,a,b,#], [d,b,c,#], [d,c,a,#]
		enextself( topcastets[ i ] ); // [a,b,d,#], [b,c,d,#], [c,a,d,#]
	}
	fsym( fliptets[ 3 ], botcastet ); // [b,a,c,#]

	if(checksubfaceflag)
	{
		// Check if there are three subfaces at 'p'.
		//   Re-use 'newface'.
		for(i = 0; i < 3; i++)
		{
			fnext( fliptets[ 3 ], newface ); // [a,b,p,d],[b,c,p,d],[c,a,p,d].
			tspivot( newface, flipshs[ i ] );
			if(flipshs[ i ].sh != NULL)
			{
				spivot = i; // Remember this subface.
				scount++;
			}
			enextself( fliptets[ 3 ] );
		}
		if(scount > 0)
		{
			// There are three subfaces connecting at p.
			if(scount < 3)
			{
				// The new subface is one of {[a,b,d], [b,c,d], [c,a,d]}.
				// Go to the tet containing the three subfaces.
				fsym( topcastets[ spivot ], neightet );
				// Get the three subfaces connecting at p.
				for(i = 0; i < 3; i++)
				{
					esym( neightet, newface );
					tspivot( newface, flipshs[ i ] );
					eprevself( neightet );
				}
			}
			else
			{
				spivot = 3; // The new subface is [a,b,c].
			}
		}
	} // if (checksubfaceflag)


	// Re-use fliptets[0] for [a,b,c,d].
	fliptets[ 0 ].ver = 11;
	setelemmarker( fliptets[ 0 ].tet, 0 ); // Clean all flags.
	// NOTE: the element attributes and volume constraint remain unchanged.
	if(checksubsegflag)
	{
		// Dealloc the space to subsegments.
		if(fliptets[ 0 ].tet[ 8 ] != NULL)
		{
			tet2segpool->dealloc( (shellface *)fliptets[ 0 ].tet[ 8 ] );
			fliptets[ 0 ].tet[ 8 ] = NULL;
		}
	}
	if(checksubfaceflag)
	{
		// Dealloc the space to subfaces.
		if(fliptets[ 0 ].tet[ 9 ] != NULL)
		{
			tet2subpool->dealloc( (shellface *)fliptets[ 0 ].tet[ 9 ] );
			fliptets[ 0 ].tet[ 9 ] = NULL;
		}
	}
	// Delete the other three tets.
	for(i = 1; i < 4; i++)
	{
		tetrahedrondealloc( fliptets[ i ].tet );
	}

	if(pp != dummypoint)
	{
		// Mark the point pp as unused.
		setpointtype( pp, UNUSEDVERTEX );
		unuverts++;
	}

	// Create the new tet [a,b,c,d].
	if(hullflag > 0)
	{
		// One of the five vertices may be 'dummypoint'.
		if(pa == dummypoint)
		{
			// pa is dummypoint.
			setvertices( fliptets[ 0 ], pc, pb, pd, pa );
			esymself( fliptets[ 0 ] );  // [b,c,a,d]
			eprevself( fliptets[ 0 ] ); // [a,b,c,d]
			dummyflag = 1;
		}
		else if(pb == dummypoint)
		{
			setvertices( fliptets[ 0 ], pa, pc, pd, pb );
			esymself( fliptets[ 0 ] );  // [c,a,b,d]
			enextself( fliptets[ 0 ] ); // [a,b,c,d]
			dummyflag = 2;
		}
		else if(pc == dummypoint)
		{
			setvertices( fliptets[ 0 ], pb, pa, pd, pc );
			esymself( fliptets[ 0 ] );  // [a,b,c,d]
			dummyflag = 3;
		}
		else if(pd == dummypoint)
		{
			setvertices( fliptets[ 0 ], pa, pb, pc, pd );
			dummyflag = 4;
		}
		else
		{
			setvertices( fliptets[ 0 ], pa, pb, pc, pd );
			if(pp == dummypoint)
			{
				dummyflag = -1;
			}
			else
			{
				dummyflag = 0;
			}
		}
		if(dummyflag > 0)
		{
			// We deleted 3 hull tets, and create 1 hull tet.
			hullsize -= 2;
		}
		else if(dummyflag < 0)
		{
			// We deleted 4 hull tets.
			hullsize -= 4;
			// meshedges does not change.
		}
	}
	else
	{
		setvertices( fliptets[ 0 ], pa, pb, pc, pd );
	}

	if(fc->remove_ndelaunay_edge)
	{ // calc_tetprism_vol
		REAL volneg[ 4 ], volpos[ 1 ], vol_diff;
		if(dummyflag > 0)
		{
			if(pa == dummypoint)
			{
				volneg[ 0 ] = 0.;
				volneg[ 1 ] = tetprismvol( pp, pd, pb, pc );
				volneg[ 2 ] = 0.;
				volneg[ 3 ] = 0.;
			}
			else if(pb == dummypoint)
			{
				volneg[ 0 ] = 0.;
				volneg[ 1 ] = 0.;
				volneg[ 2 ] = tetprismvol( pp, pd, pc, pa );
				volneg[ 3 ] = 0.;
			}
			else if(pc == dummypoint)
			{
				volneg[ 0 ] = tetprismvol( pp, pd, pa, pb );
				volneg[ 1 ] = 0.;
				volneg[ 2 ] = 0.;
				volneg[ 3 ] = 0.;
			}
			else
			{ // pd == dummypoint
				volneg[ 0 ] = 0.;
				volneg[ 1 ] = 0.;
				volneg[ 2 ] = 0.;
				volneg[ 3 ] = tetprismvol( pa, pb, pc, pp );
			}
			volpos[ 0 ] = 0.;
		}
		else if(dummyflag < 0)
		{
			volneg[ 0 ] = 0.;
			volneg[ 1 ] = 0.;
			volneg[ 2 ] = 0.;
			volneg[ 3 ] = 0.;
			volpos[ 0 ] = tetprismvol( pa, pb, pc, pd );
		}
		else
		{
			volneg[ 0 ] = tetprismvol( pp, pd, pa, pb );
			volneg[ 1 ] = tetprismvol( pp, pd, pb, pc );
			volneg[ 2 ] = tetprismvol( pp, pd, pc, pa );
			volneg[ 3 ] = tetprismvol( pa, pb, pc, pp );
			volpos[ 0 ] = tetprismvol( pa, pb, pc, pd );
		}
		vol_diff = volpos[ 0 ] - volneg[ 0 ] - volneg[ 1 ] - volneg[ 2 ] - volneg[ 3 ];
		fc->tetprism_vol_sum += vol_diff; // Update the total sum.
	}

	// Bond the new tet to adjacent tets.
	for(i = 0; i < 3; i++)
	{
		esym( fliptets[ 0 ], newface ); // At faces [b,a,d], [c,b,d], [a,c,d].
		bond( newface, topcastets[ i ] );
		enextself( fliptets[ 0 ] );
	}
	bond( fliptets[ 0 ], botcastet );

	if(checksubsegflag)
	{
		face checkseg;
		// Bond 6 segments (at edges of [a,b,c,d]) if there there are.
		for(i = 0; i < 3; i++)
		{
			eprev( topcastets[ i ], newface ); // At edges [d,a],[d,b],[d,c].
			if(issubseg( newface ))
			{
				tsspivot1( newface, checkseg );
				esym( fliptets[ 0 ], newface );
				enextself( newface ); // At edges [a,d], [b,d], [c,d].
				tssbond1( newface, checkseg );
				sstbond1( checkseg, newface );
				if(fc->chkencflag & 1)
				{
					enqueuesubface( badsubsegs, &checkseg );
				}
			}
			enextself( fliptets[ 0 ] );
		}
		for(i = 0; i < 3; i++)
		{
			if(issubseg( topcastets[ i ] ))
			{
				tsspivot1( topcastets[ i ], checkseg ); // At edges [a,b],[b,c],[c,a].
				tssbond1( fliptets[ 0 ], checkseg );
				sstbond1( checkseg, fliptets[ 0 ] );
				if(fc->chkencflag & 1)
				{
					enqueuesubface( badsubsegs, &checkseg );
				}
			}
			enextself( fliptets[ 0 ] );
		}
	}

	if(checksubfaceflag)
	{
		face checksh;
		// Bond 4 subfaces (at faces of [a,b,c,d]) if there are.
		for(i = 0; i < 3; i++)
		{
			if(issubface( topcastets[ i ] ))
			{
				tspivot( topcastets[ i ], checksh ); // At faces [a,b,d],[b,c,d],[c,a,d]
				esym( fliptets[ 0 ], newface ); // At faces [b,a,d],[c,b,d],[a,c,d]
				sesymself( checksh );
				tsbond( newface, checksh );
				if(fc->chkencflag & 2)
				{
					enqueuesubface( badsubfacs, &checksh );
				}
			}
			enextself( fliptets[ 0 ] );
		}
		if(issubface( botcastet ))
		{
			tspivot( botcastet, checksh ); // At face [b,a,c]
			sesymself( checksh );
			tsbond( fliptets[ 0 ], checksh );
			if(fc->chkencflag & 2)
			{
				enqueuesubface( badsubfacs, &checksh );
			}
		}

		if(spivot >= 0)
		{
			// Perform a 3-to-1 flip in surface triangulation.
			// Depending on the value of 'spivot', the three subfaces are:
			//   - 0: [a,b,p], [b,d,p], [d,a,p]
			//   - 1: [b,c,p], [c,d,p], [d,b,p] 
			//   - 2: [c,a,p], [a,d,p], [d,c,p] 
			//   - 3: [a,b,p], [b,c,p], [c,a,p]
			// Adjust the three subfaces such that their origins are p, i.e., 
			//   - 3: [p,a,b], [p,b,c], [p,c,a]. (Required by the flip31()).
			for(i = 0; i < 3; i++)
			{
				senext2self( flipshs[ i ] );
			}
			flip31( flipshs, 0 );
			// Delete the three old subfaces.
			for(i = 0; i < 3; i++)
			{
				shellfacedealloc( subfaces, flipshs[ i ].sh );
			}
			if(spivot < 3)
			{
				// // Bond the new subface to the new tet [a,b,c,d].
				tsbond( topcastets[ spivot ], flipshs[ 3 ] );
				fsym( topcastets[ spivot ], newface );
				sesym( flipshs[ 3 ], checksh );
				tsbond( newface, checksh );
			}
			else
			{
				// Bound the new subface [a,b,c] to the new tet [a,b,c,d].
				tsbond( fliptets[ 0 ], flipshs[ 3 ] );
				fsym( fliptets[ 0 ], newface );
				sesym( flipshs[ 3 ], checksh );
				tsbond( newface, checksh );
			}
		} // if (spivot > 0)
	} // if (checksubfaceflag)

	if(fc->chkencflag & 4)
	{
		enqueuetetrahedron( &( fliptets[ 0 ] ) );
	}

	// Update the point-to-tet map.
	setpoint2tet( pa, (tetrahedron)fliptets[ 0 ].tet );
	setpoint2tet( pb, (tetrahedron)fliptets[ 0 ].tet );
	setpoint2tet( pc, (tetrahedron)fliptets[ 0 ].tet );
	setpoint2tet( pd, (tetrahedron)fliptets[ 0 ].tet );

	if(fc->enqflag > 0)
	{
		// Queue faces which may be locally non-Delaunay.
		flippush( flipstack, &( fliptets[ 0 ] ) ); // [a,b,c] (opposite to new point).
		if(fc->enqflag > 1)
		{
			for(i = 0; i < 3; i++)
			{
				esym( fliptets[ 0 ], newface );
				flippush( flipstack, &newface );
				enextself( fliptets[ 0 ] );
			}
		}
	}

	recenttet = fliptets[ 0 ];
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// flipnm()    Flip an edge through a sequence of elementary flips.          //
//                                                                           //
// 'abtets' is an array of 'n' tets in the star of edge [a,b].These tets are //
// ordered in a counterclockwise cycle with respect to the vector a->b, i.e.,//
// use the right-hand rule.                                                  //
//                                                                           //
// 'level' (>= 0) indicates the current link level. If 'level > 0', we are   //
// flipping a link edge of an edge [a',b'],  and 'abedgepivot' indicates     //
// which link edge, i.e., [c',b'] or [a',c'], is [a,b]  These two parameters //
// allow us to determine the new tets after a 3-to-2 flip, i.e., tets that   //
// do not inside the reduced star of edge [a',b'].                           //
//                                                                           //
// If the flag 'fc->unflip' is set, this routine un-does the flips performed //
// in flipnm([a,b]) so that the mesh is returned to its original state       //
// before doing the flipnm([a,b]) operation.                                 //
//                                                                           //
// The return value is an integer nn, where nn <= n.  If nn is 2, then the   //
// edge is flipped.  The first and the second tets in 'abtets' are new tets. //
// Otherwise, nn > 2, the edge is not flipped, and nn is the number of tets  //
// in the current star of [a,b].                                             //
//                                                                           //
// ASSUMPTIONS:                                                              //
//  - Neither a nor b is 'dummypoint'.                                       //
//  - [a,b] must not be a segment.                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::flipnm( triface* abtets, int n, int level, int abedgepivot,
						flipconstraints* fc )
{
	triface fliptets[ 3 ], spintet, flipedge;
	triface *tmpabtets, *parytet;
	point pa, pb, pc, pd, pe, pf;
	REAL ori;
	int hullflag, hulledgeflag;
	int reducflag, rejflag;
	int reflexlinkedgecount;
	int edgepivot;
	int n1, nn;
	int t1ver;
	int i, j;

	pa = org( abtets[ 0 ] );
	pb = dest( abtets[ 0 ] );

	if(n > 3)
	{
		// Try to reduce the size of the Star(ab) by flipping a face in it. 
		reflexlinkedgecount = 0;

		for(i = 0; i < n; i++)
		{
			// Let the face of 'abtets[i]' be [a,b,c].
			if(checksubfaceflag)
			{
				if(issubface( abtets[ i ] ))
				{
					continue; // Skip a subface.
				}
			}
			// Do not flip this face if it is involved in two Stars.
			if(( elemcounter( abtets[ i ] ) > 1 ) ||
				( elemcounter( abtets[ ( i - 1 + n ) % n ] ) > 1 ))
			{
				continue;
			}

			pc = apex( abtets[ i ] );
			pd = apex( abtets[ ( i + 1 ) % n ] );
			pe = apex( abtets[ ( i - 1 + n ) % n ] );
			if(( pd == dummypoint ) || ( pe == dummypoint ))
			{
				continue; // [a,b,c] is a hull face.
			}


			// Decide whether [a,b,c] is flippable or not.
			reducflag = 0;

			hullflag = ( pc == dummypoint ); // pc may be dummypoint.
			hulledgeflag = 0;
			if(hullflag == 0)
			{
				ori = orient3d( pb, pc, pd, pe ); // Is [b,c] locally convex?
				if(ori > 0)
				{
					ori = orient3d( pc, pa, pd, pe ); // Is [c,a] locally convex?
					if(ori > 0)
					{
						// Test if [a,b] is locally convex OR flat.
						ori = orient3d( pa, pb, pd, pe );
						if(ori > 0)
						{
							// Found a 2-to-3 flip: [a,b,c] => [e,d]
							reducflag = 1;
						}
						else if(ori == 0)
						{
							// [a,b] is flat.
							if(n == 4)
							{
								// The "flat" tet can be removed immediately by a 3-to-2 flip.
								reducflag = 1;
								// Check if [e,d] is a hull edge.
								pf = apex( abtets[ ( i + 2 ) % n ] );
								hulledgeflag = ( pf == dummypoint );
							}
						}
					}
				}
				if(!reducflag)
				{
					reflexlinkedgecount++;
				}
			}
			else
			{
				// 'c' is dummypoint.
				if(n == 4)
				{
					// Let the vertex opposite to 'c' is 'f'.
					// A 4-to-4 flip is possible if the two tets [d,e,f,a] and [e,d,f,b]
					//   are valid tets. 
					// Note: When the mesh is not convex, it is possible that [a,b] is
					//   locally non-convex (at hull faces [a,b,e] and [b,a,d]).
					//   In this case, an edge flip [a,b] to [e,d] is still possible.
					pf = apex( abtets[ ( i + 2 ) % n ] );
					ori = orient3d( pd, pe, pf, pa );
					if(ori < 0)
					{
						ori = orient3d( pe, pd, pf, pb );
						if(ori < 0)
						{
							// Found a 4-to-4 flip: [a,b] => [e,d]
							reducflag = 1;
							ori = 0; // Signal as a 4-to-4 flip (like a co-planar case).
							hulledgeflag = 1; // [e,d] is a hull edge.
						}
					}
				}
			} // if (hullflag)

			if(reducflag)
			{
				if(nonconvex && hulledgeflag)
				{
					// We will create a hull edge [e,d]. Make sure it does not exist.
					if(getedge( pe, pd, &spintet ))
					{
						// The 2-to-3 flip is not a topological valid flip. 
						reducflag = 0;
					}
				}
			}

			if(reducflag)
			{
				// [a,b,c] could be removed by a 2-to-3 flip.
				rejflag = 0;
				if(fc->checkflipeligibility)
				{
					// Check if the flip can be performed.
					rejflag = checkflipeligibility( 1, pa, pb, pc, pd, pe, level,
													abedgepivot, fc );
				}
				if(!rejflag)
				{
					// Do flip: [a,b,c] => [e,d].
					fliptets[ 0 ] = abtets[ i ];
					fsym( fliptets[ 0 ], fliptets[ 1 ] ); // abtets[i-1].
					flip23( fliptets, hullflag, fc );

					// Shrink the array 'abtets', maintain the original order.
					//   Two tets 'abtets[i-1] ([a,b,e,c])' and 'abtets[i] ([a,b,c,d])'
					//   are flipped, i.e., they do not in Star(ab) anymore. 
					//   'fliptets[0]' ([e,d,a,b]) is in Star(ab), it is saved in
					//   'abtets[i-1]' (adjust it to be [a,b,e,d]), see below: 
					// 
					//            before                   after
					//     [0] |___________|        [0] |___________| 
					//     ... |___________|        ... |___________|
					//   [i-1] |_[a,b,e,c]_|      [i-1] |_[a,b,e,d]_|
					//     [i] |_[a,b,c,d]_| -->    [i] |_[a,b,d,#]_|
					//   [i+1] |_[a,b,d,#]_|      [i+1] |_[a,b,#,*]_|
					//     ... |___________|        ... |___________|
					//   [n-2] |___________|      [n-2] |___________| 
					//   [n-1] |___________|      [n-1] |_[i]_2-t-3_|
					//
					edestoppoself( fliptets[ 0 ] ); // [a,b,e,d]
					// Increase the counter of this new tet (it is in Star(ab)).
					increaseelemcounter( fliptets[ 0 ] );
					abtets[ ( i - 1 + n ) % n ] = fliptets[ 0 ];
					for(j = i; j < n - 1; j++)
					{
						abtets[ j ] = abtets[ j + 1 ];  // Upshift
					}
					// The last entry 'abtets[n-1]' is empty. It is used in two ways:
					//   (i) it remembers the vertex 'c' (in 'abtets[n-1].tet'), and
					//  (ii) it remembers the position [i] where this flip took place.
					// These information let us to either undo this flip or recover
					//   the original edge link (for collecting new created tets).
					abtets[ n - 1 ].tet = (tetrahedron *)pc;
					abtets[ n - 1 ].ver = 0; // Clear it.
					// 'abtets[n - 1].ver' is in range [0,11] -- only uses 4 bits.
					// Use the 5th bit in 'abtets[n - 1].ver' to signal a 2-to-3 flip.
					abtets[ n - 1 ].ver |= ( 1 << 4 );
					// The poisition [i] of this flip is saved above the 7th bit.
					abtets[ n - 1 ].ver |= ( i << 6 );

					if(fc->collectnewtets)
					{
						// Push the two new tets [e,d,b,c] and [e,d,c,a] into a stack.
						//   Re-use the global array 'cavetetlist'.
						for(j = 1; j < 3; j++)
						{
							cavetetlist->newindex( (void **)&parytet );
							*parytet = fliptets[ j ]; // fliptets[1], fliptets[2].
						}
					}

					// Star(ab) is reduced. Try to flip the edge [a,b].
					nn = flipnm( abtets, n - 1, level, abedgepivot, fc );

					if(nn == 2)
					{
						// The edge has been flipped.
						return nn;
					}
					else
					{ // if (nn > 2)
						// The edge is not flipped.
						if(fc->unflip || ( ori == 0 ))
						{
							// Undo the previous 2-to-3 flip, i.e., do a 3-to-2 flip to 
							//   transform [e,d] => [a,b,c].
							// 'ori == 0' means that the previous flip created a degenerated
							//   tet. It must be removed. 
							// Remember that 'abtets[i-1]' is [a,b,e,d]. We can use it to
							//   find another two tets [e,d,b,c] and [e,d,c,a].
							fliptets[ 0 ] = abtets[ ( i - 1 + ( n - 1 ) ) % ( n - 1 ) ]; // [a,b,e,d]
							edestoppoself( fliptets[ 0 ] ); // [e,d,a,b]
							fnext( fliptets[ 0 ], fliptets[ 1 ] ); // [1] is [e,d,b,c]
							fnext( fliptets[ 1 ], fliptets[ 2 ] ); // [2] is [e,d,c,a]
							// Restore the two original tets in Star(ab). 
							flip32( fliptets, hullflag, fc );
							// Marktest the two restored tets in Star(ab).
							for(j = 0; j < 2; j++)
							{
								increaseelemcounter( fliptets[ j ] );
							}
							// Expand the array 'abtets', maintain the original order.
							for(j = n - 2; j >= i; j--)
							{
								abtets[ j + 1 ] = abtets[ j ];  // Downshift
							}
							// Insert the two new tets 'fliptets[0]' [a,b,c,d] and 
							//  'fliptets[1]' [b,a,c,e] into the (i-1)-th and i-th entries, 
							//  respectively.
							esym( fliptets[ 1 ], abtets[ ( i - 1 + n ) % n ] ); // [a,b,e,c]
							abtets[ i ] = fliptets[ 0 ]; // [a,b,c,d]
							nn++;
							if(fc->collectnewtets)
							{
								// Pop two (flipped) tets from the stack.
								cavetetlist->objects -= 2;
							}
						} // if (unflip || (ori == 0))
					} // if (nn > 2)

					if(!fc->unflip)
					{
						// The flips are not reversed. The current Star(ab) can not be
						//   further reduced. Return its current size (# of tets).
						return nn;
					}
					// unflip is set. 
					// Continue the search for flips.
				}
			} // if (reducflag)
		} // i

		// The Star(ab) is not reduced. 
		if(reflexlinkedgecount > 0)
		{
			// There are reflex edges in the Link(ab).
			if(( ( b->fliplinklevel < 0 ) && ( level < autofliplinklevel ) ) ||
				( ( b->fliplinklevel >= 0 ) && ( level < b->fliplinklevel ) ))
			{
				// Try to reduce the Star(ab) by flipping a reflex edge in Link(ab).
				for(i = 0; i < n; i++)
				{
					// Do not flip this face [a,b,c] if there are two Stars involved.
					if(( elemcounter( abtets[ i ] ) > 1 ) ||
						( elemcounter( abtets[ ( i - 1 + n ) % n ] ) > 1 ))
					{
						continue;
					}
					pc = apex( abtets[ i ] );
					if(pc == dummypoint)
					{
						continue; // [a,b] is a hull edge.
					}
					pd = apex( abtets[ ( i + 1 ) % n ] );
					pe = apex( abtets[ ( i - 1 + n ) % n ] );
					if(( pd == dummypoint ) || ( pe == dummypoint ))
					{
						continue; // [a,b,c] is a hull face.
					}


					edgepivot = 0; // No edge is selected yet.

					// Test if [b,c] is locally convex or flat.
					ori = orient3d( pb, pc, pd, pe );
					if(ori <= 0)
					{
						// Select the edge [c,b].
						enext( abtets[ i ], flipedge ); // [b,c,a,d]
						edgepivot = 1;
					}
					if(!edgepivot)
					{
						// Test if [c,a] is locally convex or flat.
						ori = orient3d( pc, pa, pd, pe );
						if(ori <= 0)
						{
							// Select the edge [a,c].
							eprev( abtets[ i ], flipedge ); // [c,a,b,d].
							edgepivot = 2;
						}
					}

					if(!edgepivot) continue;

					// An edge is selected.
					if(checksubsegflag)
					{
						// Do not flip it if it is a segment.
						if(issubseg( flipedge ))
						{
							if(fc->collectencsegflag)
							{
								face checkseg, *paryseg;
								tsspivot1( flipedge, checkseg );
								if(!sinfected( checkseg ))
								{
									// Queue this segment in list.
									sinfect( checkseg );
									caveencseglist->newindex( (void **)&paryseg );
									*paryseg = checkseg;
								}
							}
							continue;
						}
					}

					// Try to flip the selected edge ([c,b] or [a,c]).
					esymself( flipedge );
					// Count the number of tets at the edge.
					n1 = 0;
					j = 0; // Sum of the star counters.
					spintet = flipedge;
					while(1)
					{
						n1++;
						j += ( elemcounter( spintet ) );
						fnextself( spintet );
						if(spintet.tet == flipedge.tet) break;
					}
					if(n1 < 3)
					{
						// This is only possible when the mesh contains inverted
						//   elements.  Reprot a bug.
						terminatetetgen( this, 2 );
					}
					if(j > 2)
					{
						// The Star(flipedge) overlaps other Stars.
						continue; // Do not flip this edge.
					}

					if(( b->flipstarsize > 0 ) && ( n1 > b->flipstarsize ))
					{
						// The star size exceeds the given limit.
						continue; // Do not flip it.
					}

					// Allocate spaces for Star(flipedge).
					tmpabtets = new triface[ n1 ];
					// Form the Star(flipedge).
					j = 0;
					spintet = flipedge;
					while(1)
					{
						tmpabtets[ j ] = spintet;
						// Increase the star counter of this tet.
						increaseelemcounter( tmpabtets[ j ] );
						j++;
						fnextself( spintet );
						if(spintet.tet == flipedge.tet) break;
					}

					// Try to flip the selected edge away.
					nn = flipnm( tmpabtets, n1, level + 1, edgepivot, fc );

					if(nn == 2)
					{
						// The edge is flipped. Star(ab) is reduced.
						// Shrink the array 'abtets', maintain the original order.
						if(edgepivot == 1)
						{
							// 'tmpabtets[0]' is [d,a,e,b] => contains [a,b].
							spintet = tmpabtets[ 0 ]; // [d,a,e,b]
							enextself( spintet );
							esymself( spintet );
							enextself( spintet ); // [a,b,e,d]
						}
						else
						{
							// 'tmpabtets[1]' is [b,d,e,a] => contains [a,b].
							spintet = tmpabtets[ 1 ]; // [b,d,e,a]
							eprevself( spintet );
							esymself( spintet );
							eprevself( spintet ); // [a,b,e,d]
						} // edgepivot == 2
						increaseelemcounter( spintet ); // It is in Star(ab).
						// Put the new tet at [i-1]-th entry.
						abtets[ ( i - 1 + n ) % n ] = spintet;
						for(j = i; j < n - 1; j++)
						{
							abtets[ j ] = abtets[ j + 1 ];  // Upshift
						}
						// Remember the flips in the last entry of the array 'abtets'.
						// They can be used to recover the flipped edge.
						abtets[ n - 1 ].tet = (tetrahedron *)tmpabtets; // The star(fedge).
						abtets[ n - 1 ].ver = 0; // Clear it.
						// Use the 1st and 2nd bit to save 'edgepivot' (1 or 2).
						abtets[ n - 1 ].ver |= edgepivot;
						// Use the 6th bit to signal this n1-to-m1 flip.
						abtets[ n - 1 ].ver |= ( 1 << 5 );
						// The poisition [i] of this flip is saved from 7th to 19th bit.
						abtets[ n - 1 ].ver |= ( i << 6 );
						// The size of the star 'n1' is saved from 20th bit.
						abtets[ n - 1 ].ver |= ( n1 << 19 );

						// Remember the flipped link vertex 'c'. It can be used to recover
						//   the original edge link of [a,b], and to collect new tets.
						tmpabtets[ 0 ].tet = (tetrahedron *)pc;
						tmpabtets[ 0 ].ver = ( 1 << 5 ); // Flag it as a vertex handle.

						// Continue to flip the edge [a,b].
						nn = flipnm( abtets, n - 1, level, abedgepivot, fc );

						if(nn == 2)
						{
							// The edge has been flipped.
							return nn;
						}
						else
						{ // if (nn > 2) {
							// The edge is not flipped.
							if(fc->unflip)
							{
								// Recover the flipped edge ([c,b] or [a,c]).
								// The sequence of flips are saved in 'tmpabtets'. 
								// abtets[(i-1) % (n-1)] is [a,b,e,d], i.e., the tet created by
								//   the flipping of edge [c,b] or [a,c].It must still exist in
								//   Star(ab). It is the start tet to recover the flipped edge.
								if(edgepivot == 1)
								{
									// The flip edge is [c,b].
									tmpabtets[ 0 ] = abtets[ ( ( i - 1 ) + ( n - 1 ) ) % ( n - 1 ) ]; // [a,b,e,d]
									eprevself( tmpabtets[ 0 ] );
									esymself( tmpabtets[ 0 ] );
									eprevself( tmpabtets[ 0 ] ); // [d,a,e,b]
									fsym( tmpabtets[ 0 ], tmpabtets[ 1 ] ); // [a,d,e,c]
								}
								else
								{
									// The flip edge is [a,c].
									tmpabtets[ 1 ] = abtets[ ( ( i - 1 ) + ( n - 1 ) ) % ( n - 1 ) ]; // [a,b,e,d]
									enextself( tmpabtets[ 1 ] );
									esymself( tmpabtets[ 1 ] );
									enextself( tmpabtets[ 1 ] ); // [b,d,e,a]
									fsym( tmpabtets[ 1 ], tmpabtets[ 0 ] ); // [d,b,e,c]
								} // if (edgepivot == 2)

								// Recover the flipped edge ([c,b] or [a,c]).
								flipnm_post( tmpabtets, n1, 2, edgepivot, fc );

								// Insert the two recovered tets into Star(ab).
								for(j = n - 2; j >= i; j--)
								{
									abtets[ j + 1 ] = abtets[ j ];  // Downshift
								}
								if(edgepivot == 1)
								{
									// tmpabtets[0] is [c,b,d,a] ==> contains [a,b]
									// tmpabtets[1] is [c,b,a,e] ==> contains [a,b]
									// tmpabtets[2] is [c,b,e,d]
									fliptets[ 0 ] = tmpabtets[ 1 ];
									enextself( fliptets[ 0 ] );
									esymself( fliptets[ 0 ] ); // [a,b,e,c]
									fliptets[ 1 ] = tmpabtets[ 0 ];
									esymself( fliptets[ 1 ] );
									eprevself( fliptets[ 1 ] ); // [a,b,c,d]
								}
								else
								{
									// tmpabtets[0] is [a,c,d,b] ==> contains [a,b]
									// tmpabtets[1] is [a,c,b,e] ==> contains [a,b]
									// tmpabtets[2] is [a,c,e,d]
									fliptets[ 0 ] = tmpabtets[ 1 ];
									eprevself( fliptets[ 0 ] );
									esymself( fliptets[ 0 ] ); // [a,b,e,c]
									fliptets[ 1 ] = tmpabtets[ 0 ];
									esymself( fliptets[ 1 ] );
									enextself( fliptets[ 1 ] ); // [a,b,c,d]
								} // edgepivot == 2
								for(j = 0; j < 2; j++)
								{
									increaseelemcounter( fliptets[ j ] );
								}
								// Insert the two recovered tets into Star(ab).
								abtets[ ( i - 1 + n ) % n ] = fliptets[ 0 ];
								abtets[ i ] = fliptets[ 1 ];
								nn++;
								// Release the allocated spaces.
								delete[ ] tmpabtets;
							} // if (unflip)
						} // if (nn > 2)

						if(!fc->unflip)
						{
							// The flips are not reversed. The current Star(ab) can not be
							//   further reduced. Return its size (# of tets).
							return nn;
						}
						// unflip is set. 
						// Continue the search for flips.
					}
					else
					{
						// The selected edge is not flipped.
						if(!fc->unflip)
						{
							// Release the memory used in this attempted flip.
							flipnm_post( tmpabtets, n1, nn, edgepivot, fc );
						}
						// Decrease the star counters of tets in Star(flipedge).
						for(j = 0; j < nn; j++)
						{
							decreaseelemcounter( tmpabtets[ j ] );
						}
						// Release the allocated spaces.
						delete[ ] tmpabtets;
					}
				} // i
			} // if (level...)
		} // if (reflexlinkedgecount > 0)
	}
	else
	{
		// Check if a 3-to-2 flip is possible.
		// Let the three apexes be c, d,and e. Hull tets may be involved. If so, 
		//   we rearrange them such that the vertex e is dummypoint. 
		hullflag = 0;

		if(apex( abtets[ 0 ] ) == dummypoint)
		{
			pc = apex( abtets[ 1 ] );
			pd = apex( abtets[ 2 ] );
			pe = apex( abtets[ 0 ] );
			hullflag = 1;
		}
		else if(apex( abtets[ 1 ] ) == dummypoint)
		{
			pc = apex( abtets[ 2 ] );
			pd = apex( abtets[ 0 ] );
			pe = apex( abtets[ 1 ] );
			hullflag = 2;
		}
		else
		{
			pc = apex( abtets[ 0 ] );
			pd = apex( abtets[ 1 ] );
			pe = apex( abtets[ 2 ] );
			hullflag = ( pe == dummypoint ) ? 3 : 0;
		}

		reducflag = 0;
		rejflag = 0;


		if(hullflag == 0)
		{
			// Make sure that no inverted tet will be created, i.e. the new tets
			//   [d,c,e,a] and [c,d,e,b] must be valid tets. 
			ori = orient3d( pd, pc, pe, pa );
			if(ori < 0)
			{
				ori = orient3d( pc, pd, pe, pb );
				if(ori < 0)
				{
					reducflag = 1;
				}
			}
		}
		else
		{
			// [a,b] is a hull edge.
			//   Note: This can happen when it is in the middle of a 4-to-4 flip.
			//   Note: [a,b] may even be a non-convex hull edge.
			if(!nonconvex)
			{
				//  The mesh is convex, only do flip if it is a coplanar hull edge.
				ori = orient3d( pa, pb, pc, pd );
				if(ori == 0)
				{
					reducflag = 1;
				}
			}
			else
			{ // nonconvex
				reducflag = 1;
			}
			if(reducflag == 1)
			{
				// [a,b], [a,b,c] and [a,b,d] are on the convex hull.
				// Make sure that no inverted tet will be created.
				point searchpt = NULL, chkpt;
				REAL bigvol = 0.0, ori1, ori2;
				// Search an interior vertex which is an apex of edge [c,d].
				//   In principle, it can be arbitrary interior vertex.  To avoid
				//   numerical issue, we choose the vertex which belongs to a tet
				//   't' at edge [c,d] and 't' has the biggest volume.  
				fliptets[ 0 ] = abtets[ hullflag % 3 ]; // [a,b,c,d].
				eorgoppoself( fliptets[ 0 ] );  // [d,c,b,a]
				spintet = fliptets[ 0 ];
				while(1)
				{
					fnextself( spintet );
					chkpt = oppo( spintet );
					if(chkpt == pb) break;
					if(( chkpt != dummypoint ) && ( apex( spintet ) != dummypoint ))
					{
						ori = -orient3d( pd, pc, apex( spintet ), chkpt );
						if(ori > bigvol)
						{
							bigvol = ori;
							searchpt = chkpt;
						}
					}
				}
				if(searchpt != NULL)
				{
					// Now valid the configuration.
					ori1 = orient3d( pd, pc, searchpt, pa );
					ori2 = orient3d( pd, pc, searchpt, pb );
					if(ori1 * ori2 >= 0.0)
					{
						reducflag = 0; // Not valid. 
					}
					else
					{
						ori1 = orient3d( pa, pb, searchpt, pc );
						ori2 = orient3d( pa, pb, searchpt, pd );
						if(ori1 * ori2 >= 0.0)
						{
							reducflag = 0; // Not valid.
						}
					}
				}
				else
				{
					// No valid searchpt is found.
					reducflag = 0; // Do not flip it.
				}
			} // if (reducflag == 1)
		} // if (hullflag == 1)

		if(reducflag)
		{
			// A 3-to-2 flip is possible.
			if(checksubfaceflag)
			{
				// This edge (must not be a segment) can be flipped ONLY IF it belongs
				//   to either 0 or 2 subfaces.  In the latter case, a 2-to-2 flip in 
				//   the surface mesh will be automatically performed within the 
				//   3-to-2 flip.
				nn = 0;
				edgepivot = -1; // Re-use it.
				for(j = 0; j < 3; j++)
				{
					if(issubface( abtets[ j ] ))
					{
						nn++; // Found a subface.
					}
					else
					{
						edgepivot = j;
					}
				}
				if(nn == 1)
				{
					// Found only 1 subface containing this edge. This can happen in 
					//   the boundary recovery phase. The neighbor subface is not yet 
					//   recovered. This edge should not be flipped at this moment.
					rejflag = 1;
				}
				else if(nn == 2)
				{
					// Found two subfaces. A 2-to-2 flip is possible. Validate it.
					// Below we check if the two faces [p,q,a] and [p,q,b] are subfaces.
					eorgoppo( abtets[ ( edgepivot + 1 ) % 3 ], spintet ); // [q,p,b,a]
					if(issubface( spintet ))
					{
						rejflag = 1; // Conflict to a 2-to-2 flip.
					}
					else
					{
						esymself( spintet );
						if(issubface( spintet ))
						{
							rejflag = 1; // Conflict to a 2-to-2 flip.
						}
					}
				}
				else if(nn == 3)
				{
					// Report a bug.
					terminatetetgen( this, 2 );
				}
			}
			if(!rejflag && fc->checkflipeligibility)
			{
				// Here we must exchange 'a' and 'b'. Since in the check... function,
				//   we assume the following point sequence, 'a,b,c,d,e', where
				//   the face [a,b,c] will be flipped and the edge [e,d] will be
				//   created. The two new tets are [a,b,c,d] and [b,a,c,e]. 
				rejflag = checkflipeligibility( 2, pc, pd, pe, pb, pa, level,
												abedgepivot, fc );
			}
			if(!rejflag)
			{
				// Do flip: [a,b] => [c,d,e]
				flip32( abtets, hullflag, fc );
				if(fc->remove_ndelaunay_edge)
				{
					if(level == 0)
					{
						// It is the desired removing edge. Check if we have improved
						//   the objective function.
						if(( fc->tetprism_vol_sum >= 0.0 ) ||
							( fabs( fc->tetprism_vol_sum ) < fc->bak_tetprism_vol ))
						{
							// No improvement! flip back: [c,d,e] => [a,b].
							flip23( abtets, hullflag, fc );
							// Increase the element counter -- They are in cavity.
							for(j = 0; j < 3; j++)
							{
								increaseelemcounter( abtets[ j ] );
							}
							return 3;
						}
					} // if (level == 0)
				}
				if(fc->collectnewtets)
				{
					// Collect new tets.
					if(level == 0)
					{
						// Push the two new tets into stack.
						for(j = 0; j < 2; j++)
						{
							cavetetlist->newindex( (void **)&parytet );
							*parytet = abtets[ j ];
						}
					}
					else
					{
						// Only one of the new tets is collected. The other one is inside
						//   the reduced edge star. 'abedgepivot' is either '1' or '2'.
						cavetetlist->newindex( (void **)&parytet );
						if(abedgepivot == 1)
						{ // [c,b]
							*parytet = abtets[ 1 ];
						}
						else
						{
							*parytet = abtets[ 0 ];
						}
					}
				} // if (fc->collectnewtets)
				return 2;
			}
		} // if (reducflag)
	} // if (n == 3)

	// The current (reduced) Star size.
	return n;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// flipnm_post()    Post process a n-to-m flip.                              //
//                                                                           //
// IMPORTANT: This routine only works when there is no other flip operation  //
// is done after flipnm([a,b]) which attempts to remove an edge [a,b].       //
//                                                                           //
// 'abtets' is an array of 'n' (>= 3) tets which are in the original star of //
// [a,b] before flipnm([a,b]).  'nn' (< n) is the value returned by flipnm.  //
// If 'nn == 2', the edge [a,b] has been flipped. 'abtets[0]' and 'abtets[1]'//
// are [c,d,e,b] and [d,c,e,a], i.e., a 2-to-3 flip can recover the edge [a, //
// b] and its initial Star([a,b]).  If 'nn >= 3' edge [a,b] still exists in  //
// current mesh and 'nn' is the current number of tets in Star([a,b]).       //
//                                                                           //
// Each 'abtets[i]', where nn <= i < n, saves either a 2-to-3 flip or a      //
// flipnm([p1,p2]) operation ([p1,p2] != [a,b]) which created the tet        //
// 'abtets[t-1]', where '0 <= t <= i'.  These information can be used to     //
// undo the flips performed in flipnm([a,b]) or to collect new tets created  //
// by the flipnm([a,b]) operation.                                           //
//                                                                           //
// Default, this routine only walks through the flips and frees the spaces   //
// allocated during the flipnm([a,b]) operation.                             //
//                                                                           //
// If the flag 'fc->unflip' is set, this routine un-does the flips performed //
// in flipnm([a,b]) so that the mesh is returned to its original state       //
// before doing the flipnm([a,b]) operation.                                 //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::flipnm_post( triface* abtets, int n, int nn, int abedgepivot,
							 flipconstraints* fc )
{
	triface fliptets[ 3 ], flipface;
	triface *tmpabtets;
	int fliptype;
	int edgepivot;
	int t, n1;
	int i, j;


	if(nn == 2)
	{
		// The edge [a,b] has been flipped.
		// 'abtets[0]' is [c,d,e,b] or [#,#,#,b].
		// 'abtets[1]' is [d,c,e,a] or [#,#,#,a].
		if(fc->unflip)
		{
			// Do a 2-to-3 flip to recover the edge [a,b]. There may be hull tets.
			flip23( abtets, 1, fc );
			if(fc->collectnewtets)
			{
				// Pop up new (flipped) tets from the stack.
				if(abedgepivot == 0)
				{
					// Two new tets were collected.
					cavetetlist->objects -= 2;
				}
				else
				{
					// Only one of the two new tets was collected.
					cavetetlist->objects -= 1;
				}
			}
		}
		// The initial size of Star(ab) is 3.
		nn++;
	}

	// Walk through the performed flips.
	for(i = nn; i < n; i++)
	{
		// At the beginning of each step 'i', the size of the Star([a,b]) is 'i'.
		// At the end of this step, the size of the Star([a,b]) is 'i+1'.
		// The sizes of the Link([a,b]) are the same.
		fliptype = ( ( abtets[ i ].ver >> 4 ) & 3 ); // 0, 1, or 2.
		if(fliptype == 1)
		{
			// It was a 2-to-3 flip: [a,b,c]->[e,d].
			t = ( abtets[ i ].ver >> 6 );
			if(fc->unflip)
			{
				if(b->verbose > 2)
				{
					myprintf( "      Recover a 2-to-3 flip at f[%d].\n", t );
				}
				// 'abtets[(t-1)%i]' is the tet [a,b,e,d] in current Star(ab), i.e.,
				//   it is created by a 2-to-3 flip [a,b,c] => [e,d].
				fliptets[ 0 ] = abtets[ ( ( t - 1 ) + i ) % i ]; // [a,b,e,d]
				eprevself( fliptets[ 0 ] );
				esymself( fliptets[ 0 ] );
				enextself( fliptets[ 0 ] ); // [e,d,a,b]
				fnext( fliptets[ 0 ], fliptets[ 1 ] ); // [e,d,b,c]
				fnext( fliptets[ 1 ], fliptets[ 2 ] ); // [e,d,c,a]
				// Do a 3-to-2 flip: [e,d] => [a,b,c].
				// NOTE: hull tets may be invloved.
				flip32( fliptets, 1, fc );
				// Expand the array 'abtets', maintain the original order.
				// The new array length is (i+1).
				for(j = i - 1; j >= t; j--)
				{
					abtets[ j + 1 ] = abtets[ j ];  // Downshift
				}
				// The tet abtets[(t-1)%i] is deleted. Insert the two new tets 
				//   'fliptets[0]' [a,b,c,d] and 'fliptets[1]' [b,a,c,e] into
				//   the (t-1)-th and t-th entries, respectively.
				esym( fliptets[ 1 ], abtets[ ( ( t - 1 ) + ( i + 1 ) ) % ( i + 1 ) ] ); // [a,b,e,c]
				abtets[ t ] = fliptets[ 0 ]; // [a,b,c,d]
				if(fc->collectnewtets)
				{
					// Pop up two (flipped) tets from the stack.
					cavetetlist->objects -= 2;
				}
			}
		}
		else if(fliptype == 2)
		{
			tmpabtets = (triface *)( abtets[ i ].tet );
			n1 = ( ( abtets[ i ].ver >> 19 ) & 8191 ); // \sum_{i=0^12}{2^i} = 8191
			edgepivot = ( abtets[ i ].ver & 3 );
			t = ( ( abtets[ i ].ver >> 6 ) & 8191 );
			if(fc->unflip)
			{
				if(b->verbose > 2)
				{
					myprintf( "      Recover a %d-to-m flip at e[%d] of f[%d].\n", n1,
							edgepivot, t );
				}
				// Recover the flipped edge ([c,b] or [a,c]).
				// abtets[(t - 1 + i) % i] is [a,b,e,d], i.e., the tet created by
				//   the flipping of edge [c,b] or [a,c]. It must still exist in
				//   Star(ab). Use it to recover the flipped edge.
				if(edgepivot == 1)
				{
					// The flip edge is [c,b].
					tmpabtets[ 0 ] = abtets[ ( t - 1 + i ) % i ]; // [a,b,e,d]
					eprevself( tmpabtets[ 0 ] );
					esymself( tmpabtets[ 0 ] );
					eprevself( tmpabtets[ 0 ] ); // [d,a,e,b]
					fsym( tmpabtets[ 0 ], tmpabtets[ 1 ] ); // [a,d,e,c]
				}
				else
				{
					// The flip edge is [a,c].
					tmpabtets[ 1 ] = abtets[ ( t - 1 + i ) % i ]; // [a,b,e,d]
					enextself( tmpabtets[ 1 ] );
					esymself( tmpabtets[ 1 ] );
					enextself( tmpabtets[ 1 ] ); // [b,d,e,a]
					fsym( tmpabtets[ 1 ], tmpabtets[ 0 ] ); // [d,b,e,c]
				} // if (edgepivot == 2)

				// Do a n1-to-m1 flip to recover the flipped edge ([c,b] or [a,c]).
				flipnm_post( tmpabtets, n1, 2, edgepivot, fc );

				// Insert the two recovered tets into the original Star(ab).
				for(j = i - 1; j >= t; j--)
				{
					abtets[ j + 1 ] = abtets[ j ];  // Downshift
				}
				if(edgepivot == 1)
				{
					// tmpabtets[0] is [c,b,d,a] ==> contains [a,b]
					// tmpabtets[1] is [c,b,a,e] ==> contains [a,b]
					// tmpabtets[2] is [c,b,e,d]
					fliptets[ 0 ] = tmpabtets[ 1 ];
					enextself( fliptets[ 0 ] );
					esymself( fliptets[ 0 ] ); // [a,b,e,c]
					fliptets[ 1 ] = tmpabtets[ 0 ];
					esymself( fliptets[ 1 ] );
					eprevself( fliptets[ 1 ] ); // [a,b,c,d]
				}
				else
				{
					// tmpabtets[0] is [a,c,d,b] ==> contains [a,b]
					// tmpabtets[1] is [a,c,b,e] ==> contains [a,b]
					// tmpabtets[2] is [a,c,e,d]
					fliptets[ 0 ] = tmpabtets[ 1 ];
					eprevself( fliptets[ 0 ] );
					esymself( fliptets[ 0 ] ); // [a,b,e,c]
					fliptets[ 1 ] = tmpabtets[ 0 ];
					esymself( fliptets[ 1 ] );
					enextself( fliptets[ 1 ] ); // [a,b,c,d]
				} // edgepivot == 2
				// Insert the two recovered tets into Star(ab).
				abtets[ ( ( t - 1 ) + ( i + 1 ) ) % ( i + 1 ) ] = fliptets[ 0 ];
				abtets[ t ] = fliptets[ 1 ];
			}
			else
			{
				// Only free the spaces.
				flipnm_post( tmpabtets, n1, 2, edgepivot, fc );
			} // if (!unflip)
			if(b->verbose > 2)
			{
				myprintf( "      Release %d spaces at f[%d].\n", n1, i );
			}
			delete[ ] tmpabtets;
		}
	} // i

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// insertpoint()    Insert a point into current tetrahedralization.          //
//                                                                           //
// The Bowyer-Watson (B-W) algorithm is used to add a new point p into the   //
// tetrahedralization T. It first finds a "cavity", denoted as C, in T,  C   //
// consists of tetrahedra in T that "conflict" with p.  If T is a Delaunay   //
// tetrahedralization, then all boundary faces (triangles) of C are visible  //
// by p, i.e.,C is star-shaped. We can insert p into T by first deleting all //
// tetrahedra in C, then creating new tetrahedra formed by boundary faces of //
// C and p.  If T is not a DT, then C may be not star-shaped.  It must be    //
// modified so that it becomes star-shaped.                                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::insertpoint( point insertpt, triface *searchtet, face *splitsh,
							 face *splitseg, insertvertexflags *ivf )
{
	arraypool *swaplist;
	triface *cavetet, spintet, neightet, neineitet, *parytet;
	triface oldtet, newtet, newneitet;
	face checksh, neighsh, *parysh;
	face checkseg, *paryseg;
	point *pts, pa, pb, pc, *parypt;
	enum locateresult loc = OUTSIDE;
	REAL sign, ori;
	REAL attrib, volume;
	bool enqflag;
	int t1ver;
	int i, j, k, s;

	if(b->verbose > 2)
	{
		myprintf( "      Insert point %d\n", pointmark( insertpt ) );
	}

	// Locate the point.
	if(searchtet->tet != NULL)
	{
		loc = ( enum locateresult ) ivf->iloc;
	}

	if(loc == OUTSIDE)
	{
		if(searchtet->tet == NULL)
		{
			if(!b->weighted)
			{
				randomsample( insertpt, searchtet );
			}
			else
			{
				// Weighted DT. There may exist dangling vertex. 
				*searchtet = recenttet;
			}
		}
		// Locate the point.
		loc = locate( insertpt, searchtet );
	}

	ivf->iloc = (int)loc; // The return value.

	if(b->weighted)
	{
		if(loc != OUTSIDE)
		{
			// Check if this vertex is regular.
			pts = (point *)searchtet->tet;
			sign = orient4d_s( pts[ 4 ], pts[ 5 ], pts[ 6 ], pts[ 7 ], insertpt,
							   pts[ 4 ][ 3 ], pts[ 5 ][ 3 ], pts[ 6 ][ 3 ], pts[ 7 ][ 3 ],
							   insertpt[ 3 ] );
			if(sign > 0)
			{
				// This new vertex lies above the lower hull. Do not insert it.
				ivf->iloc = (int)NONREGULAR;
				return 0;
			}
		}
	}

	// Create the initial cavity C(p) which contains all tetrahedra that
	//   intersect p. It may include 1, 2, or n tetrahedra.
	// If p lies on a segment or subface, also create the initial sub-cavity
	//   sC(p) which contains all subfaces (and segment) which intersect p.

	if(loc == OUTSIDE)
	{
		flip14count++;
		// The current hull will be enlarged.
		// Add four adjacent boundary tets into list.
		for(i = 0; i < 4; i++)
		{
			decode( searchtet->tet[ i ], neightet );
			neightet.ver = epivot[ neightet.ver ];
			cavebdrylist->newindex( (void **)&parytet );
			*parytet = neightet;
		}
		infect( *searchtet );
		caveoldtetlist->newindex( (void **)&parytet );
		*parytet = *searchtet;
	}
	else if(loc == INTETRAHEDRON)
	{
		flip14count++;
		// Add four adjacent boundary tets into list.
		for(i = 0; i < 4; i++)
		{
			decode( searchtet->tet[ i ], neightet );
			neightet.ver = epivot[ neightet.ver ];
			cavebdrylist->newindex( (void **)&parytet );
			*parytet = neightet;
		}
		infect( *searchtet );
		caveoldtetlist->newindex( (void **)&parytet );
		*parytet = *searchtet;
	}
	else if(loc == ONFACE)
	{
		flip26count++;
		// Add six adjacent boundary tets into list.
		j = ( searchtet->ver & 3 ); // The current face number.
		for(i = 1; i < 4; i++)
		{
			decode( searchtet->tet[ ( j + i ) % 4 ], neightet );
			neightet.ver = epivot[ neightet.ver ];
			cavebdrylist->newindex( (void **)&parytet );
			*parytet = neightet;
		}
		decode( searchtet->tet[ j ], spintet );
		j = ( spintet.ver & 3 ); // The current face number.
		for(i = 1; i < 4; i++)
		{
			decode( spintet.tet[ ( j + i ) % 4 ], neightet );
			neightet.ver = epivot[ neightet.ver ];
			cavebdrylist->newindex( (void **)&parytet );
			*parytet = neightet;
		}
		infect( spintet );
		caveoldtetlist->newindex( (void **)&parytet );
		*parytet = spintet;
		infect( *searchtet );
		caveoldtetlist->newindex( (void **)&parytet );
		*parytet = *searchtet;

		if(ivf->splitbdflag)
		{
			if(( splitsh != NULL ) && ( splitsh->sh != NULL ))
			{
				// Create the initial sub-cavity sC(p).
				smarktest( *splitsh );
				caveshlist->newindex( (void **)&parysh );
				*parysh = *splitsh;
			}
		} // if (splitbdflag)
	}
	else if(loc == ONEDGE)
	{
		flipn2ncount++;
		// Add all adjacent boundary tets into list.
		spintet = *searchtet;
		while(1)
		{
			eorgoppo( spintet, neightet );
			decode( neightet.tet[ neightet.ver & 3 ], neightet );
			neightet.ver = epivot[ neightet.ver ];
			cavebdrylist->newindex( (void **)&parytet );
			*parytet = neightet;
			edestoppo( spintet, neightet );
			decode( neightet.tet[ neightet.ver & 3 ], neightet );
			neightet.ver = epivot[ neightet.ver ];
			cavebdrylist->newindex( (void **)&parytet );
			*parytet = neightet;
			infect( spintet );
			caveoldtetlist->newindex( (void **)&parytet );
			*parytet = spintet;
			fnextself( spintet );
			if(spintet.tet == searchtet->tet) break;
		} // while (1)

		if(ivf->splitbdflag)
		{
			// Create the initial sub-cavity sC(p).
			if(( splitseg != NULL ) && ( splitseg->sh != NULL ))
			{
				smarktest( *splitseg );
				splitseg->shver = 0;
				spivot( *splitseg, *splitsh );
			}
			if(splitsh != NULL)
			{
				if(splitsh->sh != NULL)
				{
					// Collect all subfaces share at this edge.
					pa = sorg( *splitsh );
					neighsh = *splitsh;
					while(1)
					{
						// Adjust the origin of its edge to be 'pa'.
						if(sorg( neighsh ) != pa)
						{
							sesymself( neighsh );
						}
						// Add this face into list (in B-W cavity).
						smarktest( neighsh );
						caveshlist->newindex( (void **)&parysh );
						*parysh = neighsh;
						// Add this face into face-at-splitedge list.
						cavesegshlist->newindex( (void **)&parysh );
						*parysh = neighsh;
						// Go to the next face at the edge.
						spivotself( neighsh );
						// Stop if all faces at the edge have been visited.
						if(neighsh.sh == splitsh->sh) break;
						if(neighsh.sh == NULL) break;
					} // while (1)
				} // if (not a dangling segment)
			}
		} // if (splitbdflag)
	}
	else if(loc == INSTAR)
	{
		// We assume that all tets in the star are given in 'caveoldtetlist',
		//   and they are all infected.
		// Collect the boundary faces of the star.
		for(i = 0; i < caveoldtetlist->objects; i++)
		{
			cavetet = (triface *)fastlookup( caveoldtetlist, i );
			// Check its 4 neighbor tets.
			for(j = 0; j < 4; j++)
			{
				decode( cavetet->tet[ j ], neightet );
				if(!infected( neightet ))
				{
					// It's a boundary face.
					neightet.ver = epivot[ neightet.ver ];
					cavebdrylist->newindex( (void **)&parytet );
					*parytet = neightet;
				}
			}
		}
	}
	else if(loc == ONVERTEX)
	{
		// The point already exist. Do nothing and return.
		return 0;
	}


	if(ivf->assignmeshsize)
	{
		// Assign mesh size for the new point.
		if(bgm != NULL)
		{
			// Interpolate the mesh size from the background mesh. 
			bgm->decode( point2bgmtet( org( *searchtet ) ), neightet );
			int bgmloc = (int)bgm->scoutpoint( insertpt, &neightet, 0 );
			if(bgmloc != (int)OUTSIDE)
			{
				insertpt[ pointmtrindex ] =
					bgm->getpointmeshsize( insertpt, &neightet, bgmloc );
				setpoint2bgmtet( insertpt, bgm->encode( neightet ) );
			}
		}
		else
		{
			insertpt[ pointmtrindex ] = getpointmeshsize( insertpt, searchtet, (int)loc );
		}
	} // if (assignmeshsize)

	if(ivf->bowywat)
	{
		// Update the cavity C(p) using the Bowyer-Watson algorithm.
		swaplist = cavetetlist;
		cavetetlist = cavebdrylist;
		cavebdrylist = swaplist;
		for(i = 0; i < cavetetlist->objects; i++)
		{
			// 'cavetet' is an adjacent tet at outside of the cavity.
			cavetet = (triface *)fastlookup( cavetetlist, i );
			// The tet may be tested and included in the (enlarged) cavity.
			if(!infected( *cavetet ))
			{
				// Check for two possible cases for this tet: 
				//   (1) It is a cavity tet, or
				//   (2) it is a cavity boundary face.
				enqflag = false;
				if(!marktested( *cavetet ))
				{
					// Do Delaunay (in-sphere) test.
					pts = (point *)cavetet->tet;
					if(pts[ 7 ] != dummypoint)
					{
						// A volume tet. Operate on it.
						if(b->weighted)
						{
							sign = orient4d_s( pts[ 4 ], pts[ 5 ], pts[ 6 ], pts[ 7 ], insertpt,
											   pts[ 4 ][ 3 ], pts[ 5 ][ 3 ], pts[ 6 ][ 3 ], pts[ 7 ][ 3 ],
											   insertpt[ 3 ] );
						}
						else
						{
							sign = insphere_s( pts[ 4 ], pts[ 5 ], pts[ 6 ], pts[ 7 ], insertpt );
						}
						enqflag = ( sign < 0.0 );
					}
					else
					{
						if(!nonconvex)
						{
							// Test if this hull face is visible by the new point. 
							ori = orient3d( pts[ 4 ], pts[ 5 ], pts[ 6 ], insertpt );
							if(ori < 0)
							{
								// A visible hull face. 
								// Include it in the cavity. The convex hull will be enlarged.
								enqflag = true;
							}
							else if(ori == 0.0)
							{
								// A coplanar hull face. We need to test if this hull face is
								//   Delaunay or not. We test if the adjacent tet (not faked)
								//   of this hull face is Delaunay or not.
								decode( cavetet->tet[ 3 ], neineitet );
								if(!infected( neineitet ))
								{
									if(!marktested( neineitet ))
									{
										// Do Delaunay test on this tet.
										pts = (point *)neineitet.tet;
										if(b->weighted)
										{
											sign = orient4d_s( pts[ 4 ], pts[ 5 ], pts[ 6 ], pts[ 7 ], insertpt,
															   pts[ 4 ][ 3 ], pts[ 5 ][ 3 ], pts[ 6 ][ 3 ],
															   pts[ 7 ][ 3 ], insertpt[ 3 ] );
										}
										else
										{
											sign = insphere_s( pts[ 4 ], pts[ 5 ], pts[ 6 ], pts[ 7 ], insertpt );
										}
										enqflag = ( sign < 0.0 );
									}
								}
								else
								{
									// The adjacent tet is non-Delaunay. The hull face is non-
									//   Delaunay as well. Include it in the cavity.
									enqflag = true;
								} // if (!infected(neineitet))
							} // if (ori == 0.0)
						}
						else
						{
							// A hull face (must be a subface).
							// We FIRST include it in the initial cavity if the adjacent tet
							//   (not faked) of this hull face is not Delaunay wrt p.
							//   Whether it belongs to the final cavity will be determined
							//   during the validation process. 'validflag'.
							decode( cavetet->tet[ 3 ], neineitet );
							if(!infected( neineitet ))
							{
								if(!marktested( neineitet ))
								{
									// Do Delaunay test on this tet.
									pts = (point *)neineitet.tet;
									if(b->weighted)
									{
										sign = orient4d_s( pts[ 4 ], pts[ 5 ], pts[ 6 ], pts[ 7 ], insertpt,
														   pts[ 4 ][ 3 ], pts[ 5 ][ 3 ], pts[ 6 ][ 3 ],
														   pts[ 7 ][ 3 ], insertpt[ 3 ] );
									}
									else
									{
										sign = insphere_s( pts[ 4 ], pts[ 5 ], pts[ 6 ], pts[ 7 ], insertpt );
									}
									enqflag = ( sign < 0.0 );
								}
							}
							else
							{
								// The adjacent tet is non-Delaunay. The hull face is non-
								//   Delaunay as well. Include it in the cavity.
								enqflag = true;
							} // if (infected(neineitet))
						} // if (nonconvex)
					} // if (pts[7] != dummypoint)
					marktest( *cavetet ); // Only test it once.
				} // if (!marktested(*cavetet))

				if(enqflag)
				{
					// Found a tet in the cavity. Put other three faces in check list.
					k = ( cavetet->ver & 3 ); // The current face number
					for(j = 1; j < 4; j++)
					{
						decode( cavetet->tet[ ( j + k ) % 4 ], neightet );
						cavetetlist->newindex( (void **)&parytet );
						*parytet = neightet;
					}
					infect( *cavetet );
					caveoldtetlist->newindex( (void **)&parytet );
					*parytet = *cavetet;
				}
				else
				{
					// Found a boundary face of the cavity. 
					cavetet->ver = epivot[ cavetet->ver ];
					cavebdrylist->newindex( (void **)&parytet );
					*parytet = *cavetet;
				}
			} // if (!infected(*cavetet))
		} // i

		cavetetlist->restart( ); // Clear the working list.
	} // if (ivf->bowywat)

	if(checksubsegflag)
	{
		// Collect all segments of C(p).
		shellface *ssptr;
		for(i = 0; i < caveoldtetlist->objects; i++)
		{
			cavetet = (triface *)fastlookup( caveoldtetlist, i );
			if(( ssptr = (shellface*)cavetet->tet[ 8 ] ) != NULL)
			{
				for(j = 0; j < 6; j++)
				{
					if(ssptr[ j ])
					{
						sdecode( ssptr[ j ], checkseg );
						if(!sinfected( checkseg ))
						{
							sinfect( checkseg );
							cavetetseglist->newindex( (void **)&paryseg );
							*paryseg = checkseg;
						}
					}
				} // j
			}
		} // i
		// Uninfect collected segments.
		for(i = 0; i < cavetetseglist->objects; i++)
		{
			paryseg = (face *)fastlookup( cavetetseglist, i );
			suninfect( *paryseg );
		}

		if(ivf->rejflag & 1)
		{
			// Reject this point if it encroaches upon any segment.
			face *paryseg1;
			for(i = 0; i < cavetetseglist->objects; i++)
			{
				paryseg1 = (face *)fastlookup( cavetetseglist, i );
				if(checkseg4encroach( (point)paryseg1->sh[ 3 ], (point)paryseg1->sh[ 4 ],
					insertpt ))
				{
					encseglist->newindex( (void **)&paryseg );
					*paryseg = *paryseg1;
				}
			} // i
			if(( ivf->rejflag & 1 ) && ( encseglist->objects > 0 ))
			{
				insertpoint_abort( splitseg, ivf );
				ivf->iloc = (int)ENCSEGMENT;
				return 0;
			}
		}
	} // if (checksubsegflag)

	if(checksubfaceflag)
	{
		// Collect all subfaces of C(p).
		shellface *sptr;
		for(i = 0; i < caveoldtetlist->objects; i++)
		{
			cavetet = (triface *)fastlookup( caveoldtetlist, i );
			if(( sptr = (shellface*)cavetet->tet[ 9 ] ) != NULL)
			{
				for(j = 0; j < 4; j++)
				{
					if(sptr[ j ])
					{
						sdecode( sptr[ j ], checksh );
						if(!sinfected( checksh ))
						{
							sinfect( checksh );
							cavetetshlist->newindex( (void **)&parysh );
							*parysh = checksh;
						}
					}
				} // j
			}
		} // i
		// Uninfect collected subfaces.
		for(i = 0; i < cavetetshlist->objects; i++)
		{
			parysh = (face *)fastlookup( cavetetshlist, i );
			suninfect( *parysh );
		}

		if(ivf->rejflag & 2)
		{
			REAL rd, cent[ 3 ];
			badface *bface;
			// Reject this point if it encroaches upon any subface.
			for(i = 0; i < cavetetshlist->objects; i++)
			{
				parysh = (face *)fastlookup( cavetetshlist, i );
				if(checkfac4encroach( (point)parysh->sh[ 3 ], (point)parysh->sh[ 4 ],
					(point)parysh->sh[ 5 ], insertpt, cent, &rd ))
				{
					encshlist->newindex( (void **)&bface );
					bface->ss = *parysh;
					bface->forg = (point)parysh->sh[ 3 ]; // Not a dad one.
					for(j = 0; j < 3; j++) bface->cent[ j ] = cent[ j ];
					bface->key = rd;
				}
			}
			if(encshlist->objects > 0)
			{
				insertpoint_abort( splitseg, ivf );
				ivf->iloc = (int)ENCSUBFACE;
				return 0;
			}
		}
	} // if (checksubfaceflag)

	if(( ivf->iloc == (int)OUTSIDE ) && ivf->refineflag)
	{
		// The vertex lies outside of the domain. And it does not encroach
		//   upon any boundary segment or subface. Do not insert it.
		insertpoint_abort( splitseg, ivf );
		return 0;
	}

	if(ivf->splitbdflag)
	{
		// The new point locates in surface mesh. Update the sC(p). 
		// We have already 'smarktested' the subfaces which directly intersect
		//   with p in 'caveshlist'. From them, we 'smarktest' their neighboring
		//   subfaces which are included in C(p). Do not across a segment.
		for(i = 0; i < caveshlist->objects; i++)
		{
			parysh = (face *)fastlookup( caveshlist, i );
			checksh = *parysh;
			for(j = 0; j < 3; j++)
			{
				if(!isshsubseg( checksh ))
				{
					spivot( checksh, neighsh );
					if(!smarktested( neighsh ))
					{
						stpivot( neighsh, neightet );
						if(infected( neightet ))
						{
							fsymself( neightet );
							if(infected( neightet ))
							{
								// This subface is inside C(p). 
								// Check if its diametrical circumsphere encloses 'p'.
								//   The purpose of this check is to avoid forming invalid
								//   subcavity in surface mesh.
								sign = incircle3d( sorg( neighsh ), sdest( neighsh ),
												   sapex( neighsh ), insertpt );
								if(sign < 0)
								{
									smarktest( neighsh );
									caveshlist->newindex( (void **)&parysh );
									*parysh = neighsh;
								}
							}
						}
					}
				}
				senextself( checksh );
			} // j
		} // i
	} // if (ivf->splitbdflag)

	if(ivf->validflag)
	{
		// Validate C(p) and update it if it is not star-shaped.
		int cutcount = 0;

		if(ivf->respectbdflag)
		{
			// The initial cavity may include subfaces which are not on the facets
			//   being splitting. Find them and make them as boundary of C(p). 
			// Comment: We have already 'smarktested' the subfaces in sC(p). They
			//   are completely inside C(p). 
			for(i = 0; i < cavetetshlist->objects; i++)
			{
				parysh = (face *)fastlookup( cavetetshlist, i );
				stpivot( *parysh, neightet );
				if(infected( neightet ))
				{
					fsymself( neightet );
					if(infected( neightet ))
					{
						// Found a subface inside C(p).
						if(!smarktested( *parysh ))
						{
							// It is possible that this face is a boundary subface.
							// Check if it is a hull face.
							//assert(apex(neightet) != dummypoint);
							if(oppo( neightet ) != dummypoint)
							{
								fsymself( neightet );
							}
							if(oppo( neightet ) != dummypoint)
							{
								ori = orient3d( org( neightet ), dest( neightet ), apex( neightet ),
												insertpt );
								if(ori < 0)
								{
									// A visible face, get its neighbor face.
									fsymself( neightet );
									ori = -ori; // It must be invisible by p.
								}
							}
							else
							{
								// A hull tet. It needs to be cut.
								ori = 1;
							}
							// Cut this tet if it is either invisible by or coplanar with p.
							if(ori >= 0)
							{
								uninfect( neightet );
								unmarktest( neightet );
								cutcount++;
								neightet.ver = epivot[ neightet.ver ];
								cavebdrylist->newindex( (void **)&parytet );
								*parytet = neightet;
								// Add three new faces to find new boundaries.
								for(j = 0; j < 3; j++)
								{
									esym( neightet, neineitet );
									neineitet.ver = epivot[ neineitet.ver ];
									cavebdrylist->newindex( (void **)&parytet );
									*parytet = neineitet;
									enextself( neightet );
								}
							} // if (ori >= 0) 
						}
					}
				}
			} // i

			// The initial cavity may include segments in its interior. We need to
			//   Update the cavity so that these segments are on the boundary of
			//   the cavity.
			for(i = 0; i < cavetetseglist->objects; i++)
			{
				paryseg = (face *)fastlookup( cavetetseglist, i );
				// Check this segment if it is not a splitting segment.
				if(!smarktested( *paryseg ))
				{
					sstpivot1( *paryseg, neightet );
					spintet = neightet;
					while(1)
					{
						if(!infected( spintet )) break;
						fnextself( spintet );
						if(spintet.tet == neightet.tet) break;
					}
					if(infected( spintet ))
					{
						// Find an adjacent tet at this segment such that both faces
						//   at this segment are not visible by p.
						pa = org( neightet );
						pb = dest( neightet );
						spintet = neightet;
						j = 0;
						while(1)
						{
							// Check if this face is visible by p.
							pc = apex( spintet );
							if(pc != dummypoint)
							{
								ori = orient3d( pa, pb, pc, insertpt );
								if(ori >= 0)
								{
									// Not visible. Check another face in this tet.
									esym( spintet, neineitet );
									pc = apex( neineitet );
									if(pc != dummypoint)
									{
										ori = orient3d( pb, pa, pc, insertpt );
										if(ori >= 0)
										{
											// Not visible. Found this face.
											j = 1; // Flag that it is found.
											break;
										}
									}
								}
							}
							fnextself( spintet );
							if(spintet.tet == neightet.tet) break;
						}
						if(j == 0)
						{
							// Not found such a face.
							terminatetetgen( this, 2 );
						}
						neightet = spintet;
						if(b->verbose > 3)
						{
							myprintf( "        Cut tet (%d, %d, %d, %d)\n",
									pointmark( org( neightet ) ), pointmark( dest( neightet ) ),
									pointmark( apex( neightet ) ), pointmark( oppo( neightet ) ) );
						}
						uninfect( neightet );
						unmarktest( neightet );
						cutcount++;
						neightet.ver = epivot[ neightet.ver ];
						cavebdrylist->newindex( (void **)&parytet );
						*parytet = neightet;
						// Add three new faces to find new boundaries.
						for(j = 0; j < 3; j++)
						{
							esym( neightet, neineitet );
							neineitet.ver = epivot[ neineitet.ver ];
							cavebdrylist->newindex( (void **)&parytet );
							*parytet = neineitet;
							enextself( neightet );
						}
					}
				}
			} // i
		} // if (ivf->respectbdflag)

		// Update the cavity by removing invisible faces until it is star-shaped.
		for(i = 0; i < cavebdrylist->objects; i++)
		{
			cavetet = (triface *)fastlookup( cavebdrylist, i );
			// 'cavetet' is an exterior tet adjacent to the cavity.
			// Check if its neighbor is inside C(p).
			fsym( *cavetet, neightet );
			if(infected( neightet ))
			{
				if(apex( *cavetet ) != dummypoint)
				{
					// It is a cavity boundary face. Check its visibility.
					if(oppo( neightet ) != dummypoint)
					{
						// Check if this face is visible by the new point.
						if(issubface( neightet ))
						{
							// We should only create a new tet that has a reasonable volume.
							// Re-use 'volume' and 'attrib'.
							pa = org( *cavetet );
							pb = dest( *cavetet );
							pc = apex( *cavetet );
							volume = orient3dfast( pa, pb, pc, insertpt );
							attrib = distance( pa, pb ) * distance( pb, pc ) * distance( pc, pa );
							if(( fabs( volume ) / attrib ) < b->epsilon)
							{
								ori = 0.0;
							}
							else
							{
								ori = orient3d( pa, pb, pc, insertpt );
							}
						}
						else
						{
							ori = orient3d( org( *cavetet ), dest( *cavetet ), apex( *cavetet ),
											insertpt );
						}
						enqflag = ( ori > 0 );
						// Comment: if ori == 0 (coplanar case), we also cut the tet.
					}
					else
					{
						// It is a hull face. And its adjacent tet (at inside of the 
						//   domain) has been cut from the cavity. Cut it as well.
						//assert(nonconvex);
						enqflag = false;
					}
				}
				else
				{
					enqflag = true; // A hull edge.
				}
				if(enqflag)
				{
					// This face is valid, save it.
					cavetetlist->newindex( (void **)&parytet );
					*parytet = *cavetet;
				}
				else
				{
					uninfect( neightet );
					unmarktest( neightet );
					cutcount++;
					// Add three new faces to find new boundaries.
					for(j = 0; j < 3; j++)
					{
						esym( neightet, neineitet );
						neineitet.ver = epivot[ neineitet.ver ];
						cavebdrylist->newindex( (void **)&parytet );
						*parytet = neineitet;
						enextself( neightet );
					}
					// 'cavetet' is not on the cavity boundary anymore.
					unmarktest( *cavetet );
				}
			}
			else
			{
				// 'cavetet' is not on the cavity boundary anymore.
				unmarktest( *cavetet );
			}
		} // i

		if(cutcount > 0)
		{
			// The cavity has been updated.
			// Update the cavity boundary faces.
			cavebdrylist->restart( );
			for(i = 0; i < cavetetlist->objects; i++)
			{
				cavetet = (triface *)fastlookup( cavetetlist, i );
				// 'cavetet' was an exterior tet adjacent to the cavity.
				fsym( *cavetet, neightet );
				if(infected( neightet ))
				{
					// It is a cavity boundary face.
					cavebdrylist->newindex( (void **)&parytet );
					*parytet = *cavetet;
				}
				else
				{
					// Not a cavity boundary face.
					unmarktest( *cavetet );
				}
			}

			// Update the list of old tets.
			cavetetlist->restart( );
			for(i = 0; i < caveoldtetlist->objects; i++)
			{
				cavetet = (triface *)fastlookup( caveoldtetlist, i );
				if(infected( *cavetet ))
				{
					cavetetlist->newindex( (void **)&parytet );
					*parytet = *cavetet;
				}
			}
			// Swap 'cavetetlist' and 'caveoldtetlist'.
			swaplist = caveoldtetlist;
			caveoldtetlist = cavetetlist;
			cavetetlist = swaplist;

			// The cavity should contain at least one tet.
			if(caveoldtetlist->objects == 0l)
			{
				insertpoint_abort( splitseg, ivf );
				ivf->iloc = (int)BADELEMENT;
				return 0;
			}

			if(ivf->splitbdflag)
			{
				int cutshcount = 0;
				// Update the sub-cavity sC(p).
				for(i = 0; i < caveshlist->objects; i++)
				{
					parysh = (face *)fastlookup( caveshlist, i );
					if(smarktested( *parysh ))
					{
						enqflag = false;
						stpivot( *parysh, neightet );
						if(infected( neightet ))
						{
							fsymself( neightet );
							if(infected( neightet ))
							{
								enqflag = true;
							}
						}
						if(!enqflag)
						{
							sunmarktest( *parysh );
							// Use the last entry of this array to fill this entry.
							j = caveshlist->objects - 1;
							checksh = *(face *)fastlookup( caveshlist, j );
							*parysh = checksh;
							cutshcount++;
							caveshlist->objects--; // The list is shrinked.
							i--;
						}
					}
				}

				if(cutshcount > 0)
				{
					i = 0; // Count the number of invalid subfaces/segments.
					// Valid the updated sub-cavity sC(p).
					if(loc == ONFACE)
					{
						if(( splitsh != NULL ) && ( splitsh->sh != NULL ))
						{
							// The to-be split subface should be in sC(p).
							if(!smarktested( *splitsh )) i++;
						}
					}
					else if(loc == ONEDGE)
					{
						if(( splitseg != NULL ) && ( splitseg->sh != NULL ))
						{
							// The to-be split segment should be in sC(p).
							if(!smarktested( *splitseg )) i++;
						}
						if(( splitsh != NULL ) && ( splitsh->sh != NULL ))
						{
							// All subfaces at this edge should be in sC(p).
							pa = sorg( *splitsh );
							neighsh = *splitsh;
							while(1)
							{
								// Adjust the origin of its edge to be 'pa'.
								if(sorg( neighsh ) != pa)
								{
									sesymself( neighsh );
								}
								// Add this face into list (in B-W cavity).
								if(!smarktested( neighsh )) i++;
								// Go to the next face at the edge.
								spivotself( neighsh );
								// Stop if all faces at the edge have been visited.
								if(neighsh.sh == splitsh->sh) break;
								if(neighsh.sh == NULL) break;
							} // while (1)
						}
					}

					if(i > 0)
					{
						// The updated sC(p) is invalid. Do not insert this vertex.
						insertpoint_abort( splitseg, ivf );
						ivf->iloc = (int)BADELEMENT;
						return 0;
					}
				} // if (cutshcount > 0)
			} // if (ivf->splitbdflag)
		} // if (cutcount > 0)

	} // if (ivf->validflag)

	if(ivf->refineflag)
	{
		// The new point is inserted by Delaunay refinement, i.e., it is the 
		//   circumcenter of a tetrahedron, or a subface, or a segment.
		//   Do not insert this point if the tetrahedron, or subface, or segment
		//   is not inside the final cavity.
		if(( ( ivf->refineflag == 1 ) && !infected( ivf->refinetet ) ) ||
			( ( ivf->refineflag == 2 ) && !smarktested( ivf->refinesh ) ))
		{
			insertpoint_abort( splitseg, ivf );
			ivf->iloc = (int)BADELEMENT;
			return 0;
		}
	} // if (ivf->refineflag)

	if(b->plc && ( loc != INSTAR ))
	{
		// Reject the new point if it lies too close to an existing point (b->plc),
		// or it lies inside a protecting ball of near vertex (ivf->rejflag & 4).
		// Collect the list of vertices of the initial cavity.
		if(loc == OUTSIDE)
		{
			pts = (point *)&( searchtet->tet[ 4 ] );
			for(i = 0; i < 3; i++)
			{
				cavetetvertlist->newindex( (void **)&parypt );
				*parypt = pts[ i ];
			}
		}
		else if(loc == INTETRAHEDRON)
		{
			pts = (point *)&( searchtet->tet[ 4 ] );
			for(i = 0; i < 4; i++)
			{
				cavetetvertlist->newindex( (void **)&parypt );
				*parypt = pts[ i ];
			}
		}
		else if(loc == ONFACE)
		{
			pts = (point *)&( searchtet->tet[ 4 ] );
			for(i = 0; i < 3; i++)
			{
				cavetetvertlist->newindex( (void **)&parypt );
				*parypt = pts[ i ];
			}
			if(pts[ 3 ] != dummypoint)
			{
				cavetetvertlist->newindex( (void **)&parypt );
				*parypt = pts[ 3 ];
			}
			fsym( *searchtet, spintet );
			if(oppo( spintet ) != dummypoint)
			{
				cavetetvertlist->newindex( (void **)&parypt );
				*parypt = oppo( spintet );
			}
		}
		else if(loc == ONEDGE)
		{
			spintet = *searchtet;
			cavetetvertlist->newindex( (void **)&parypt );
			*parypt = org( spintet );
			cavetetvertlist->newindex( (void **)&parypt );
			*parypt = dest( spintet );
			while(1)
			{
				if(apex( spintet ) != dummypoint)
				{
					cavetetvertlist->newindex( (void **)&parypt );
					*parypt = apex( spintet );
				}
				fnextself( spintet );
				if(spintet.tet == searchtet->tet) break;
			}
		}

		int rejptflag = ( ivf->rejflag & 4 );
		REAL rd;
		pts = NULL;

		for(i = 0; i < cavetetvertlist->objects; i++)
		{
			parypt = (point *)fastlookup( cavetetvertlist, i );
			rd = distance( *parypt, insertpt );
			// Is the point very close to an existing point?
			if(rd < minedgelength)
			{
				pts = parypt;
				loc = NEARVERTEX;
				break;
			}
			if(rejptflag)
			{
				// Is the point encroaches upon an existing point?
				if(rd < ( 0.5 * ( *parypt )[ pointmtrindex ] ))
				{
					pts = parypt;
					loc = ENCVERTEX;
					break;
				}
			}
		}
		cavetetvertlist->restart( ); // Clear the work list.

		if(pts != NULL)
		{
			// The point is either too close to an existing vertex (NEARVERTEX)
			//   or encroaches upon (inside the protecting ball) of that vertex.
			if(loc == NEARVERTEX)
			{
				if(!issteinerpoint( insertpt ) && b->nomergevertex)
				{ // -M0/1 option.
					// 'insertpt' is an input vertex. 
					// In this case, we still insert this vertex. Issue a warning.
					if(!b->quiet)
					{
						myprintf( "Warning:  Two points, %d and %d, are very close.\n",
								pointmark( insertpt ), pointmark( *pts ) );
						myprintf( "  Creating a very short edge (len = %g) (< %g).\n",
								rd, minedgelength );
						myprintf( "  You may try a smaller tolerance (-T) (current is %g)\n",
								b->epsilon );
						myprintf( "  to avoid this warning.\n" );
					}
				}
				else
				{
					point2tetorg( *pts, *searchtet );
					insertpoint_abort( splitseg, ivf );
					ivf->iloc = (int)loc;
					return 0;
				}
			}
			else
			{ // loc == ENCVERTEX
				// The point lies inside the protection ball.
				point2tetorg( *pts, *searchtet );
				insertpoint_abort( splitseg, ivf );
				ivf->iloc = (int)loc;
				return 0;
			}
		}
	} // if (b->plc && (loc != INSTAR))

	if(b->weighted || ivf->cdtflag || ivf->smlenflag
		)
	{
		// There may be other vertices inside C(p). We need to find them.
		// Collect all vertices of C(p).
		for(i = 0; i < caveoldtetlist->objects; i++)
		{
			cavetet = (triface *)fastlookup( caveoldtetlist, i );
			//assert(infected(*cavetet));
			pts = (point *)&( cavetet->tet[ 4 ] );
			for(j = 0; j < 4; j++)
			{
				if(pts[ j ] != dummypoint)
				{
					if(!pinfected( pts[ j ] ))
					{
						pinfect( pts[ j ] );
						cavetetvertlist->newindex( (void **)&parypt );
						*parypt = pts[ j ];
					}
				}
			} // j
		} // i
		// Uninfect all collected (cavity) vertices.
		for(i = 0; i < cavetetvertlist->objects; i++)
		{
			parypt = (point *)fastlookup( cavetetvertlist, i );
			puninfect( *parypt );
		}
		if(ivf->smlenflag)
		{
			REAL len;
			// Get the length of the shortest edge connecting to 'newpt'.
			parypt = (point *)fastlookup( cavetetvertlist, 0 );
			ivf->smlen = distance( *parypt, insertpt );
			ivf->parentpt = *parypt;
			for(i = 1; i < cavetetvertlist->objects; i++)
			{
				parypt = (point *)fastlookup( cavetetvertlist, i );
				len = distance( *parypt, insertpt );
				if(len < ivf->smlen)
				{
					ivf->smlen = len;
					ivf->parentpt = *parypt;
				}
			}
		}
	}


	if(ivf->cdtflag)
	{
		// Unmark tets.
		for(i = 0; i < caveoldtetlist->objects; i++)
		{
			cavetet = (triface *)fastlookup( caveoldtetlist, i );
			unmarktest( *cavetet );
		}
		for(i = 0; i < cavebdrylist->objects; i++)
		{
			cavetet = (triface *)fastlookup( cavebdrylist, i );
			unmarktest( *cavetet );
		}
		// Clean up arrays which are not needed.
		cavetetlist->restart( );
		if(checksubsegflag)
		{
			cavetetseglist->restart( );
		}
		if(checksubfaceflag)
		{
			cavetetshlist->restart( );
		}
		return 1;
	}

	// Before re-mesh C(p). Process the segments and subfaces which are on the
	//   boundary of C(p). Make sure that each such segment or subface is
	//   connecting to a tet outside C(p). So we can re-connect them to the
	//   new tets inside the C(p) later.

	if(checksubsegflag)
	{
		for(i = 0; i < cavetetseglist->objects; i++)
		{
			paryseg = (face *)fastlookup( cavetetseglist, i );
			// Operate on it if it is not the splitting segment, i.e., in sC(p).
			if(!smarktested( *paryseg ))
			{
				// Check if the segment is inside the cavity.
				//   'j' counts the num of adjacent tets of this seg.
				//   'k' counts the num of adjacent tets which are 'sinfected'.
				j = k = 0;
				sstpivot1( *paryseg, neightet );
				spintet = neightet;
				while(1)
				{
					j++;
					if(!infected( spintet ))
					{
						neineitet = spintet; // An outer tet. Remember it.
					}
					else
					{
						k++; // An in tet.
					}
					fnextself( spintet );
					if(spintet.tet == neightet.tet) break;
				}
				// assert(j > 0);
				if(k == 0)
				{
					// The segment is not connect to C(p) anymore. Remove it by
					//   Replacing it by the last entry of this list.
					s = cavetetseglist->objects - 1;
					checkseg = *(face *)fastlookup( cavetetseglist, s );
					*paryseg = checkseg;
					cavetetseglist->objects--;
					i--;
				}
				else if(k < j)
				{
					// The segment is on the boundary of C(p).
					sstbond1( *paryseg, neineitet );
				}
				else
				{ // k == j
					// The segment is inside C(p).
					if(!ivf->splitbdflag)
					{
						checkseg = *paryseg;
						sinfect( checkseg ); // Flag it as an interior segment.
						caveencseglist->newindex( (void **)&paryseg );
						*paryseg = checkseg;
					}
					else
					{
						//assert(0); // Not possible.
						terminatetetgen( this, 2 );
					}
				}
			}
			else
			{
				// assert(smarktested(*paryseg));
				// Flag it as an interior segment. Do not queue it, since it will
				//   be deleted after the segment splitting.
				sinfect( *paryseg );
			}
		} // i
	} // if (checksubsegflag)

	if(checksubfaceflag)
	{
		for(i = 0; i < cavetetshlist->objects; i++)
		{
			parysh = (face *)fastlookup( cavetetshlist, i );
			// Operate on it if it is not inside the sub-cavity sC(p).
			if(!smarktested( *parysh ))
			{
				// Check if this subface is inside the cavity.
				k = 0;
				for(j = 0; j < 2; j++)
				{
					stpivot( *parysh, neightet );
					if(!infected( neightet ))
					{
						checksh = *parysh; // Remember this side.
					}
					else
					{
						k++;
					}
					sesymself( *parysh );
				}
				if(k == 0)
				{
					// The subface is not connected to C(p). Remove it.
					s = cavetetshlist->objects - 1;
					checksh = *(face *)fastlookup( cavetetshlist, s );
					*parysh = checksh;
					cavetetshlist->objects--;
					i--;
				}
				else if(k == 1)
				{
					// This side is the outer boundary of C(p).
					*parysh = checksh;
				}
				else
				{ // k == 2
					if(!ivf->splitbdflag)
					{
						checksh = *parysh;
						sinfect( checksh ); // Flag it.
						caveencshlist->newindex( (void **)&parysh );
						*parysh = checksh;
					}
					else
					{
						//assert(0); // Not possible.
						terminatetetgen( this, 2 );
					}
				}
			}
			else
			{
				// assert(smarktested(*parysh));
				// Flag it as an interior subface. Do not queue it. It will be
				//   deleted after the facet point insertion.
				sinfect( *parysh );
			}
		} // i
	} // if (checksubfaceflag)

	// Create new tetrahedra to fill the cavity.

	for(i = 0; i < cavebdrylist->objects; i++)
	{
		cavetet = (triface *)fastlookup( cavebdrylist, i );
		neightet = *cavetet;
		unmarktest( neightet ); // Unmark it.
		// Get the oldtet (inside the cavity).
		fsym( neightet, oldtet );
		if(apex( neightet ) != dummypoint)
		{
			// Create a new tet in the cavity.
			maketetrahedron( &newtet );
			setorg( newtet, dest( neightet ) );
			setdest( newtet, org( neightet ) );
			setapex( newtet, apex( neightet ) );
			setoppo( newtet, insertpt );
		}
		else
		{
			// Create a new hull tet.
			hullsize++;
			maketetrahedron( &newtet );
			setorg( newtet, org( neightet ) );
			setdest( newtet, dest( neightet ) );
			setapex( newtet, insertpt );
			setoppo( newtet, dummypoint ); // It must opposite to face 3.
			// Adjust back to the cavity bounday face.
			esymself( newtet );
		}
		// The new tet inherits attribtes from the old tet.
		for(j = 0; j < numelemattrib; j++)
		{
			attrib = elemattribute( oldtet.tet, j );
			setelemattribute( newtet.tet, j, attrib );
		}
		if(b->varvolume)
		{
			volume = volumebound( oldtet.tet );
			setvolumebound( newtet.tet, volume );
		}
		// Connect newtet <==> neightet, this also disconnect the old bond.
		bond( newtet, neightet );
		// oldtet still connects to neightet.
		*cavetet = oldtet; // *cavetet = newtet;
	} // i

	// Set a handle for speeding point location.
	recenttet = newtet;
	//setpoint2tet(insertpt, encode(newtet));
	setpoint2tet( insertpt, (tetrahedron)( newtet.tet ) );

	// Re-use this list to save new interior cavity faces.
	cavetetlist->restart( );

	// Connect adjacent new tetrahedra together.
	for(i = 0; i < cavebdrylist->objects; i++)
	{
		cavetet = (triface *)fastlookup( cavebdrylist, i );
		// cavtet is an oldtet, get the newtet at this face.
		oldtet = *cavetet;
		fsym( oldtet, neightet );
		fsym( neightet, newtet );
		// Comment: oldtet and newtet must be at the same directed edge.
		// Connect the three other faces of this newtet.
		for(j = 0; j < 3; j++)
		{
			esym( newtet, neightet ); // Go to the face.
			if(neightet.tet[ neightet.ver & 3 ] == NULL)
			{
				// Find the adjacent face of this newtet.
				spintet = oldtet;
				while(1)
				{
					fnextself( spintet );
					if(!infected( spintet )) break;
				}
				fsym( spintet, newneitet );
				esymself( newneitet );
				bond( neightet, newneitet );
				if(ivf->lawson > 1)
				{
					cavetetlist->newindex( (void **)&parytet );
					*parytet = neightet;
				}
			}
			//setpoint2tet(org(newtet), encode(newtet));
			setpoint2tet( org( newtet ), (tetrahedron)( newtet.tet ) );
			enextself( newtet );
			enextself( oldtet );
		}
		*cavetet = newtet; // Save the new tet.
	} // i

	if(checksubfaceflag)
	{
		// Connect subfaces on the boundary of the cavity to the new tets.
		for(i = 0; i < cavetetshlist->objects; i++)
		{
			parysh = (face *)fastlookup( cavetetshlist, i );
			// Connect it if it is not a missing subface.
			if(!sinfected( *parysh ))
			{
				stpivot( *parysh, neightet );
				fsym( neightet, spintet );
				sesymself( *parysh );
				tsbond( spintet, *parysh );
			}
		}
	}

	if(checksubsegflag)
	{
		// Connect segments on the boundary of the cavity to the new tets.
		for(i = 0; i < cavetetseglist->objects; i++)
		{
			paryseg = (face *)fastlookup( cavetetseglist, i );
			// Connect it if it is not a missing segment.
			if(!sinfected( *paryseg ))
			{
				sstpivot1( *paryseg, neightet );
				spintet = neightet;
				while(1)
				{
					tssbond1( spintet, *paryseg );
					fnextself( spintet );
					if(spintet.tet == neightet.tet) break;
				}
			}
		}
	}

	if(( ( splitsh != NULL ) && ( splitsh->sh != NULL ) ) ||
		( ( splitseg != NULL ) && ( splitseg->sh != NULL ) ))
	{
		// Split a subface or a segment.
		sinsertvertex( insertpt, splitsh, splitseg, ivf->sloc, ivf->sbowywat, 0 );
	}

	if(checksubfaceflag)
	{
		if(ivf->splitbdflag)
		{
			// Recover new subfaces in C(p).
			for(i = 0; i < caveshbdlist->objects; i++)
			{
				// Get an old subface at edge [a, b].
				parysh = (face *)fastlookup( caveshbdlist, i );
				spivot( *parysh, checksh ); // The new subface [a, b, p].
				// Do not recover a deleted new face (degenerated).
				if(checksh.sh[ 3 ] != NULL)
				{
					// Note that the old subface still connects to adjacent old tets 
					//   of C(p), which still connect to the tets outside C(p).
					stpivot( *parysh, neightet );
					// Find the adjacent tet containing the edge [a,b] outside C(p).
					spintet = neightet;
					while(1)
					{
						fnextself( spintet );
						if(!infected( spintet )) break;
					}
					// The adjacent tet connects to a new tet in C(p).
					fsym( spintet, neightet );
					// Find the tet containing the face [a, b, p].
					spintet = neightet;
					while(1)
					{
						fnextself( spintet );
						if(apex( spintet ) == insertpt) break;
					}
					// Adjust the edge direction in spintet and checksh.
					if(sorg( checksh ) != org( spintet ))
					{
						sesymself( checksh );
					}
					// Connect the subface to two adjacent tets.
					tsbond( spintet, checksh );
					fsymself( spintet );
					sesymself( checksh );
					tsbond( spintet, checksh );
				} // if (checksh.sh[3] != NULL)
			}
		}
		else
		{
			// The Boundary recovery phase.
			// Put all new subfaces into stack for recovery.
			for(i = 0; i < caveshbdlist->objects; i++)
			{
				// Get an old subface at edge [a, b].
				parysh = (face *)fastlookup( caveshbdlist, i );
				spivot( *parysh, checksh ); // The new subface [a, b, p].
				// Do not recover a deleted new face (degenerated).
				if(checksh.sh[ 3 ] != NULL)
				{
					subfacstack->newindex( (void **)&parysh );
					*parysh = checksh;
				}
			}
			// Put all interior subfaces into stack for recovery.
			for(i = 0; i < caveencshlist->objects; i++)
			{
				parysh = (face *)fastlookup( caveencshlist, i );
				// Some subfaces inside C(p) might be split in sinsertvertex().
				//   Only queue those faces which are not split.
				if(!smarktested( *parysh ))
				{
					checksh = *parysh;
					suninfect( checksh );
					stdissolve( checksh ); // Detach connections to old tets.
					subfacstack->newindex( (void **)&parysh );
					*parysh = checksh;
				}
			}
		}
	} // if (checksubfaceflag)

	if(checksubsegflag)
	{
		if(ivf->splitbdflag)
		{
			if(splitseg != NULL)
			{
				// Recover the two new subsegments in C(p).
				for(i = 0; i < cavesegshlist->objects; i++)
				{
					paryseg = (face *)fastlookup( cavesegshlist, i );
					// Insert this subsegment into C(p).
					checkseg = *paryseg;
					// Get the adjacent new subface.
					checkseg.shver = 0;
					spivot( checkseg, checksh );
					if(checksh.sh != NULL)
					{
						// Get the adjacent new tetrahedron.
						stpivot( checksh, neightet );
					}
					else
					{
						// It's a dangling segment.
						point2tetorg( sorg( checkseg ), neightet );
						finddirection( &neightet, sdest( checkseg ) );
					}
					sstbond1( checkseg, neightet );
					spintet = neightet;
					while(1)
					{
						tssbond1( spintet, checkseg );
						fnextself( spintet );
						if(spintet.tet == neightet.tet) break;
					}
				}
			} // if (splitseg != NULL)
		}
		else
		{
			// The Boundary Recovery Phase.  
			// Queue missing segments in C(p) for recovery.
			if(splitseg != NULL)
			{
				// Queue two new subsegments in C(p) for recovery.
				for(i = 0; i < cavesegshlist->objects; i++)
				{
					paryseg = (face *)fastlookup( cavesegshlist, i );
					checkseg = *paryseg;
					//sstdissolve1(checkseg); // It has not been connected yet.
					s = randomnation( subsegstack->objects + 1 );
					subsegstack->newindex( (void **)&paryseg );
					*paryseg = *(face *)fastlookup( subsegstack, s );
					paryseg = (face *)fastlookup( subsegstack, s );
					*paryseg = checkseg;
				}
			} // if (splitseg != NULL)
			for(i = 0; i < caveencseglist->objects; i++)
			{
				paryseg = (face *)fastlookup( caveencseglist, i );
				if(!smarktested( *paryseg ))
				{ // It may be split.
					checkseg = *paryseg;
					suninfect( checkseg );
					sstdissolve1( checkseg ); // Detach connections to old tets.
					s = randomnation( subsegstack->objects + 1 );
					subsegstack->newindex( (void **)&paryseg );
					*paryseg = *(face *)fastlookup( subsegstack, s );
					paryseg = (face *)fastlookup( subsegstack, s );
					*paryseg = checkseg;
				}
			}
		}
	} // if (checksubsegflag)

	if(b->weighted
		)
	{
		// Some vertices may be completed inside the cavity. They must be
		//   detected and added to recovering list.
		// Since every "live" vertex must contain a pointer to a non-dead
		//   tetrahedron, we can check for each vertex this pointer.
		for(i = 0; i < cavetetvertlist->objects; i++)
		{
			pts = (point *)fastlookup( cavetetvertlist, i );
			decode( point2tet( *pts ), *searchtet );
			if(infected( *searchtet ))
			{
				if(b->weighted)
				{
					if(b->verbose > 1)
					{
						myprintf( "    Point #%d is non-regular after the insertion of #%d.\n",
								pointmark( *pts ), pointmark( insertpt ) );
					}
					setpointtype( *pts, NREGULARVERTEX );
					nonregularcount++;
				}
			}
		}
	}

	if(ivf->chkencflag & 1)
	{
		// Queue all segment outside C(p).
		for(i = 0; i < cavetetseglist->objects; i++)
		{
			paryseg = (face *)fastlookup( cavetetseglist, i );
			// Skip if it is the split segment.
			if(!sinfected( *paryseg ))
			{
				enqueuesubface( badsubsegs, paryseg );
			}
		}
		if(splitseg != NULL)
		{
			// Queue the two new subsegments inside C(p).
			for(i = 0; i < cavesegshlist->objects; i++)
			{
				paryseg = (face *)fastlookup( cavesegshlist, i );
				enqueuesubface( badsubsegs, paryseg );
			}
		}
	} // if (chkencflag & 1)

	if(ivf->chkencflag & 2)
	{
		// Queue all subfaces outside C(p).
		for(i = 0; i < cavetetshlist->objects; i++)
		{
			parysh = (face *)fastlookup( cavetetshlist, i );
			// Skip if it is a split subface.
			if(!sinfected( *parysh ))
			{
				enqueuesubface( badsubfacs, parysh );
			}
		}
		// Queue all new subfaces inside C(p).
		for(i = 0; i < caveshbdlist->objects; i++)
		{
			// Get an old subface at edge [a, b].
			parysh = (face *)fastlookup( caveshbdlist, i );
			spivot( *parysh, checksh ); // checksh is a new subface [a, b, p].
			// Do not recover a deleted new face (degenerated).
			if(checksh.sh[ 3 ] != NULL)
			{
				enqueuesubface( badsubfacs, &checksh );
			}
		}
	} // if (chkencflag & 2)

	if(ivf->chkencflag & 4)
	{
		// Queue all new tetrahedra in C(p).
		for(i = 0; i < cavebdrylist->objects; i++)
		{
			cavetet = (triface *)fastlookup( cavebdrylist, i );
			enqueuetetrahedron( cavetet );
		}
	}

	// C(p) is re-meshed successfully.

	// Delete the old tets in C(p).
	for(i = 0; i < caveoldtetlist->objects; i++)
	{
		searchtet = (triface *)fastlookup( caveoldtetlist, i );
		if(ishulltet( *searchtet ))
		{
			hullsize--;
		}
		tetrahedrondealloc( searchtet->tet );
	}

	if(( ( splitsh != NULL ) && ( splitsh->sh != NULL ) ) ||
		( ( splitseg != NULL ) && ( splitseg->sh != NULL ) ))
	{
		// Delete the old subfaces in sC(p).
		for(i = 0; i < caveshlist->objects; i++)
		{
			parysh = (face *)fastlookup( caveshlist, i );
			if(checksubfaceflag)
			{//if (bowywat == 2) {
				// It is possible that this subface still connects to adjacent
				//   tets which are not in C(p). If so, clear connections in the
				//   adjacent tets at this subface.
				stpivot( *parysh, neightet );
				if(neightet.tet != NULL)
				{
					if(neightet.tet[ 4 ] != NULL)
					{
						// Found an adjacent tet. It must be not in C(p).
						tsdissolve( neightet );
						fsymself( neightet );
						tsdissolve( neightet );
					}
				}
			}
			shellfacedealloc( subfaces, parysh->sh );
		}
		if(( splitseg != NULL ) && ( splitseg->sh != NULL ))
		{
			// Delete the old segment in sC(p).
			shellfacedealloc( subsegs, splitseg->sh );
		}
	}

	if(ivf->lawson)
	{
		for(i = 0; i < cavebdrylist->objects; i++)
		{
			searchtet = (triface *)fastlookup( cavebdrylist, i );
			flippush( flipstack, searchtet );
		}
		if(ivf->lawson > 1)
		{
			for(i = 0; i < cavetetlist->objects; i++)
			{
				searchtet = (triface *)fastlookup( cavetetlist, i );
				flippush( flipstack, searchtet );
			}
		}
	}


	// Clean the working lists.

	caveoldtetlist->restart( );
	cavebdrylist->restart( );
	cavetetlist->restart( );

	if(checksubsegflag)
	{
		cavetetseglist->restart( );
		caveencseglist->restart( );
	}

	if(checksubfaceflag)
	{
		cavetetshlist->restart( );
		caveencshlist->restart( );
	}

	if(b->weighted || ivf->smlenflag
		)
	{
		cavetetvertlist->restart( );
	}

	if(( ( splitsh != NULL ) && ( splitsh->sh != NULL ) ) ||
		( ( splitseg != NULL ) && ( splitseg->sh != NULL ) ))
	{
		caveshlist->restart( );
		caveshbdlist->restart( );
		cavesegshlist->restart( );
	}

	return 1; // Point is inserted.
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// insertpoint_abort()    Abort the insertion of a new vertex.               //
//                                                                           //
// The cavity will be restored.  All working lists are cleared.              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::insertpoint_abort( face *splitseg, insertvertexflags *ivf )
{
	triface *cavetet;
	face *parysh;
	int i;

	for(i = 0; i < caveoldtetlist->objects; i++)
	{
		cavetet = (triface *)fastlookup( caveoldtetlist, i );
		uninfect( *cavetet );
		unmarktest( *cavetet );
	}
	for(i = 0; i < cavebdrylist->objects; i++)
	{
		cavetet = (triface *)fastlookup( cavebdrylist, i );
		unmarktest( *cavetet );
	}
	cavetetlist->restart( );
	cavebdrylist->restart( );
	caveoldtetlist->restart( );
	cavetetseglist->restart( );
	cavetetshlist->restart( );
	if(ivf->splitbdflag)
	{
		if(( splitseg != NULL ) && ( splitseg->sh != NULL ))
		{
			sunmarktest( *splitseg );
		}
		for(i = 0; i < caveshlist->objects; i++)
		{
			parysh = (face *)fastlookup( caveshlist, i );
			sunmarktest( *parysh );
		}
		caveshlist->restart( );
		cavesegshlist->restart( );
	}
}

////                                                                       ////
////                                                                       ////
//// flip_cxx /////////////////////////////////////////////////////////////////

//// delaunay_cxx /////////////////////////////////////////////////////////////
////                                                                       ////
////                                                                       ////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// transfernodes()    Read the vertices from the input (tetgenio).           //
//                                                                           //
// Transferring all points from input ('in->pointlist') to TetGen's 'points'.//
// All points are indexed (the first point index is 'in->firstnumber'). Each //
// point's type is initialized as UNUSEDVERTEX. The bounding box (xmax, xmin,//
// ...) and the diameter (longest) of the point set are calculated.          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::transfernodes( )
{
	point pointloop;
	REAL x, y, z, w;
	int coordindex;
	int attribindex;
	int mtrindex;
	int i, j;

	// Read the points.
	coordindex = 0;
	attribindex = 0;
	mtrindex = 0;
	for(i = 0; i < in->numberofpoints; i++)
	{
		makepoint( &pointloop, UNUSEDVERTEX );
		// Read the point coordinates.
		x = pointloop[ 0 ] = in->pointlist[ coordindex++ ];
		y = pointloop[ 1 ] = in->pointlist[ coordindex++ ];
		z = pointloop[ 2 ] = in->pointlist[ coordindex++ ];
		// Read the point attributes. (Including point weights.)
		for(j = 0; j < in->numberofpointattributes; j++)
		{
			pointloop[ 3 + j ] = in->pointattributelist[ attribindex++ ];
		}
		// Read the point metric tensor.
		for(j = 0; j < in->numberofpointmtrs; j++)
		{
			pointloop[ pointmtrindex + j ] = in->pointmtrlist[ mtrindex++ ];
		}
		if(b->weighted)
		{ // -w option
			if(in->numberofpointattributes > 0)
			{
				// The first point attribute is its weight.
				//w = in->pointattributelist[in->numberofpointattributes * i];
				w = pointloop[ 3 ];
			}
			else
			{
				// No given weight available. Default choose the maximum
				//   absolute value among its coordinates.        
				w = fabs( x );
				if(w < fabs( y )) w = fabs( y );
				if(w < fabs( z )) w = fabs( z );
			}
			if(b->weighted_param == 0)
			{
				pointloop[ 3 ] = x * x + y * y + z * z - w; // Weighted DT.
			}
			else
			{ // -w1 option
				pointloop[ 3 ] = w;  // Regular tetrahedralization.
			}
		}
		// Determine the smallest and largest x, y and z coordinates.
		if(i == 0)
		{
			xmin = xmax = x;
			ymin = ymax = y;
			zmin = zmax = z;
		}
		else
		{
			xmin = ( x < xmin ) ? x : xmin;
			xmax = ( x > xmax ) ? x : xmax;
			ymin = ( y < ymin ) ? y : ymin;
			ymax = ( y > ymax ) ? y : ymax;
			zmin = ( z < zmin ) ? z : zmin;
			zmax = ( z > zmax ) ? z : zmax;
		}
		if(b->psc)
		{
			// Read the geometry parameters.
			for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
			{
				setpointgeomuv( pointloop, tt, in->pointparamlist[ i ].uv[ tt ] );
			}
			setpointgeomtag( pointloop, in->pointparamlist[ i ].tag );
			if(in->pointparamlist[ i ].type == 0)
			{
				setpointtype( pointloop, RIDGEVERTEX );
			}
			else if(in->pointparamlist[ i ].type == 1)
			{
				setpointtype( pointloop, FREESEGVERTEX );
			}
			else if(in->pointparamlist[ i ].type == 2)
			{
				setpointtype( pointloop, FREEFACETVERTEX );
			}
			else if(in->pointparamlist[ i ].type == 3)
			{
				setpointtype( pointloop, FREEVOLVERTEX );
			}
		}
	}

	// 'longest' is the largest possible edge length formed by input vertices.
	x = xmax - xmin;
	y = ymax - ymin;
	z = zmax - zmin;
	longest = sqrt( x * x + y * y + z * z );
	if(longest == 0.0)
	{
		myprintf( "Error:  The point set is trivial.\n" );
		terminatetetgen( this, 10 );
	}
	// Two identical points are distinguished by 'minedgelength'.
	minedgelength = longest * b->epsilon;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// hilbert_init()    Initialize the Gray code permutation table.             //
//                                                                           //
// The table 'transgc' has 8 x 3 x 8 entries. It contains all possible Gray  //
// code sequences traveled by the 1st order Hilbert curve in 3 dimensions.   //
// The first column is the Gray code of the entry point of the curve, and    //
// the second column is the direction (0, 1, or 2, 0 means the x-axis) where //
// the exit point of curve lies.                                             //
//                                                                           //
// The table 'tsb1mod3' contains the numbers of trailing set '1' bits of the //
// indices from 0 to 7, modulo by '3'. The code for generating this table is //
// from: http://graphics.stanford.edu/~seander/bithacks.html.                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::hilbert_init( int n )
{
	int gc[ 8 ], N, mask, travel_bit;
	int e, d, f, k, g;
	int v, c;
	int i;

	N = ( n == 2 ) ? 4 : 8;
	mask = ( n == 2 ) ? 3 : 7;

	// Generate the Gray code sequence.
	for(i = 0; i < N; i++)
	{
		gc[ i ] = i ^ ( i >> 1 );
	}

	for(e = 0; e < N; e++)
	{
		for(d = 0; d < n; d++)
		{
			// Calculate the end point (f).
			f = e ^ ( 1 << d );  // Toggle the d-th bit of 'e'.
			// travel_bit = 2**p, the bit we want to travel. 
			travel_bit = e ^ f;
			for(i = 0; i < N; i++)
			{
				// // Rotate gc[i] left by (p + 1) % n bits.
				k = gc[ i ] * ( travel_bit * 2 );
				g = ( ( k | ( k / N ) ) & mask );
				// Calculate the permuted Gray code by xor with the start point (e).
				transgc[ e ][ d ][ i ] = ( g ^ e );
			}
		} // d
	} // e

	// Count the consecutive '1' bits (trailing) on the right.
	tsb1mod3[ 0 ] = 0;
	for(i = 1; i < N; i++)
	{
		v = ~i; // Count the 0s.
		v = ( v ^ ( v - 1 ) ) >> 1; // Set v's trailing 0s to 1s and zero rest
		for(c = 0; v; c++)
		{
			v >>= 1;
		}
		tsb1mod3[ i ] = c % n;
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// hilbert_sort3()    Sort points using the 3d Hilbert curve.                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::hilbert_split( point* vertexarray, int arraysize, int gc0, int gc1,
							   REAL bxmin, REAL bxmax, REAL bymin, REAL bymax,
							   REAL bzmin, REAL bzmax )
{
	point swapvert;
	int axis, d;
	REAL split;
	int i, j;


	// Find the current splitting axis. 'axis' is a value 0, or 1, or 2, which 
	//   correspoding to x-, or y- or z-axis.
	axis = ( gc0 ^ gc1 ) >> 1;

	// Calulate the split position along the axis.
	if(axis == 0)
	{
		split = 0.5 * ( bxmin + bxmax );
	}
	else if(axis == 1)
	{
		split = 0.5 * ( bymin + bymax );
	}
	else
	{ // == 2
		split = 0.5 * ( bzmin + bzmax );
	}

	// Find the direction (+1 or -1) of the axis. If 'd' is +1, the direction
	//   of the axis is to the positive of the axis, otherwise, it is -1.
	d = ( ( gc0 & ( 1 << axis ) ) == 0 ) ? 1 : -1;


	// Partition the vertices into left- and right-arrays such that left points
	//   have Hilbert indices lower than the right points.
	i = 0;
	j = arraysize - 1;

	// Partition the vertices into left- and right-arrays.
	if(d > 0)
	{
		do
		{
			for(; i < arraysize; i++)
			{
				if(vertexarray[ i ][ axis ] >= split) break;
			}
			for(; j >= 0; j--)
			{
				if(vertexarray[ j ][ axis ] < split) break;
			}
			// Is the partition finished?
			if(i == ( j + 1 )) break;
			// Swap i-th and j-th vertices.
			swapvert = vertexarray[ i ];
			vertexarray[ i ] = vertexarray[ j ];
			vertexarray[ j ] = swapvert;
			// Continue patitioning the array;
		}
		while(true);
	}
	else
	{
		do
		{
			for(; i < arraysize; i++)
			{
				if(vertexarray[ i ][ axis ] <= split) break;
			}
			for(; j >= 0; j--)
			{
				if(vertexarray[ j ][ axis ] > split) break;
			}
			// Is the partition finished?
			if(i == ( j + 1 )) break;
			// Swap i-th and j-th vertices.
			swapvert = vertexarray[ i ];
			vertexarray[ i ] = vertexarray[ j ];
			vertexarray[ j ] = swapvert;
			// Continue patitioning the array;
		}
		while(true);
	}

	return i;
}

void tetgenmesh::hilbert_sort3( point* vertexarray, int arraysize, int e, int d,
								REAL bxmin, REAL bxmax, REAL bymin, REAL bymax,
								REAL bzmin, REAL bzmax, int depth )
{
	REAL x1, x2, y1, y2, z1, z2;
	int p[ 9 ], w, e_w, d_w, k, ei, di;
	int n = 3, mask = 7;

	p[ 0 ] = 0;
	p[ 8 ] = arraysize;

	// Sort the points according to the 1st order Hilbert curve in 3d.
	p[ 4 ] = hilbert_split( vertexarray, p[ 8 ], transgc[ e ][ d ][ 3 ], transgc[ e ][ d ][ 4 ],
							bxmin, bxmax, bymin, bymax, bzmin, bzmax );
	p[ 2 ] = hilbert_split( vertexarray, p[ 4 ], transgc[ e ][ d ][ 1 ], transgc[ e ][ d ][ 2 ],
							bxmin, bxmax, bymin, bymax, bzmin, bzmax );
	p[ 1 ] = hilbert_split( vertexarray, p[ 2 ], transgc[ e ][ d ][ 0 ], transgc[ e ][ d ][ 1 ],
							bxmin, bxmax, bymin, bymax, bzmin, bzmax );
	p[ 3 ] = hilbert_split( &( vertexarray[ p[ 2 ] ] ), p[ 4 ] - p[ 2 ],
							transgc[ e ][ d ][ 2 ], transgc[ e ][ d ][ 3 ],
							bxmin, bxmax, bymin, bymax, bzmin, bzmax ) + p[ 2 ];
	p[ 6 ] = hilbert_split( &( vertexarray[ p[ 4 ] ] ), p[ 8 ] - p[ 4 ],
							transgc[ e ][ d ][ 5 ], transgc[ e ][ d ][ 6 ],
							bxmin, bxmax, bymin, bymax, bzmin, bzmax ) + p[ 4 ];
	p[ 5 ] = hilbert_split( &( vertexarray[ p[ 4 ] ] ), p[ 6 ] - p[ 4 ],
							transgc[ e ][ d ][ 4 ], transgc[ e ][ d ][ 5 ],
							bxmin, bxmax, bymin, bymax, bzmin, bzmax ) + p[ 4 ];
	p[ 7 ] = hilbert_split( &( vertexarray[ p[ 6 ] ] ), p[ 8 ] - p[ 6 ],
							transgc[ e ][ d ][ 6 ], transgc[ e ][ d ][ 7 ],
							bxmin, bxmax, bymin, bymax, bzmin, bzmax ) + p[ 6 ];

	if(b->hilbert_order > 0)
	{
		// A maximum order is prescribed. 
		if(( depth + 1 ) == b->hilbert_order)
		{
			// The maximum prescribed order is reached.
			return;
		}
	}

	// Recursively sort the points in sub-boxes.
	for(w = 0; w < 8; w++)
	{
		// w is the local Hilbert index (NOT Gray code).
		// Sort into the sub-box either there are more than 2 points in it, or
		//   the prescribed order of the curve is not reached yet.
		//if ((p[w+1] - p[w] > b->hilbert_limit) || (b->hilbert_order > 0)) {
		if(( p[ w + 1 ] - p[ w ] ) > b->hilbert_limit)
		{
			// Calculcate the start point (ei) of the curve in this sub-box.
			//   update e = e ^ (e(w) left_rotate (d+1)).
			if(w == 0)
			{
				e_w = 0;
			}
			else
			{
				//   calculate e(w) = gc(2 * floor((w - 1) / 2)).
				k = 2 * ( ( w - 1 ) / 2 );
				e_w = k ^ ( k >> 1 ); // = gc(k).
			}
			k = e_w;
			e_w = ( ( k << ( d + 1 ) ) & mask ) | ( ( k >> ( n - d - 1 ) ) & mask );
			ei = e ^ e_w;
			// Calulcate the direction (di) of the curve in this sub-box.
			//   update d = (d + d(w) + 1) % n
			if(w == 0)
			{
				d_w = 0;
			}
			else
			{
				d_w = ( ( w % 2 ) == 0 ) ? tsb1mod3[ w - 1 ] : tsb1mod3[ w ];
			}
			di = ( d + d_w + 1 ) % n;
			// Calculate the bounding box of the sub-box.
			if(transgc[ e ][ d ][ w ] & 1)
			{ // x-axis
				x1 = 0.5 * ( bxmin + bxmax );
				x2 = bxmax;
			}
			else
			{
				x1 = bxmin;
				x2 = 0.5 * ( bxmin + bxmax );
			}
			if(transgc[ e ][ d ][ w ] & 2)
			{ // y-axis
				y1 = 0.5 * ( bymin + bymax );
				y2 = bymax;
			}
			else
			{
				y1 = bymin;
				y2 = 0.5 * ( bymin + bymax );
			}
			if(transgc[ e ][ d ][ w ] & 4)
			{ // z-axis
				z1 = 0.5 * ( bzmin + bzmax );
				z2 = bzmax;
			}
			else
			{
				z1 = bzmin;
				z2 = 0.5 * ( bzmin + bzmax );
			}
			hilbert_sort3( &( vertexarray[ p[ w ] ] ), p[ w + 1 ] - p[ w ], ei, di,
						   x1, x2, y1, y2, z1, z2, depth + 1 );
		} // if (p[w+1] - p[w] > 1)
	} // w
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// brio_multiscale_sort()    Sort the points using BRIO and Hilbert curve.   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::brio_multiscale_sort( point* vertexarray, int arraysize,
									   int threshold, REAL ratio, int *depth )
{
	int middle;

	middle = 0;
	if(arraysize >= threshold)
	{
		( *depth )++;
		middle = arraysize * ratio;
		brio_multiscale_sort( vertexarray, middle, threshold, ratio, depth );
	}
	// Sort the right-array (rnd-th round) using the Hilbert curve.
	hilbert_sort3( &( vertexarray[ middle ] ), arraysize - middle, 0, 0, // e, d
				   xmin, xmax, ymin, ymax, zmin, zmax, 0 ); // depth.
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// randomnation()    Generate a random number between 0 and 'choices' - 1.   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

unsigned long tetgenmesh::randomnation( unsigned int choices )
{
	unsigned long newrandom;

	if(choices >= 714025l)
	{
		newrandom = ( randomseed * 1366l + 150889l ) % 714025l;
		randomseed = ( newrandom * 1366l + 150889l ) % 714025l;
		newrandom = newrandom * ( choices / 714025l ) + randomseed;
		if(newrandom >= choices)
		{
			return newrandom - choices;
		}
		else
		{
			return newrandom;
		}
	}
	else
	{
		randomseed = ( randomseed * 1366l + 150889l ) % 714025l;
		return randomseed % choices;
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// randomsample()    Randomly sample the tetrahedra for point loation.       //
//                                                                           //
// Searching begins from one of handles:  the input 'searchtet', a recently  //
// encountered tetrahedron 'recenttet',  or from one chosen from a random    //
// sample.  The choice is made by determining which one's origin is closest  //
// to the point we are searching for.                                        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::randomsample( point searchpt, triface *searchtet )
{
	tetrahedron *firsttet, *tetptr;
	point torg;
	void **sampleblock;
	uintptr_t alignptr;
	long sampleblocks, samplesperblock, samplenum;
	long tetblocks, i, j;
	REAL searchdist, dist;

	if(b->verbose > 2)
	{
		myprintf( "      Random sampling tetrahedra for searching point %d.\n",
				pointmark( searchpt ) );
	}

	if(!nonconvex)
	{
		if(searchtet->tet == NULL)
		{
			// A null tet. Choose the recenttet as the starting tet.
			*searchtet = recenttet;
		}

		// 'searchtet' should be a valid tetrahedron. Choose the base face
		//   whose vertices must not be 'dummypoint'.
		searchtet->ver = 3;
		// Record the distance from its origin to the searching point.
		torg = org( *searchtet );
		searchdist = ( searchpt[ 0 ] - torg[ 0 ] ) * ( searchpt[ 0 ] - torg[ 0 ] ) +
			( searchpt[ 1 ] - torg[ 1 ] ) * ( searchpt[ 1 ] - torg[ 1 ] ) +
			( searchpt[ 2 ] - torg[ 2 ] ) * ( searchpt[ 2 ] - torg[ 2 ] );

		// If a recently encountered tetrahedron has been recorded and has not
		//   been deallocated, test it as a good starting point.
		if(recenttet.tet != searchtet->tet)
		{
			recenttet.ver = 3;
			torg = org( recenttet );
			dist = ( searchpt[ 0 ] - torg[ 0 ] ) * ( searchpt[ 0 ] - torg[ 0 ] ) +
				( searchpt[ 1 ] - torg[ 1 ] ) * ( searchpt[ 1 ] - torg[ 1 ] ) +
				( searchpt[ 2 ] - torg[ 2 ] ) * ( searchpt[ 2 ] - torg[ 2 ] );
			if(dist < searchdist)
			{
				*searchtet = recenttet;
				searchdist = dist;
			}
		}
	}
	else
	{
		// The mesh is non-convex. Do not use 'recenttet'.
		searchdist = longest;
	}

	// Select "good" candidate using k random samples, taking the closest one.
	//   The number of random samples taken is proportional to the fourth root
	//   of the number of tetrahedra in the mesh. 
	while(samples * samples * samples * samples < tetrahedrons->items)
	{
		samples++;
	}
	// Find how much blocks in current tet pool.
	tetblocks = ( tetrahedrons->maxitems + b->tetrahedraperblock - 1 )
		/ b->tetrahedraperblock;
	// Find the average samples per block. Each block at least have 1 sample.
	samplesperblock = 1 + ( samples / tetblocks );
	sampleblocks = samples / samplesperblock;
	sampleblock = tetrahedrons->firstblock;
	for(i = 0; i < sampleblocks; i++)
	{
		alignptr = (uintptr_t)( sampleblock + 1 );
		firsttet = (tetrahedron *)
			( alignptr + (uintptr_t)tetrahedrons->alignbytes
			- ( alignptr % (uintptr_t)tetrahedrons->alignbytes ) );
		for(j = 0; j < samplesperblock; j++)
		{
			if(i == tetblocks - 1)
			{
				// This is the last block.
				samplenum = randomnation( (int)
										  ( tetrahedrons->maxitems - ( i * b->tetrahedraperblock ) ) );
			}
			else
			{
				samplenum = randomnation( b->tetrahedraperblock );
			}
			tetptr = (tetrahedron *)
				( firsttet + ( samplenum * tetrahedrons->itemwords ) );
			torg = (point)tetptr[ 4 ];
			if(torg != (point)NULL)
			{
				dist = ( searchpt[ 0 ] - torg[ 0 ] ) * ( searchpt[ 0 ] - torg[ 0 ] ) +
					( searchpt[ 1 ] - torg[ 1 ] ) * ( searchpt[ 1 ] - torg[ 1 ] ) +
					( searchpt[ 2 ] - torg[ 2 ] ) * ( searchpt[ 2 ] - torg[ 2 ] );
				if(dist < searchdist)
				{
					searchtet->tet = tetptr;
					searchtet->ver = 11; // torg = org(t);
					searchdist = dist;
				}
			}
			else
			{
				// A dead tet. Re-sample it.
				if(i != tetblocks - 1) j--;
			}
		}
		sampleblock = (void **)*sampleblock;
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// locate()    Find a tetrahedron containing a given point.                  //
//                                                                           //
// Begins its search from 'searchtet', assume there is a line segment L from //
// a vertex of 'searchtet' to the query point 'searchpt', and simply walk    //
// towards 'searchpt' by traversing all faces intersected by L.              //
//                                                                           //
// On completion, 'searchtet' is a tetrahedron that contains 'searchpt'. The //
// returned value indicates one of the following cases:                      //
//   - ONVERTEX, the search point lies on the origin of 'searchtet'.         //
//   - ONEDGE, the search point lies on an edge of 'searchtet'.              //
//   - ONFACE, the search point lies on a face of 'searchtet'.               //
//   - INTET, the search point lies in the interior of 'searchtet'.          //
//   - OUTSIDE, the search point lies outside the mesh. 'searchtet' is a     //
//              hull face which is visible by the search point.              //
//                                                                           //
// WARNING: This routine is designed for convex triangulations, and will not //
// generally work after the holes and concavities have been carved.          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

enum tetgenmesh::locateresult
	tetgenmesh::locate( point searchpt, triface* searchtet, int chkencflag )
{
		point torg, tdest, tapex, toppo;
		enum { ORGMOVE, DESTMOVE, APEXMOVE } nextmove;
		REAL ori, oriorg, oridest, oriapex;
		enum locateresult loc = OUTSIDE;
		int t1ver;
		int s;

		if(searchtet->tet == NULL)
		{
			// A null tet. Choose the recenttet as the starting tet.
			searchtet->tet = recenttet.tet;
		}

		// Check if we are in the outside of the convex hull.
		if(ishulltet( *searchtet ))
		{
			// Get its adjacent tet (inside the hull).
			searchtet->ver = 3;
			fsymself( *searchtet );
		}

		// Let searchtet be the face such that 'searchpt' lies above to it.
		for(searchtet->ver = 0; searchtet->ver < 4; searchtet->ver++)
		{
			torg = org( *searchtet );
			tdest = dest( *searchtet );
			tapex = apex( *searchtet );
			ori = orient3d( torg, tdest, tapex, searchpt );
			if(ori < 0.0) break;
		}
		if(searchtet->ver == 4)
		{
			terminatetetgen( this, 2 );
		}

		// Walk through tetrahedra to locate the point.
		while(true)
		{

			toppo = oppo( *searchtet );

			// Check if the vertex is we seek.
			if(toppo == searchpt)
			{
				// Adjust the origin of searchtet to be searchpt.
				esymself( *searchtet );
				eprevself( *searchtet );
				loc = ONVERTEX; // return ONVERTEX;
				break;
			}

			// We enter from one of serarchtet's faces, which face do we exit?
			oriorg = orient3d( tdest, tapex, toppo, searchpt );
			oridest = orient3d( tapex, torg, toppo, searchpt );
			oriapex = orient3d( torg, tdest, toppo, searchpt );

			// Now decide which face to move. It is possible there are more than one
			//   faces are viable moves. If so, randomly choose one.
			if(oriorg < 0)
			{
				if(oridest < 0)
				{
					if(oriapex < 0)
					{
						// All three faces are possible.
						s = randomnation( 3 ); // 's' is in {0,1,2}.
						if(s == 0)
						{
							nextmove = ORGMOVE;
						}
						else if(s == 1)
						{
							nextmove = DESTMOVE;
						}
						else
						{
							nextmove = APEXMOVE;
						}
					}
					else
					{
						// Two faces, opposite to origin and destination, are viable.
						//s = randomnation(2); // 's' is in {0,1}.
						if(randomnation( 2 ))
						{
							nextmove = ORGMOVE;
						}
						else
						{
							nextmove = DESTMOVE;
						}
					}
				}
				else
				{
					if(oriapex < 0)
					{
						// Two faces, opposite to origin and apex, are viable.
						//s = randomnation(2); // 's' is in {0,1}.
						if(randomnation( 2 ))
						{
							nextmove = ORGMOVE;
						}
						else
						{
							nextmove = APEXMOVE;
						}
					}
					else
					{
						// Only the face opposite to origin is viable.
						nextmove = ORGMOVE;
					}
				}
			}
			else
			{
				if(oridest < 0)
				{
					if(oriapex < 0)
					{
						// Two faces, opposite to destination and apex, are viable.
						//s = randomnation(2); // 's' is in {0,1}.
						if(randomnation( 2 ))
						{
							nextmove = DESTMOVE;
						}
						else
						{
							nextmove = APEXMOVE;
						}
					}
					else
					{
						// Only the face opposite to destination is viable.
						nextmove = DESTMOVE;
					}
				}
				else
				{
					if(oriapex < 0)
					{
						// Only the face opposite to apex is viable.
						nextmove = APEXMOVE;
					}
					else
					{
						// The point we seek must be on the boundary of or inside this
						//   tetrahedron. Check for boundary cases.
						if(oriorg == 0)
						{
							// Go to the face opposite to origin.
							enextesymself( *searchtet );
							if(oridest == 0)
							{
								eprevself( *searchtet ); // edge oppo->apex
								if(oriapex == 0)
								{
									// oppo is duplicated with p.
									loc = ONVERTEX; // return ONVERTEX;
									break;
								}
								loc = ONEDGE; // return ONEDGE;
								break;
							}
							if(oriapex == 0)
							{
								enextself( *searchtet ); // edge dest->oppo
								loc = ONEDGE; // return ONEDGE;
								break;
							}
							loc = ONFACE; // return ONFACE;
							break;
						}
						if(oridest == 0)
						{
							// Go to the face opposite to destination.
							eprevesymself( *searchtet );
							if(oriapex == 0)
							{
								eprevself( *searchtet ); // edge oppo->org
								loc = ONEDGE; // return ONEDGE;
								break;
							}
							loc = ONFACE; // return ONFACE;
							break;
						}
						if(oriapex == 0)
						{
							// Go to the face opposite to apex
							esymself( *searchtet );
							loc = ONFACE; // return ONFACE;
							break;
						}
						loc = INTETRAHEDRON; // return INTETRAHEDRON;
						break;
					}
				}
			}

			// Move to the selected face.
			if(nextmove == ORGMOVE)
			{
				enextesymself( *searchtet );
			}
			else if(nextmove == DESTMOVE)
			{
				eprevesymself( *searchtet );
			}
			else
			{
				esymself( *searchtet );
			}
			if(chkencflag)
			{
				// Check if we are walking across a subface.
				if(issubface( *searchtet ))
				{
					loc = ENCSUBFACE;
					break;
				}
			}
			// Move to the adjacent tetrahedron (maybe a hull tetrahedron).
			fsymself( *searchtet );
			if(oppo( *searchtet ) == dummypoint)
			{
				loc = OUTSIDE; // return OUTSIDE;
				break;
			}

			// Retreat the three vertices of the base face.
			torg = org( *searchtet );
			tdest = dest( *searchtet );
			tapex = apex( *searchtet );

		} // while (true)

		return loc;
	}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// flippush()    Push a face (possibly will be flipped) into flipstack.      //
//                                                                           //
// The face is marked. The flag is used to check the validity of the face on //
// its popup.  Some other flips may change it already.                       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::flippush( badface*& fstack, triface* flipface )
{
	if(!facemarked( *flipface ))
	{
		badface *newflipface = (badface *)flippool->alloc( );
		newflipface->tt = *flipface;
		markface( newflipface->tt );
		// Push this face into stack.
		newflipface->nextitem = fstack;
		fstack = newflipface;
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// incrementalflip()    Incrementally flipping to construct DT.              //
//                                                                           //
// Faces need to be checked for flipping are already queued in 'flipstack'.  //
// Return the total number of performed flips.                               //
//                                                                           //
// Comment:  This routine should be only used in the incremental Delaunay    //
// construction.  In other cases, lawsonflip3d() should be used.             // 
//                                                                           //
// If the new point lies outside of the convex hull ('hullflag' is set). The //
// incremental flip algorithm still works as usual.  However, we must ensure //
// that every flip (2-to-3 or 3-to-2) does not create a duplicated (existing)//
// edge or face. Otherwise, the underlying space of the triangulation becomes//
// non-manifold and it is not possible to flip further.                      //
// Thanks to Joerg Rambau and Frank Lutz for helping in this issue.          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::incrementalflip( point newpt, int hullflag, flipconstraints *fc )
{
	badface *popface;
	triface fliptets[ 5 ], *parytet;
	point *pts, *parypt, pe;
	REAL sign, ori;
	int flipcount = 0;
	int t1ver;
	int i;

	if(b->verbose > 2)
	{
		myprintf( "      Lawson flip (%ld faces).\n", flippool->items );
	}

	if(hullflag)
	{
		// 'newpt' lies in the outside of the convex hull. 
		// Mark all hull vertices which are connecting to it.
		popface = flipstack;
		while(popface != NULL)
		{
			pts = (point *)popface->tt.tet;
			for(i = 4; i < 8; i++)
			{
				if(( pts[ i ] != newpt ) && ( pts[ i ] != dummypoint ))
				{
					if(!pinfected( pts[ i ] ))
					{
						pinfect( pts[ i ] );
						cavetetvertlist->newindex( (void **)&parypt );
						*parypt = pts[ i ];
					}
				}
			}
			popface = popface->nextitem;
		}
	}

	// Loop until the queue is empty.
	while(flipstack != NULL)
	{

		// Pop a face from the stack.
		popface = flipstack;
		fliptets[ 0 ] = popface->tt;
		flipstack = flipstack->nextitem; // The next top item in stack.
		flippool->dealloc( (void *)popface );

		// Skip it if it is a dead tet (destroyed by previous flips).
		if(isdeadtet( fliptets[ 0 ] )) continue;
		// Skip it if it is not the same tet as we saved.
		if(!facemarked( fliptets[ 0 ] )) continue;

		unmarkface( fliptets[ 0 ] );

		if((point)fliptets[ 0 ].tet[ 7 ] == dummypoint)
		{
			// It must be a hull edge.
			fliptets[ 0 ].ver = epivot[ fliptets[ 0 ].ver ];
			// A hull edge. The current convex hull may be enlarged.
			fsym( fliptets[ 0 ], fliptets[ 1 ] );
			pts = (point *)fliptets[ 1 ].tet;
			ori = orient3d( pts[ 4 ], pts[ 5 ], pts[ 6 ], newpt );
			if(ori < 0)
			{
				// Visible. The convex hull will be enlarged.
				// Decide which flip (2-to-3, 3-to-2, or 4-to-1) to use.
				// Check if the tet [a,c,e,d] or [c,b,e,d] exists.
				enext( fliptets[ 1 ], fliptets[ 2 ] );
				eprev( fliptets[ 1 ], fliptets[ 3 ] );
				fnextself( fliptets[ 2 ] ); // [a,c,e,*]
				fnextself( fliptets[ 3 ] ); // [c,b,e,*]
				if(oppo( fliptets[ 2 ] ) == newpt)
				{
					if(oppo( fliptets[ 3 ] ) == newpt)
					{
						// Both tets exist! A 4-to-1 flip is found.
						terminatetetgen( this, 2 ); // Report a bug.
					}
					else
					{
						esym( fliptets[ 2 ], fliptets[ 0 ] );
						fnext( fliptets[ 0 ], fliptets[ 1 ] );
						fnext( fliptets[ 1 ], fliptets[ 2 ] );
						// Perform a 3-to-2 flip. Replace edge [c,a] by face [d,e,b].
						// This corresponds to my standard labels, where edge [e,d] is
						//   repalced by face [a,b,c], and a is the new vertex. 
						//   [0] [c,a,d,e] (d = newpt)
						//   [1] [c,a,e,b] (c = dummypoint)
						//   [2] [c,a,b,d]
						flip32( fliptets, 1, fc );
					}
				}
				else
				{
					if(oppo( fliptets[ 3 ] ) == newpt)
					{
						fnext( fliptets[ 3 ], fliptets[ 0 ] );
						fnext( fliptets[ 0 ], fliptets[ 1 ] );
						fnext( fliptets[ 1 ], fliptets[ 2 ] );
						// Perform a 3-to-2 flip. Replace edge [c,b] by face [d,a,e].
						//   [0] [c,b,d,a] (d = newpt)
						//   [1] [c,b,a,e] (c = dummypoint)
						//   [2] [c,b,e,d]
						flip32( fliptets, 1, fc );
					}
					else
					{
						if(hullflag)
						{
							// Reject this flip if pe is already marked.
							pe = oppo( fliptets[ 1 ] );
							if(!pinfected( pe ))
							{
								pinfect( pe );
								cavetetvertlist->newindex( (void **)&parypt );
								*parypt = pe;
								// Perform a 2-to-3 flip.
								flip23( fliptets, 1, fc );
							}
							else
							{
								// Reject this flip.
								flipcount--;
							}
						}
						else
						{
							// Perform a 2-to-3 flip. Replace face [a,b,c] by edge [e,d].
							//   [0] [a,b,c,d], d = newpt.
							//   [1] [b,a,c,e], c = dummypoint.
							flip23( fliptets, 1, fc );
						}
					}
				}
				flipcount++;
			}
			continue;
		} // if (dummypoint)

		fsym( fliptets[ 0 ], fliptets[ 1 ] );
		if((point)fliptets[ 1 ].tet[ 7 ] == dummypoint)
		{
			// A hull face is locally Delaunay.
			continue;
		}
		// Check if the adjacent tet has already been tested.
		if(marktested( fliptets[ 1 ] ))
		{
			// It has been tested and it is Delaunay.
			continue;
		}

		// Test whether the face is locally Delaunay or not.
		pts = (point *)fliptets[ 1 ].tet;
		if(b->weighted)
		{
			sign = orient4d_s( pts[ 4 ], pts[ 5 ], pts[ 6 ], pts[ 7 ], newpt,
							   pts[ 4 ][ 3 ], pts[ 5 ][ 3 ], pts[ 6 ][ 3 ], pts[ 7 ][ 3 ],
							   newpt[ 3 ] );
		}
		else
		{
			sign = insphere_s( pts[ 4 ], pts[ 5 ], pts[ 6 ], pts[ 7 ], newpt );
		}


		if(sign < 0)
		{
			point pd = newpt;
			point pe = oppo( fliptets[ 1 ] );
			// Check the convexity of its three edges. Stop checking either a
			//   locally non-convex edge (ori < 0) or a flat edge (ori = 0) is
			//   encountered, and 'fliptet' represents that edge.
			for(i = 0; i < 3; i++)
			{
				ori = orient3d( org( fliptets[ 0 ] ), dest( fliptets[ 0 ] ), pd, pe );
				if(ori <= 0) break;
				enextself( fliptets[ 0 ] );
			}
			if(ori > 0)
			{
				// A 2-to-3 flip is found.
				//   [0] [a,b,c,d], 
				//   [1] [b,a,c,e]. no dummypoint.
				flip23( fliptets, 0, fc );
				flipcount++;
			}
			else
			{ // ori <= 0
				// The edge ('fliptets[0]' = [a',b',c',d]) is non-convex or flat,
				//   where the edge [a',b'] is one of [a,b], [b,c], and [c,a].
				// Check if there are three or four tets sharing at this edge.        
				esymself( fliptets[ 0 ] ); // [b,a,d,c]
				for(i = 0; i < 3; i++)
				{
					fnext( fliptets[ i ], fliptets[ i + 1 ] );
				}
				if(fliptets[ 3 ].tet == fliptets[ 0 ].tet)
				{
					// A 3-to-2 flip is found. (No hull tet.)
					flip32( fliptets, 0, fc );
					flipcount++;
				}
				else
				{
					// There are more than 3 tets at this edge.
					fnext( fliptets[ 3 ], fliptets[ 4 ] );
					if(fliptets[ 4 ].tet == fliptets[ 0 ].tet)
					{
						if(ori == 0)
						{
							// A 4-to-4 flip is found. (Two hull tets may be involved.)
							// Current tets in 'fliptets':
							//   [0] [b,a,d,c] (d may be newpt)
							//   [1] [b,a,c,e]
							//   [2] [b,a,e,f] (f may be dummypoint)
							//   [3] [b,a,f,d]
							esymself( fliptets[ 0 ] ); // [a,b,c,d] 
							// A 2-to-3 flip replaces face [a,b,c] by edge [e,d].
							//   This creates a degenerate tet [e,d,a,b] (tmpfliptets[0]).
							//   It will be removed by the followed 3-to-2 flip.
							flip23( fliptets, 0, fc ); // No hull tet.
							fnext( fliptets[ 3 ], fliptets[ 1 ] );
							fnext( fliptets[ 1 ], fliptets[ 2 ] );
							// Current tets in 'fliptets':
							//   [0] [...]
							//   [1] [b,a,d,e] (degenerated, d may be new point).
							//   [2] [b,a,e,f] (f may be dummypoint)
							//   [3] [b,a,f,d]
							// A 3-to-2 flip replaces edge [b,a] by face [d,e,f].
							//   Hull tets may be involved (f may be dummypoint).
							flip32( &( fliptets[ 1 ] ), ( apex( fliptets[ 3 ] ) == dummypoint ), fc );
							flipcount++;
						}
					}
				}
			} // ori
		}
		else
		{
			// The adjacent tet is Delaunay. Mark it to avoid testing it again.
			marktest( fliptets[ 1 ] );
			// Save it for unmarking it later.
			cavebdrylist->newindex( (void **)&parytet );
			*parytet = fliptets[ 1 ];
		}

	} // while (flipstack)

	// Unmark saved tetrahedra.
	for(i = 0; i < cavebdrylist->objects; i++)
	{
		parytet = (triface *)fastlookup( cavebdrylist, i );
		unmarktest( *parytet );
	}
	cavebdrylist->restart( );

	if(hullflag)
	{
		// Unmark infected vertices.
		for(i = 0; i < cavetetvertlist->objects; i++)
		{
			parypt = (point *)fastlookup( cavetetvertlist, i );
			puninfect( *parypt );
		}
		cavetetvertlist->restart( );
	}


	return flipcount;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// initialdelaunay()    Create an initial Delaunay tetrahedralization.       //
//                                                                           //
// The tetrahedralization contains only one tetrahedron abcd, and four hull  //
// tetrahedra. The points pa, pb, pc, and pd must be linearly independent.   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::initialdelaunay( point pa, point pb, point pc, point pd )
{
	triface firsttet, tetopa, tetopb, tetopc, tetopd;
	triface worktet, worktet1;

	if(b->verbose > 2)
	{
		myprintf( "      Create init tet (%d, %d, %d, %d)\n", pointmark( pa ),
				pointmark( pb ), pointmark( pc ), pointmark( pd ) );
	}

	// Create the first tetrahedron.
	maketetrahedron( &firsttet );
	setvertices( firsttet, pa, pb, pc, pd );
	// Create four hull tetrahedra.
	maketetrahedron( &tetopa );
	setvertices( tetopa, pb, pc, pd, dummypoint );
	maketetrahedron( &tetopb );
	setvertices( tetopb, pc, pa, pd, dummypoint );
	maketetrahedron( &tetopc );
	setvertices( tetopc, pa, pb, pd, dummypoint );
	maketetrahedron( &tetopd );
	setvertices( tetopd, pb, pa, pc, dummypoint );
	hullsize += 4;

	// Connect hull tetrahedra to firsttet (at four faces of firsttet).
	bond( firsttet, tetopd );
	esym( firsttet, worktet );
	bond( worktet, tetopc ); // ab
	enextesym( firsttet, worktet );
	bond( worktet, tetopa ); // bc 
	eprevesym( firsttet, worktet );
	bond( worktet, tetopb ); // ca

	// Connect hull tetrahedra together (at six edges of firsttet).
	esym( tetopc, worktet );
	esym( tetopd, worktet1 );
	bond( worktet, worktet1 ); // ab
	esym( tetopa, worktet );
	eprevesym( tetopd, worktet1 );
	bond( worktet, worktet1 ); // bc
	esym( tetopb, worktet );
	enextesym( tetopd, worktet1 );
	bond( worktet, worktet1 ); // ca
	eprevesym( tetopc, worktet );
	enextesym( tetopb, worktet1 );
	bond( worktet, worktet1 ); // da
	eprevesym( tetopa, worktet );
	enextesym( tetopc, worktet1 );
	bond( worktet, worktet1 ); // db
	eprevesym( tetopb, worktet );
	enextesym( tetopa, worktet1 );
	bond( worktet, worktet1 ); // dc

	// Set the vertex type.
	if(pointtype( pa ) == UNUSEDVERTEX)
	{
		setpointtype( pa, VOLVERTEX );
	}
	if(pointtype( pb ) == UNUSEDVERTEX)
	{
		setpointtype( pb, VOLVERTEX );
	}
	if(pointtype( pc ) == UNUSEDVERTEX)
	{
		setpointtype( pc, VOLVERTEX );
	}
	if(pointtype( pd ) == UNUSEDVERTEX)
	{
		setpointtype( pd, VOLVERTEX );
	}

	setpoint2tet( pa, encode( firsttet ) );
	setpoint2tet( pb, encode( firsttet ) );
	setpoint2tet( pc, encode( firsttet ) );
	setpoint2tet( pd, encode( firsttet ) );

	// Remember the first tetrahedron.
	recenttet = firsttet;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// incrementaldelaunay()    Create a Delaunay tetrahedralization by          //
//                          the incremental approach.                        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


void tetgenmesh::incrementaldelaunay( clock_t& tv )
{
	triface searchtet;
	point *permutarray, swapvertex;
	REAL v1[ 3 ], v2[ 3 ], n[ 3 ];
	REAL bboxsize, bboxsize2, bboxsize3, ori;
	int randindex;
	int ngroup = 0;
	int i, j;

	if(!b->quiet)
	{
		myprintf( "Delaunizing vertices...\n" );
	}

	// Form a random permuation (uniformly at random) of the set of vertices.
	permutarray = new point[ in->numberofpoints ];
	points->traversalinit( );

	if(b->no_sort)
	{
		if(b->verbose)
		{
			myprintf( "  Using the input order.\n" );
		}
		for(i = 0; i < in->numberofpoints; i++)
		{
			permutarray[ i ] = (point)points->traverse( );
		}
	}
	else
	{
		if(b->verbose)
		{
			myprintf( "  Permuting vertices.\n" );
		}
		srand( in->numberofpoints );
		for(i = 0; i < in->numberofpoints; i++)
		{
			randindex = rand( ) % ( i + 1 ); // randomnation(i + 1);
			permutarray[ i ] = permutarray[ randindex ];
			permutarray[ randindex ] = (point)points->traverse( );
		}
		if(b->brio_hilbert)
		{ // -b option
			if(b->verbose)
			{
				myprintf( "  Sorting vertices.\n" );
			}
			hilbert_init( in->mesh_dim );
			brio_multiscale_sort( permutarray, in->numberofpoints, b->brio_threshold,
								  b->brio_ratio, &ngroup );
		}
	}

	tv = clock( ); // Remember the time for sorting points.

	// Calculate the diagonal size of its bounding box.
	bboxsize = sqrt( norm2( xmax - xmin, ymax - ymin, zmax - zmin ) );
	bboxsize2 = bboxsize * bboxsize;
	bboxsize3 = bboxsize2 * bboxsize;

	// Make sure the second vertex is not identical with the first one.
	i = 1;
	while(( distance( permutarray[ 0 ], permutarray[ i ] ) / bboxsize )<b->epsilon)
	{
		i++;
		if(i == in->numberofpoints - 1)
		{
			myprintf( "Exception:  All vertices are (nearly) identical (Tol = %g).\n",
					b->epsilon );
			terminatetetgen( this, 10 );
		}
	}
	if(i > 1)
	{
		// Swap to move the non-identical vertex from index i to index 1.
		swapvertex = permutarray[ i ];
		permutarray[ i ] = permutarray[ 1 ];
		permutarray[ 1 ] = swapvertex;
	}

	// Make sure the third vertex is not collinear with the first two.
	// Acknowledgement:  Thanks Jan Pomplun for his correction by using 
	//   epsilon^2 and epsilon^3 (instead of epsilon). 2013-08-15.
	i = 2;
	for(j = 0; j < 3; j++)
	{
		v1[ j ] = permutarray[ 1 ][ j ] - permutarray[ 0 ][ j ];
		v2[ j ] = permutarray[ i ][ j ] - permutarray[ 0 ][ j ];
	}
	cross( v1, v2, n );
	while(( sqrt( norm2( n[ 0 ], n[ 1 ], n[ 2 ] ) ) / bboxsize2 ) <
		   ( b->epsilon * b->epsilon ))
	{
		i++;
		if(i == in->numberofpoints - 1)
		{
			myprintf( "Exception:  All vertices are (nearly) collinear (Tol = %g).\n",
					b->epsilon );
			terminatetetgen( this, 10 );
		}
		for(j = 0; j < 3; j++)
		{
			v2[ j ] = permutarray[ i ][ j ] - permutarray[ 0 ][ j ];
		}
		cross( v1, v2, n );
	}
	if(i > 2)
	{
		// Swap to move the non-identical vertex from index i to index 1.
		swapvertex = permutarray[ i ];
		permutarray[ i ] = permutarray[ 2 ];
		permutarray[ 2 ] = swapvertex;
	}

	// Make sure the fourth vertex is not coplanar with the first three.
	i = 3;
	ori = orient3dfast( permutarray[ 0 ], permutarray[ 1 ], permutarray[ 2 ],
						permutarray[ i ] );
	while(( fabs( ori ) / bboxsize3 ) < ( b->epsilon * b->epsilon * b->epsilon ))
	{
		i++;
		if(i == in->numberofpoints)
		{
			myprintf( "Exception:  All vertices are coplanar (Tol = %g).\n",
					b->epsilon );
			terminatetetgen( this, 10 );
		}
		ori = orient3dfast( permutarray[ 0 ], permutarray[ 1 ], permutarray[ 2 ],
							permutarray[ i ] );
	}
	if(i > 3)
	{
		// Swap to move the non-identical vertex from index i to index 1.
		swapvertex = permutarray[ i ];
		permutarray[ i ] = permutarray[ 3 ];
		permutarray[ 3 ] = swapvertex;
	}

	// Orient the first four vertices in permutarray so that they follow the
	//   right-hand rule.
	if(ori > 0.0)
	{
		// Swap the first two vertices.
		swapvertex = permutarray[ 0 ];
		permutarray[ 0 ] = permutarray[ 1 ];
		permutarray[ 1 ] = swapvertex;
	}

	// Create the initial Delaunay tetrahedralization.
	initialdelaunay( permutarray[ 0 ], permutarray[ 1 ], permutarray[ 2 ],
					 permutarray[ 3 ] );

	if(b->verbose)
	{
		myprintf( "  Incrementally inserting vertices.\n" );
	}
	insertvertexflags ivf;
	flipconstraints fc;

	// Choose algorithm: Bowyer-Watson (default) or Incremental Flip
	if(b->incrflip)
	{
		ivf.bowywat = 0;
		ivf.lawson = 1;
		fc.enqflag = 1;
	}
	else
	{
		ivf.bowywat = 1;
		ivf.lawson = 0;
	}


	for(i = 4; i < in->numberofpoints; i++)
	{
		if(pointtype( permutarray[ i ] ) == UNUSEDVERTEX)
		{
			setpointtype( permutarray[ i ], VOLVERTEX );
		}
		if(b->brio_hilbert || b->no_sort)
		{ // -b or -b/1
			// Start the last updated tet.
			searchtet.tet = recenttet.tet;
		}
		else
		{ // -b0
			// Randomly choose the starting tet for point location.
			searchtet.tet = NULL;
		}
		ivf.iloc = (int)OUTSIDE;
		// Insert the vertex.
		if(insertpoint( permutarray[ i ], &searchtet, NULL, NULL, &ivf ))
		{
			if(flipstack != NULL)
			{
				// Perform flip to recover Delaunayness.
				incrementalflip( permutarray[ i ], ( ivf.iloc == (int)OUTSIDE ), &fc );
			}
		}
		else
		{
			if(ivf.iloc == (int)ONVERTEX)
			{
				// The point already exists. Mark it and do nothing on it.
				swapvertex = org( searchtet );
				if(b->object != tetgenbehavior::STL)
				{
					if(!b->quiet)
					{
						myprintf( "Warning:  Point #%d is coincident with #%d. Ignored!\n",
								pointmark( permutarray[ i ] ), pointmark( swapvertex ) );
					}
				}
				setpoint2ppt( permutarray[ i ], swapvertex );
				setpointtype( permutarray[ i ], DUPLICATEDVERTEX );
				dupverts++;
			}
			else if(ivf.iloc == (int)NEARVERTEX)
			{
				swapvertex = org( searchtet );
				if(!b->quiet)
				{
					myprintf( "Warning:  Point %d is replaced by point %d.\n",
							pointmark( permutarray[ i ] ), pointmark( swapvertex ) );
					myprintf( "  Avoid creating a very short edge (len = %g) (< %g).\n",
							permutarray[ i ][ 3 ], minedgelength );
					myprintf( "  You may try a smaller tolerance (-T) (current is %g)\n",
							b->epsilon );
					myprintf( "  or use the option -M0/1 to avoid such replacement.\n" );
				}
				// Remember it is a duplicated point.
				setpoint2ppt( permutarray[ i ], swapvertex );
				setpointtype( permutarray[ i ], DUPLICATEDVERTEX );
				dupverts++;
			}
			else if(ivf.iloc == (int)NONREGULAR)
			{
				// The point is non-regular. Skipped.
				if(b->verbose)
				{
					myprintf( "  Point #%d is non-regular, skipped.\n",
							pointmark( permutarray[ i ] ) );
				}
				setpointtype( permutarray[ i ], NREGULARVERTEX );
				nonregularcount++;
			}
		}
	}



	delete[ ] permutarray;
}

////                                                                       ////
////                                                                       ////
//// delaunay_cxx /////////////////////////////////////////////////////////////

//// surface_cxx //////////////////////////////////////////////////////////////
////                                                                       ////
////                                                                       ////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// flipshpush()    Push a facet edge into flip stack.                        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::flipshpush( face* flipedge )
{
	badface *newflipface;

	newflipface = (badface *)flippool->alloc( );
	newflipface->ss = *flipedge;
	newflipface->forg = sorg( *flipedge );
	newflipface->fdest = sdest( *flipedge );
	newflipface->nextitem = flipstack;
	flipstack = newflipface;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// flip22()    Perform a 2-to-2 flip in surface mesh.                        //
//                                                                           //
// 'flipfaces' is an array of two subfaces. On input, they are [a,b,c] and   //
// [b,a,d]. On output, they are [c,d,b] and [d,c,a]. As a result, edge [a,b] //
// is replaced by edge [c,d].                                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::flip22( face* flipfaces, int flipflag, int chkencflag )
{
	face bdedges[ 4 ], outfaces[ 4 ], infaces[ 4 ];
	face bdsegs[ 4 ];
	face checkface;
	point pa, pb, pc, pd;
	int i;

	pa = sorg( flipfaces[ 0 ] );
	pb = sdest( flipfaces[ 0 ] );
	pc = sapex( flipfaces[ 0 ] );
	pd = sapex( flipfaces[ 1 ] );

	if(sorg( flipfaces[ 1 ] ) != pb)
	{
		sesymself( flipfaces[ 1 ] );
	}

	flip22count++;

	// Collect the four boundary edges.
	senext( flipfaces[ 0 ], bdedges[ 0 ] );
	senext2( flipfaces[ 0 ], bdedges[ 1 ] );
	senext( flipfaces[ 1 ], bdedges[ 2 ] );
	senext2( flipfaces[ 1 ], bdedges[ 3 ] );

	// Collect outer boundary faces.
	for(i = 0; i < 4; i++)
	{
		spivot( bdedges[ i ], outfaces[ i ] );
		infaces[ i ] = outfaces[ i ];
		sspivot( bdedges[ i ], bdsegs[ i ] );
		if(outfaces[ i ].sh != NULL)
		{
			if(isshsubseg( bdedges[ i ] ))
			{
				spivot( infaces[ i ], checkface );
				while(checkface.sh != bdedges[ i ].sh)
				{
					infaces[ i ] = checkface;
					spivot( infaces[ i ], checkface );
				}
			}
		}
	}

	// The flags set in these two subfaces do not change.
	// Shellmark does not change.
	// area constraint does not change.

	// Transform [a,b,c] -> [c,d,b].
	setshvertices( flipfaces[ 0 ], pc, pd, pb );
	// Transform [b,a,d] -> [d,c,a].
	setshvertices( flipfaces[ 1 ], pd, pc, pa );

	// Update the point-to-subface map.
	if(pointtype( pa ) == FREEFACETVERTEX)
	{
		setpoint2sh( pa, sencode( flipfaces[ 1 ] ) );
	}
	if(pointtype( pb ) == FREEFACETVERTEX)
	{
		setpoint2sh( pb, sencode( flipfaces[ 0 ] ) );
	}
	if(pointtype( pc ) == FREEFACETVERTEX)
	{
		setpoint2sh( pc, sencode( flipfaces[ 0 ] ) );
	}
	if(pointtype( pd ) == FREEFACETVERTEX)
	{
		setpoint2sh( pd, sencode( flipfaces[ 0 ] ) );
	}

	// Reconnect boundary edges to outer boundary faces.
	for(i = 0; i < 4; i++)
	{
		if(outfaces[ ( 3 + i ) % 4 ].sh != NULL)
		{
			// Make sure that the subface has the ori as the segment.
			if(bdsegs[ ( 3 + i ) % 4 ].sh != NULL)
			{
				bdsegs[ ( 3 + i ) % 4 ].shver = 0;
				if(sorg( bdedges[ i ] ) != sorg( bdsegs[ ( 3 + i ) % 4 ] ))
				{
					sesymself( bdedges[ i ] );
				}
			}
			sbond1( bdedges[ i ], outfaces[ ( 3 + i ) % 4 ] );
			sbond1( infaces[ ( 3 + i ) % 4 ], bdedges[ i ] );
		}
		else
		{
			sdissolve( bdedges[ i ] );
		}
		if(bdsegs[ ( 3 + i ) % 4 ].sh != NULL)
		{
			ssbond( bdedges[ i ], bdsegs[ ( 3 + i ) % 4 ] );
			if(chkencflag & 1)
			{
				// Queue this segment for encroaching check.
				enqueuesubface( badsubsegs, &( bdsegs[ ( 3 + i ) % 4 ] ) );
			}
		}
		else
		{
			ssdissolve( bdedges[ i ] );
		}
	}

	if(chkencflag & 2)
	{
		// Queue the flipped subfaces for quality/encroaching checks.
		for(i = 0; i < 2; i++)
		{
			enqueuesubface( badsubfacs, &( flipfaces[ i ] ) );
		}
	}

	recentsh = flipfaces[ 0 ];

	if(flipflag)
	{
		// Put the boundary edges into flip stack.
		for(i = 0; i < 4; i++)
		{
			flipshpush( &( bdedges[ i ] ) );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// flip31()    Remove a vertex by transforming 3-to-1 subfaces.              //
//                                                                           //
// 'flipfaces' is an array of subfaces. Its length is at least 4.  On input, //
// the first three faces are: [p,a,b], [p,b,c], and [p,c,a]. This routine    //
// replaces them by one face [a,b,c], it is returned in flipfaces[3].        //
//                                                                           //
// NOTE: The three old subfaces are not deleted within this routine.  They   //
// still hold pointers to their adjacent subfaces. These informations are    //
// needed by the routine 'sremovevertex()' for recovering a segment.         //
// The caller of this routine must delete the old subfaces after their uses. //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::flip31( face* flipfaces, int flipflag )
{
	face bdedges[ 3 ], outfaces[ 3 ], infaces[ 3 ];
	face bdsegs[ 3 ];
	face checkface;
	point pa, pb, pc;
	int i;

	pa = sdest( flipfaces[ 0 ] );
	pb = sdest( flipfaces[ 1 ] );
	pc = sdest( flipfaces[ 2 ] );

	flip31count++;

	// Collect all infos at the three boundary edges.
	for(i = 0; i < 3; i++)
	{
		senext( flipfaces[ i ], bdedges[ i ] );
		spivot( bdedges[ i ], outfaces[ i ] );
		infaces[ i ] = outfaces[ i ];
		sspivot( bdedges[ i ], bdsegs[ i ] );
		if(outfaces[ i ].sh != NULL)
		{
			if(isshsubseg( bdedges[ i ] ))
			{
				spivot( infaces[ i ], checkface );
				while(checkface.sh != bdedges[ i ].sh)
				{
					infaces[ i ] = checkface;
					spivot( infaces[ i ], checkface );
				}
			}
		}
	} // i

	// Create a new subface.
	makeshellface( subfaces, &( flipfaces[ 3 ] ) );
	setshvertices( flipfaces[ 3 ], pa, pb, pc );
	setshellmark( flipfaces[ 3 ], shellmark( flipfaces[ 0 ] ) );
	if(checkconstraints)
	{
		//area = areabound(flipfaces[0]);
		setareabound( flipfaces[ 3 ], areabound( flipfaces[ 0 ] ) );
	}
	if(useinsertradius)
	{
		setfacetindex( flipfaces[ 3 ], getfacetindex( flipfaces[ 0 ] ) );
	}

	// Update the point-to-subface map.
	if(pointtype( pa ) == FREEFACETVERTEX)
	{
		setpoint2sh( pa, sencode( flipfaces[ 3 ] ) );
	}
	if(pointtype( pb ) == FREEFACETVERTEX)
	{
		setpoint2sh( pb, sencode( flipfaces[ 3 ] ) );
	}
	if(pointtype( pc ) == FREEFACETVERTEX)
	{
		setpoint2sh( pc, sencode( flipfaces[ 3 ] ) );
	}

	// Update the three new boundary edges.
	bdedges[ 0 ] = flipfaces[ 3 ];         // [a,b]
	senext( flipfaces[ 3 ], bdedges[ 1 ] );  // [b,c]
	senext2( flipfaces[ 3 ], bdedges[ 2 ] ); // [c,a]

	// Reconnect boundary edges to outer boundary faces.
	for(i = 0; i < 3; i++)
	{
		if(outfaces[ i ].sh != NULL)
		{
			// Make sure that the subface has the ori as the segment.
			if(bdsegs[ i ].sh != NULL)
			{
				bdsegs[ i ].shver = 0;
				if(sorg( bdedges[ i ] ) != sorg( bdsegs[ i ] ))
				{
					sesymself( bdedges[ i ] );
				}
			}
			sbond1( bdedges[ i ], outfaces[ i ] );
			sbond1( infaces[ i ], bdedges[ i ] );
		}
		if(bdsegs[ i ].sh != NULL)
		{
			ssbond( bdedges[ i ], bdsegs[ i ] );
		}
	}

	recentsh = flipfaces[ 3 ];

	if(flipflag)
	{
		// Put the boundary edges into flip stack.
		for(i = 0; i < 3; i++)
		{
			flipshpush( &( bdedges[ i ] ) );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// lawsonflip()    Flip non-locally Delaunay edges.                          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

long tetgenmesh::lawsonflip( )
{
	badface *popface;
	face flipfaces[ 2 ];
	point pa, pb, pc, pd;
	REAL sign;
	long flipcount = 0;

	if(b->verbose > 2)
	{
		myprintf( "      Lawson flip %ld edges.\n", flippool->items );
	}

	while(flipstack != (badface *)NULL)
	{

		// Pop an edge from the stack.
		popface = flipstack;
		flipfaces[ 0 ] = popface->ss;
		pa = popface->forg;
		pb = popface->fdest;
		flipstack = popface->nextitem; // The next top item in stack.
		flippool->dealloc( (void *)popface );

		// Skip it if it is dead.
		if(flipfaces[ 0 ].sh[ 3 ] == NULL) continue;
		// Skip it if it is not the same edge as we saved.
		if(( sorg( flipfaces[ 0 ] ) != pa ) || ( sdest( flipfaces[ 0 ] ) != pb )) continue;
		// Skip it if it is a subsegment.
		if(isshsubseg( flipfaces[ 0 ] )) continue;

		// Get the adjacent face.
		spivot( flipfaces[ 0 ], flipfaces[ 1 ] );
		if(flipfaces[ 1 ].sh == NULL) continue; // Skip a hull edge.
		pc = sapex( flipfaces[ 0 ] );
		pd = sapex( flipfaces[ 1 ] );

		sign = incircle3d( pa, pb, pc, pd );

		if(sign < 0)
		{
			// It is non-locally Delaunay. Flip it.
			flip22( flipfaces, 1, 0 );
			flipcount++;
		}
	}

	if(b->verbose > 2)
	{
		myprintf( "      Performed %ld flips.\n", flipcount );
	}

	return flipcount;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// sinsertvertex()    Insert a vertex into a triangulation of a facet.       //
//                                                                           //
// This function uses three global arrays: 'caveshlist', 'caveshbdlist', and //
// 'caveshseglist'. On return, 'caveshlist' contains old subfaces in C(p),   //
// 'caveshbdlist' contains new subfaces in C(p). If the new point lies on a  //
// segment, 'cavesegshlist' returns the two new subsegments.                 //
//                                                                           //
// 'iloc' suggests the location of the point. If it is OUTSIDE, this routine //
// will first locate the point. It starts searching from 'searchsh' or 'rec- //
// entsh' if 'searchsh' is NULL.                                             //
//                                                                           //
// If 'bowywat' is set (1), the Bowyer-Watson algorithm is used to insert    //
// the vertex. Otherwise, only insert the vertex in the initial cavity.      // 
//                                                                           //
// If 'iloc' is 'INSTAR', this means the cavity of this vertex was already   //
// provided in the list 'caveshlist'.                                        //
//                                                                           //
// If 'splitseg' is not NULL, the new vertex lies on the segment and it will //
// be split. 'iloc' must be either 'ONEDGE' or 'INSTAR'.                     //
//                                                                           //
// 'rflag' (rounding) is a parameter passed to slocate() function.  If it is //
// set, after the location of the point is found, either ONEDGE or ONFACE,   //
// round the result using an epsilon.                                        //
//                                                                           //
// NOTE: the old subfaces in C(p) are not deleted. They're needed in case we //
// want to remove the new point immediately.                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::sinsertvertex( point insertpt, face *searchsh, face *splitseg,
							   int iloc, int bowywat, int rflag )
{
	face cavesh, neighsh, *parysh;
	face newsh, casout, casin;
	face checkseg;
	point pa, pb;
	enum locateresult loc = OUTSIDE;
	REAL sign, ori;
	int i, j;

	if(b->verbose > 2)
	{
		myprintf( "      Insert facet point %d.\n", pointmark( insertpt ) );
	}

	if(bowywat == 3)
	{
		loc = INSTAR;
	}

	if(( splitseg != NULL ) && ( splitseg->sh != NULL ))
	{
		// A segment is going to be split, no point location.
		spivot( *splitseg, *searchsh );
		if(loc != INSTAR) loc = ONEDGE;
	}
	else
	{
		if(loc != INSTAR) loc = ( enum locateresult ) iloc;
		if(loc == OUTSIDE)
		{
			// Do point location in surface mesh.
			if(searchsh->sh == NULL)
			{
				*searchsh = recentsh;
			}
			// Search the vertex. An above point must be provided ('aflag' = 1).
			loc = slocate( insertpt, searchsh, 1, 1, rflag );
		}
	}


	// Form the initial sC(p).
	if(loc == ONFACE)
	{
		// Add the face into list (in B-W cavity).
		smarktest( *searchsh );
		caveshlist->newindex( (void **)&parysh );
		*parysh = *searchsh;
	}
	else if(loc == ONEDGE)
	{
		if(( splitseg != NULL ) && ( splitseg->sh != NULL ))
		{
			splitseg->shver = 0;
			pa = sorg( *splitseg );
		}
		else
		{
			pa = sorg( *searchsh );
		}
		if(searchsh->sh != NULL)
		{
			// Collect all subfaces share at this edge.
			neighsh = *searchsh;
			while(1)
			{
				// Adjust the origin of its edge to be 'pa'.
				if(sorg( neighsh ) != pa) sesymself( neighsh );
				// Add this face into list (in B-W cavity).
				smarktest( neighsh );
				caveshlist->newindex( (void **)&parysh );
				*parysh = neighsh;
				// Add this face into face-at-splitedge list.
				cavesegshlist->newindex( (void **)&parysh );
				*parysh = neighsh;
				// Go to the next face at the edge.
				spivotself( neighsh );
				// Stop if all faces at the edge have been visited.
				if(neighsh.sh == searchsh->sh) break;
				if(neighsh.sh == NULL) break;
			}
		} // If (not a non-dangling segment).
	}
	else if(loc == ONVERTEX)
	{
		return (int)loc;
	}
	else if(loc == OUTSIDE)
	{
		// Comment: This should only happen during the surface meshing step.
		// Enlarge the convex hull of the triangulation by including p.
		// An above point of the facet is set in 'dummypoint' to replace
		// orient2d tests by orient3d tests.
		// Imagine that the current edge a->b (in 'searchsh') is horizontal in a
		//   plane, and a->b is directed from left to right, p lies above a->b.  
		//   Find the right-most edge of the triangulation which is visible by p.
		neighsh = *searchsh;
		while(1)
		{
			senext2self( neighsh );
			spivot( neighsh, casout );
			if(casout.sh == NULL)
			{
				// A convex hull edge. Is it visible by p.
				ori = orient3d( sorg( neighsh ), sdest( neighsh ), dummypoint, insertpt );
				if(ori < 0)
				{
					*searchsh = neighsh; // Visible, update 'searchsh'.
				}
				else
				{
					break; // 'searchsh' is the right-most visible edge.
				}
			}
			else
			{
				if(sorg( casout ) != sdest( neighsh )) sesymself( casout );
				neighsh = casout;
			}
		}
		// Create new triangles for all visible edges of p (from right to left).
		casin.sh = NULL;  // No adjacent face at right.
		pa = sorg( *searchsh );
		pb = sdest( *searchsh );
		while(1)
		{
			// Create a new subface on top of the (visible) edge.
			makeshellface( subfaces, &newsh );
			setshvertices( newsh, pb, pa, insertpt );
			setshellmark( newsh, shellmark( *searchsh ) );
			if(checkconstraints)
			{
				//area = areabound(*searchsh);
				setareabound( newsh, areabound( *searchsh ) );
			}
			if(useinsertradius)
			{
				setfacetindex( newsh, getfacetindex( *searchsh ) );
			}
			// Connect the new subface to the bottom subfaces.
			sbond1( newsh, *searchsh );
			sbond1( *searchsh, newsh );
			// Connect the new subface to its right-adjacent subface.
			if(casin.sh != NULL)
			{
				senext( newsh, casout );
				sbond1( casout, casin );
				sbond1( casin, casout );
			}
			// The left-adjacent subface has not been created yet.
			senext2( newsh, casin );
			// Add the new face into list (inside the B-W cavity).
			smarktest( newsh );
			caveshlist->newindex( (void **)&parysh );
			*parysh = newsh;
			// Move to the convex hull edge at the left of 'searchsh'.
			neighsh = *searchsh;
			while(1)
			{
				senextself( neighsh );
				spivot( neighsh, casout );
				if(casout.sh == NULL)
				{
					*searchsh = neighsh;
					break;
				}
				if(sorg( casout ) != sdest( neighsh )) sesymself( casout );
				neighsh = casout;
			}
			// A convex hull edge. Is it visible by p.
			pa = sorg( *searchsh );
			pb = sdest( *searchsh );
			ori = orient3d( pa, pb, dummypoint, insertpt );
			// Finish the process if p is not visible by the hull edge.
			if(ori >= 0) break;
		}
	}
	else if(loc == INSTAR)
	{
		// Under this case, the sub-cavity sC(p) has already been formed in
		//   insertvertex().
	}

	// Form the Bowyer-Watson cavity sC(p).
	for(i = 0; i < caveshlist->objects; i++)
	{
		cavesh = *(face *)fastlookup( caveshlist, i );
		for(j = 0; j < 3; j++)
		{
			if(!isshsubseg( cavesh ))
			{
				spivot( cavesh, neighsh );
				if(neighsh.sh != NULL)
				{
					// The adjacent face exists.
					if(!smarktested( neighsh ))
					{
						if(bowywat)
						{
							if(loc == INSTAR)
							{ // if (bowywat > 2) {
								// It must be a boundary edge.
								sign = 1;
							}
							else
							{
								// Check if this subface is connected to adjacent tet(s).
								if(!isshtet( neighsh ))
								{
									// Check if the subface is non-Delaunay wrt. the new pt.
									sign = incircle3d( sorg( neighsh ), sdest( neighsh ),
													   sapex( neighsh ), insertpt );
								}
								else
								{
									// It is connected to an adjacent tet. A boundary edge.
									sign = 1;
								}
							}
							if(sign < 0)
							{
								// Add the adjacent face in list (in B-W cavity).
								smarktest( neighsh );
								caveshlist->newindex( (void **)&parysh );
								*parysh = neighsh;
							}
						}
						else
						{
							sign = 1; // A boundary edge.
						}
					}
					else
					{
						sign = -1; // Not a boundary edge.
					}
				}
				else
				{
					// No adjacent face. It is a hull edge.
					if(loc == OUTSIDE)
					{
						// It is a boundary edge if it does not contain p.
						if(( sorg( cavesh ) == insertpt ) || ( sdest( cavesh ) == insertpt ))
						{
							sign = -1; // Not a boundary edge.
						}
						else
						{
							sign = 1; // A boundary edge.
						}
					}
					else
					{
						sign = 1; // A boundary edge.
					}
				}
			}
			else
			{
				// Do not across a segment. It is a boundary edge.
				sign = 1;
			}
			if(sign >= 0)
			{
				// Add a boundary edge.
				caveshbdlist->newindex( (void **)&parysh );
				*parysh = cavesh;
			}
			senextself( cavesh );
		} // j
	} // i


	// Creating new subfaces.
	for(i = 0; i < caveshbdlist->objects; i++)
	{
		parysh = (face *)fastlookup( caveshbdlist, i );
		sspivot( *parysh, checkseg );
		if(( parysh->shver & 01 ) != 0) sesymself( *parysh );
		pa = sorg( *parysh );
		pb = sdest( *parysh );
		// Create a new subface.
		makeshellface( subfaces, &newsh );
		setshvertices( newsh, pa, pb, insertpt );
		setshellmark( newsh, shellmark( *parysh ) );
		if(checkconstraints)
		{
			//area = areabound(*parysh);
			setareabound( newsh, areabound( *parysh ) );
		}
		if(useinsertradius)
		{
			setfacetindex( newsh, getfacetindex( *parysh ) );
		}
		// Update the point-to-subface map.
		if(pointtype( pa ) == FREEFACETVERTEX)
		{
			setpoint2sh( pa, sencode( newsh ) );
		}
		if(pointtype( pb ) == FREEFACETVERTEX)
		{
			setpoint2sh( pb, sencode( newsh ) );
		}
		// Connect newsh to outer subfaces.
		spivot( *parysh, casout );
		if(casout.sh != NULL)
		{
			casin = casout;
			if(checkseg.sh != NULL)
			{
				// Make sure that newsh has the right ori at this segment.
				checkseg.shver = 0;
				if(sorg( newsh ) != sorg( checkseg ))
				{
					sesymself( newsh );
					sesymself( *parysh ); // This side should also be inverse.
				}
				spivot( casin, neighsh );
				while(neighsh.sh != parysh->sh)
				{
					casin = neighsh;
					spivot( casin, neighsh );
				}
			}
			sbond1( newsh, casout );
			sbond1( casin, newsh );
		}
		if(checkseg.sh != NULL)
		{
			ssbond( newsh, checkseg );
		}
		// Connect oldsh <== newsh (for connecting adjacent new subfaces).
		//   *parysh and newsh point to the same edge and the same ori.
		sbond1( *parysh, newsh );
	}

	if(newsh.sh != NULL)
	{
		// Set a handle for searching.
		recentsh = newsh;
	}

	// Update the point-to-subface map.
	if(pointtype( insertpt ) == FREEFACETVERTEX)
	{
		setpoint2sh( insertpt, sencode( newsh ) );
	}

	// Connect adjacent new subfaces together.
	for(i = 0; i < caveshbdlist->objects; i++)
	{
		// Get an old subface at edge [a, b].
		parysh = (face *)fastlookup( caveshbdlist, i );
		spivot( *parysh, newsh ); // The new subface [a, b, p].
		senextself( newsh ); // At edge [b, p].
		spivot( newsh, neighsh );
		if(neighsh.sh == NULL)
		{
			// Find the adjacent new subface at edge [b, p].
			pb = sdest( *parysh );
			neighsh = *parysh;
			while(1)
			{
				senextself( neighsh );
				spivotself( neighsh );
				if(neighsh.sh == NULL) break;
				if(!smarktested( neighsh )) break;
				if(sdest( neighsh ) != pb) sesymself( neighsh );
			}
			if(neighsh.sh != NULL)
			{
				// Now 'neighsh' is a new subface at edge [b, #].
				if(sorg( neighsh ) != pb) sesymself( neighsh );
				senext2self( neighsh ); // Go to the open edge [p, b].
				sbond( newsh, neighsh );
			}
		}
		spivot( *parysh, newsh ); // The new subface [a, b, p].
		senext2self( newsh ); // At edge [p, a].
		spivot( newsh, neighsh );
		if(neighsh.sh == NULL)
		{
			// Find the adjacent new subface at edge [p, a].
			pa = sorg( *parysh );
			neighsh = *parysh;
			while(1)
			{
				senext2self( neighsh );
				spivotself( neighsh );
				if(neighsh.sh == NULL) break;
				if(!smarktested( neighsh )) break;
				if(sorg( neighsh ) != pa) sesymself( neighsh );
			}
			if(neighsh.sh != NULL)
			{
				// Now 'neighsh' is a new subface at edge [#, a].
				if(sdest( neighsh ) != pa) sesymself( neighsh );
				senextself( neighsh ); // Go to the open edge [a, p].
				sbond( newsh, neighsh );
			}
		}
	}

	if(( loc == ONEDGE ) || ( ( splitseg != NULL ) && ( splitseg->sh != NULL ) )
		|| ( cavesegshlist->objects > 0l ))
	{
		// An edge is being split. We distinguish two cases:
		//   (1) the edge is not on the boundary of the cavity;
		//   (2) the edge is on the boundary of the cavity.
		// In case (2), the edge is either a segment or a hull edge. There are
		//   degenerated new faces in the cavity. They must be removed.
		face aseg, bseg, aoutseg, boutseg;

		for(i = 0; i < cavesegshlist->objects; i++)
		{
			// Get the saved old subface.
			parysh = (face *)fastlookup( cavesegshlist, i );
			// Get a possible new degenerated subface.
			spivot( *parysh, cavesh );
			if(sapex( cavesh ) == insertpt)
			{
				// Found a degenerated new subface, i.e., case (2).
				if(cavesegshlist->objects > 1)
				{
					// There are more than one subface share at this edge.
					j = ( i + 1 ) % (int)cavesegshlist->objects;
					parysh = (face *)fastlookup( cavesegshlist, j );
					spivot( *parysh, neighsh );
					// Adjust cavesh and neighsh both at edge a->b, and has p as apex.
					if(sorg( neighsh ) != sorg( cavesh ))
					{
						sesymself( neighsh );
					}
					// Connect adjacent faces at two other edges of cavesh and neighsh.
					//   As a result, the two degenerated new faces are squeezed from the
					//   new triangulation of the cavity. Note that the squeezed faces
					//   still hold the adjacent informations which will be used in 
					//   re-connecting subsegments (if they exist). 
					for(j = 0; j < 2; j++)
					{
						senextself( cavesh );
						senextself( neighsh );
						spivot( cavesh, newsh );
						spivot( neighsh, casout );
						sbond1( newsh, casout ); // newsh <- casout.
					}
				}
				else
				{
					// There is only one subface containing this edge [a,b]. Squeeze the
					//   degenerated new face [a,b,c] by disconnecting it from its two 
					//   adjacent subfaces at edges [b,c] and [c,a]. Note that the face
					//   [a,b,c] still hold the connection to them.
					for(j = 0; j < 2; j++)
					{
						senextself( cavesh );
						spivot( cavesh, newsh );
						sdissolve( newsh );
					}
				}
				//recentsh = newsh;
				// Update the point-to-subface map.
				if(pointtype( insertpt ) == FREEFACETVERTEX)
				{
					setpoint2sh( insertpt, sencode( newsh ) );
				}
			}
		}

		if(( splitseg != NULL ) && ( splitseg->sh != NULL ))
		{
			if(loc != INSTAR)
			{ // if (bowywat < 3) {
				smarktest( *splitseg ); // Mark it as being processed.
			}

			aseg = *splitseg;
			pa = sorg( *splitseg );
			pb = sdest( *splitseg );

			// Insert the new point p.
			makeshellface( subsegs, &aseg );
			makeshellface( subsegs, &bseg );

			setshvertices( aseg, pa, insertpt, NULL );
			setshvertices( bseg, insertpt, pb, NULL );
			setshellmark( aseg, shellmark( *splitseg ) );
			setshellmark( bseg, shellmark( *splitseg ) );
			if(checkconstraints)
			{
				setareabound( aseg, areabound( *splitseg ) );
				setareabound( bseg, areabound( *splitseg ) );
			}
			if(useinsertradius)
			{
				setfacetindex( aseg, getfacetindex( *splitseg ) );
				setfacetindex( bseg, getfacetindex( *splitseg ) );
			}

			// Connect [#, a]<->[a, p].
			senext2( *splitseg, boutseg ); // Temporarily use boutseg.
			spivotself( boutseg );
			if(boutseg.sh != NULL)
			{
				senext2( aseg, aoutseg );
				sbond( boutseg, aoutseg );
			}
			// Connect [p, b]<->[b, #].
			senext( *splitseg, aoutseg );
			spivotself( aoutseg );
			if(aoutseg.sh != NULL)
			{
				senext( bseg, boutseg );
				sbond( boutseg, aoutseg );
			}
			// Connect [a, p] <-> [p, b].
			senext( aseg, aoutseg );
			senext2( bseg, boutseg );
			sbond( aoutseg, boutseg );

			// Connect subsegs [a, p] and [p, b] to adjacent new subfaces.
			// Although the degenerated new faces have been squeezed. They still
			//   hold the connections to the actual new faces. 
			for(i = 0; i < cavesegshlist->objects; i++)
			{
				parysh = (face *)fastlookup( cavesegshlist, i );
				spivot( *parysh, neighsh );
				// neighsh is a degenerated new face.
				if(sorg( neighsh ) != pa)
				{
					sesymself( neighsh );
				}
				senext2( neighsh, newsh );
				spivotself( newsh ); // The edge [p, a] in newsh
				ssbond( newsh, aseg );
				senext( neighsh, newsh );
				spivotself( newsh ); // The edge [b, p] in newsh
				ssbond( newsh, bseg );
			}


			// Let the point remember the segment it lies on.
			if(pointtype( insertpt ) == FREESEGVERTEX)
			{
				setpoint2sh( insertpt, sencode( aseg ) );
			}
			// Update the point-to-seg map.
			if(pointtype( pa ) == FREESEGVERTEX)
			{
				setpoint2sh( pa, sencode( aseg ) );
			}
			if(pointtype( pb ) == FREESEGVERTEX)
			{
				setpoint2sh( pb, sencode( bseg ) );
			}
		} // if ((splitseg != NULL) && (splitseg->sh != NULL)) 

		// Delete all degenerated new faces.
		for(i = 0; i < cavesegshlist->objects; i++)
		{
			parysh = (face *)fastlookup( cavesegshlist, i );
			spivotself( *parysh );
			if(sapex( *parysh ) == insertpt)
			{
				shellfacedealloc( subfaces, parysh->sh );
			}
		}
		cavesegshlist->restart( );

		if(( splitseg != NULL ) && ( splitseg->sh != NULL ))
		{
			// Return the two new subsegments (for further process).
			//   Re-use 'cavesegshlist'.
			cavesegshlist->newindex( (void **)&parysh );
			*parysh = aseg;
			cavesegshlist->newindex( (void **)&parysh );
			*parysh = bseg;
		}
	} // if (loc == ONEDGE)


	return (int)loc;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// sremovevertex()    Remove a vertex from the surface mesh.                 //
//                                                                           //
// 'delpt' (p) is the vertex to be removed. If 'parentseg' is not NULL, p is //
// a segment vertex, and the origin of 'parentseg' is p. Otherwise, p is a   //
// facet vertex, and the origin of 'parentsh' is p.                          //
//                                                                           //
// Within each facet, we first use a sequence of 2-to-2 flips to flip any    //
// edge at p, finally use a 3-to-1 flip to remove p.                         //
//                                                                           //
// All new created subfaces are returned in the global array 'caveshbdlist'. //
// The new segment (when p is on segment) is returned in 'parentseg'.        //
//                                                                           //
// If 'lawson' > 0, the Lawson flip algorithm is used to recover Delaunay-   //
// ness after p is removed.                                                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::sremovevertex( point delpt, face* parentsh, face* parentseg,
							   int lawson )
{
	face flipfaces[ 4 ], spinsh, *parysh;
	point pa, pb, pc, pd;
	REAL ori1, ori2;
	int it, i, j;

	if(parentseg != NULL)
	{
		// 'delpt' (p) should be a Steiner point inserted in a segment [a,b],
		//   where 'parentseg' should be [p,b]. Find the segment [a,p].
		face startsh, neighsh, nextsh;
		face abseg, prevseg, checkseg;
		face adjseg1, adjseg2;
		face fakesh;
		senext2( *parentseg, prevseg );
		spivotself( prevseg );
		prevseg.shver = 0;
		// Restore the original segment [a,b].
		pa = sorg( prevseg );
		pb = sdest( *parentseg );
		if(b->verbose > 2)
		{
			myprintf( "      Remove vertex %d from segment [%d, %d].\n",
					pointmark( delpt ), pointmark( pa ), pointmark( pb ) );
		}
		makeshellface( subsegs, &abseg );
		setshvertices( abseg, pa, pb, NULL );
		setshellmark( abseg, shellmark( *parentseg ) );
		if(checkconstraints)
		{
			setareabound( abseg, areabound( *parentseg ) );
		}
		if(useinsertradius)
		{
			setfacetindex( abseg, getfacetindex( *parentseg ) );
		}
		// Connect [#, a]<->[a, b].
		senext2( prevseg, adjseg1 );
		spivotself( adjseg1 );
		if(adjseg1.sh != NULL)
		{
			adjseg1.shver = 0;
			senextself( adjseg1 );
			senext2( abseg, adjseg2 );
			sbond( adjseg1, adjseg2 );
		}
		// Connect [a, b]<->[b, #].
		senext( *parentseg, adjseg1 );
		spivotself( adjseg1 );
		if(adjseg1.sh != NULL)
		{
			adjseg1.shver = 0;
			senext2self( adjseg1 );
			senext( abseg, adjseg2 );
			sbond( adjseg1, adjseg2 );
		}
		// Update the point-to-segment map.
		setpoint2sh( pa, sencode( abseg ) );
		setpoint2sh( pb, sencode( abseg ) );

		// Get the faces in face ring at segment [p, b].
		//   Re-use array 'caveshlist'.
		spivot( *parentseg, *parentsh );
		if(parentsh->sh != NULL)
		{
			spinsh = *parentsh;
			while(1)
			{
				// Save this face in list.
				caveshlist->newindex( (void **)&parysh );
				*parysh = spinsh;
				// Go to the next face in the ring.
				spivotself( spinsh );
				if(spinsh.sh == NULL)
				{
					break; // It is possible there is only one facet.
				}
				if(spinsh.sh == parentsh->sh) break;
			}
		}

		// Create the face ring of the new segment [a,b]. Each face in the ring
		//   is [a,b,p] (degenerated!). It will be removed (automatically).
		for(i = 0; i < caveshlist->objects; i++)
		{
			parysh = (face *)fastlookup( caveshlist, i );
			startsh = *parysh;
			if(sorg( startsh ) != delpt)
			{
				sesymself( startsh );
			}
			// startsh is [p, b, #1], find the subface [a, p, #2].
			neighsh = startsh;
			while(1)
			{
				senext2self( neighsh );
				sspivot( neighsh, checkseg );
				if(checkseg.sh != NULL)
				{
					// It must be the segment [a, p].
					break;
				}
				spivotself( neighsh );
				if(sorg( neighsh ) != delpt) sesymself( neighsh );
			}
			// Now neighsh is [a, p, #2].
			if(neighsh.sh != startsh.sh)
			{
				// Detach the two subsegments [a,p] and [p,b] from subfaces.
				ssdissolve( startsh );
				ssdissolve( neighsh );
				// Create a degenerated subface [a,b,p]. It is used to: (1) hold the
				//   new segment [a,b]; (2) connect to the two adjacent subfaces
				//   [p,b,#] and [a,p,#].
				makeshellface( subfaces, &fakesh );
				setshvertices( fakesh, pa, pb, delpt );
				setshellmark( fakesh, shellmark( startsh ) );
				// Connect fakesh to the segment [a,b].
				ssbond( fakesh, abseg );
				// Connect fakesh to adjacent subfaces: [p,b,#1] and [a,p,#2].
				senext( fakesh, nextsh );
				sbond( nextsh, startsh );
				senext2( fakesh, nextsh );
				sbond( nextsh, neighsh );
				smarktest( fakesh ); // Mark it as faked.
			}
			else
			{
				// Special case. There exists already a degenerated face [a,b,p]!
				//   There is no need to create a faked subface here.
				senext2self( neighsh ); // [a,b,p]
				// Since we will re-connect the face ring using the faked subfaces.
				//   We put the adjacent face of [a,b,p] to the list.
				spivot( neighsh, startsh ); // The original adjacent subface.
				if(sorg( startsh ) != pa) sesymself( startsh );
				sdissolve( startsh );
				// Connect fakesh to the segment [a,b].
				ssbond( startsh, abseg );
				fakesh = startsh; // Do not mark it!
				// Delete the degenerated subface.
				shellfacedealloc( subfaces, neighsh.sh );
			}
			// Save the fakesh in list (for re-creating the face ring).
			cavesegshlist->newindex( (void **)&parysh );
			*parysh = fakesh;
		} // i
		caveshlist->restart( );

		// Re-create the face ring.
		if(cavesegshlist->objects > 1)
		{
			for(i = 0; i < cavesegshlist->objects; i++)
			{
				parysh = (face *)fastlookup( cavesegshlist, i );
				fakesh = *parysh;
				// Get the next face in the ring.
				j = ( i + 1 ) % cavesegshlist->objects;
				parysh = (face *)fastlookup( cavesegshlist, j );
				nextsh = *parysh;
				sbond1( fakesh, nextsh );
			}
		}

		// Delete the two subsegments containing p.
		shellfacedealloc( subsegs, parentseg->sh );
		shellfacedealloc( subsegs, prevseg.sh );
		// Return the new segment.
		*parentseg = abseg;
	}
	else
	{
		// p is inside the surface.
		if(b->verbose > 2)
		{
			myprintf( "      Remove vertex %d from surface.\n", pointmark( delpt ) );
		}
		// Let 'delpt' be its apex.
		senextself( *parentsh );
		// For unifying the code, we add parentsh to list.
		cavesegshlist->newindex( (void **)&parysh );
		*parysh = *parentsh;
	}

	// Remove the point (p).

	for(it = 0; it < cavesegshlist->objects; it++)
	{
		parentsh = (face *)fastlookup( cavesegshlist, it ); // [a,b,p]
		senextself( *parentsh ); // [b,p,a].
		spivotself( *parentsh );
		if(sorg( *parentsh ) != delpt) sesymself( *parentsh );
		// now parentsh is [p,b,#].
		if(sorg( *parentsh ) != delpt)
		{
			// The vertex has already been removed in above special case.
			continue;
		}

		while(1)
		{
			// Initialize the flip edge list. Re-use 'caveshlist'.
			spinsh = *parentsh; // [p, b, #]
			while(1)
			{
				caveshlist->newindex( (void **)&parysh );
				*parysh = spinsh;
				senext2self( spinsh );
				spivotself( spinsh );
				if(spinsh.sh == parentsh->sh) break;
				if(sorg( spinsh ) != delpt) sesymself( spinsh );
			} // while (1)

			if(caveshlist->objects == 3)
			{
				// Delete the point by a 3-to-1 flip.
				for(i = 0; i < 3; i++)
				{
					parysh = (face *)fastlookup( caveshlist, i );
					flipfaces[ i ] = *parysh;
				}
				flip31( flipfaces, lawson );
				for(i = 0; i < 3; i++)
				{
					shellfacedealloc( subfaces, flipfaces[ i ].sh );
				}
				caveshlist->restart( );
				// Save the new subface.
				caveshbdlist->newindex( (void **)&parysh );
				*parysh = flipfaces[ 3 ];
				// The vertex is removed.
				break;
			}

			// Search an edge to flip.
			for(i = 0; i < caveshlist->objects; i++)
			{
				parysh = (face *)fastlookup( caveshlist, i );
				flipfaces[ 0 ] = *parysh;
				spivot( flipfaces[ 0 ], flipfaces[ 1 ] );
				if(sorg( flipfaces[ 0 ] ) != sdest( flipfaces[ 1 ] ))
					sesymself( flipfaces[ 1 ] );
				// Skip this edge if it belongs to a faked subface.
				if(!smarktested( flipfaces[ 0 ] ) && !smarktested( flipfaces[ 1 ] ))
				{
					pa = sorg( flipfaces[ 0 ] );
					pb = sdest( flipfaces[ 0 ] );
					pc = sapex( flipfaces[ 0 ] );
					pd = sapex( flipfaces[ 1 ] );
					calculateabovepoint4( pa, pb, pc, pd );
					// Check if a 2-to-2 flip is possible.
					ori1 = orient3d( pc, pd, dummypoint, pa );
					ori2 = orient3d( pc, pd, dummypoint, pb );
					if(ori1 * ori2 < 0)
					{
						// A 2-to-2 flip is found.
						flip22( flipfaces, lawson, 0 );
						// The i-th edge is flipped. The i-th and (i-1)-th subfaces are
						//   changed. The 'flipfaces[1]' contains p as its apex.
						senext2( flipfaces[ 1 ], *parentsh );
						// Save the new subface.
						caveshbdlist->newindex( (void **)&parysh );
						*parysh = flipfaces[ 0 ];
						break;
					}
				} //
			} // i

			if(i == caveshlist->objects)
			{
				// Do a flip22 and a flip31 to remove p.
				parysh = (face *)fastlookup( caveshlist, 0 );
				flipfaces[ 0 ] = *parysh;
				spivot( flipfaces[ 0 ], flipfaces[ 1 ] );
				if(sorg( flipfaces[ 0 ] ) != sdest( flipfaces[ 1 ] ))
				{
					sesymself( flipfaces[ 1 ] );
				}
				flip22( flipfaces, lawson, 0 );
				senext2( flipfaces[ 1 ], *parentsh );
				// Save the new subface.
				caveshbdlist->newindex( (void **)&parysh );
				*parysh = flipfaces[ 0 ];
			}

			// The edge list at p are changed.
			caveshlist->restart( );
		} // while (1)

	} // it

	cavesegshlist->restart( );

	if(b->verbose > 2)
	{
		myprintf( "      Created %ld new subfaces.\n", caveshbdlist->objects );
	}


	if(lawson)
	{
		lawsonflip( );
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// slocate()    Locate a point in a surface triangulation.                   //
//                                                                           //
// Staring the search from 'searchsh'(it should not be NULL). Perform a line //
// walk search for a subface containing the point (p).                       //
//                                                                           //
// If 'aflag' is set, the 'dummypoint' is pre-calculated so that it lies     //
// above the 'searchsh' in its current orientation. The test if c is CCW to  //
// the line a->b can be done by the test if c is below the oriented plane    //
// a->b->dummypoint.                                                         //
//                                                                           //
// If 'cflag' is not TRUE, the triangulation may not be convex.  Stop search //
// when a segment is met and return OUTSIDE.                                 //
//                                                                           //
// If 'rflag' (rounding) is set, after the location of the point is found,   //
// either ONEDGE or ONFACE, round the result using an epsilon.               //
//                                                                           //
// The returned value indicates the following cases:                         //
//   - ONVERTEX, p is the origin of 'searchsh'.                              //
//   - ONEDGE, p lies on the edge of 'searchsh'.                             //
//   - ONFACE, p lies in the interior of 'searchsh'.                         //
//   - OUTSIDE, p lies outside of the triangulation, p is on the left-hand   //
//     side of the edge 'searchsh'(s), i.e., org(s), dest(s), p are CW.      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

enum tetgenmesh::locateresult tetgenmesh::slocate( point searchpt,
	face* searchsh, int aflag, int cflag, int rflag )
{
	face neighsh;
	point pa, pb, pc;
	enum locateresult loc;
	enum { MOVE_BC, MOVE_CA } nextmove;
	REAL ori, ori_bc, ori_ca;
	int i;

	pa = sorg( *searchsh );
	pb = sdest( *searchsh );
	pc = sapex( *searchsh );

	if(!aflag)
	{
		// No above point is given. Calculate an above point for this facet.
		calculateabovepoint4( pa, pb, pc, searchpt );
	}

	// 'dummypoint' is given. Make sure it is above [a,b,c]
	ori = orient3d( pa, pb, pc, dummypoint );
	if(ori > 0)
	{
		sesymself( *searchsh ); // Reverse the face orientation.
	}
	else if(ori == 0.0)
	{
		// This case should not happen theoretically. But... 
		return UNKNOWN;
	}

	// Find an edge of the face s.t. p lies on its right-hand side (CCW).
	for(i = 0; i < 3; i++)
	{
		pa = sorg( *searchsh );
		pb = sdest( *searchsh );
		ori = orient3d( pa, pb, dummypoint, searchpt );
		if(ori > 0) break;
		senextself( *searchsh );
	}
	if(i == 3)
	{
		return UNKNOWN;
	}

	pc = sapex( *searchsh );

	if(pc == searchpt)
	{
		senext2self( *searchsh );
		return ONVERTEX;
	}

	while(1)
	{

		ori_bc = orient3d( pb, pc, dummypoint, searchpt );
		ori_ca = orient3d( pc, pa, dummypoint, searchpt );

		if(ori_bc < 0)
		{
			if(ori_ca < 0)
			{ // (--)
				// Any of the edges is a viable move.
				if(randomnation( 2 ))
				{
					nextmove = MOVE_CA;
				}
				else
				{
					nextmove = MOVE_BC;
				}
			}
			else
			{ // (-#)
				// Edge [b, c] is viable.
				nextmove = MOVE_BC;
			}
		}
		else
		{
			if(ori_ca < 0)
			{ // (#-)
				// Edge [c, a] is viable.
				nextmove = MOVE_CA;
			}
			else
			{
				if(ori_bc > 0)
				{
					if(ori_ca > 0)
					{ // (++)
						loc = ONFACE;  // Inside [a, b, c].
						break;
					}
					else
					{ // (+0)
						senext2self( *searchsh ); // On edge [c, a].
						loc = ONEDGE;
						break;
					}
				}
				else
				{ // ori_bc == 0
					if(ori_ca > 0)
					{ // (0+)
						senextself( *searchsh ); // On edge [b, c].
						loc = ONEDGE;
						break;
					}
					else
					{ // (00)
						// p is coincident with vertex c. 
						senext2self( *searchsh );
						return ONVERTEX;
					}
				}
			}
		}

		// Move to the next face.
		if(nextmove == MOVE_BC)
		{
			senextself( *searchsh );
		}
		else
		{
			senext2self( *searchsh );
		}
		if(!cflag)
		{
			// NON-convex case. Check if we will cross a boundary.
			if(isshsubseg( *searchsh ))
			{
				return ENCSEGMENT;
			}
		}
		spivot( *searchsh, neighsh );
		if(neighsh.sh == NULL)
		{
			return OUTSIDE; // A hull edge.
		}
		// Adjust the edge orientation.
		if(sorg( neighsh ) != sdest( *searchsh ))
		{
			sesymself( neighsh );
		}

		// Update the newly discovered face and its endpoints.
		*searchsh = neighsh;
		pa = sorg( *searchsh );
		pb = sdest( *searchsh );
		pc = sapex( *searchsh );

		if(pc == searchpt)
		{
			senext2self( *searchsh );
			return ONVERTEX;
		}

	} // while (1)

	// assert(loc == ONFACE || loc == ONEDGE);


	if(rflag)
	{
		// Round the locate result before return.
		REAL n[ 3 ], area_abc, area_abp, area_bcp, area_cap;

		pa = sorg( *searchsh );
		pb = sdest( *searchsh );
		pc = sapex( *searchsh );

		facenormal( pa, pb, pc, n, 1, NULL );
		area_abc = sqrt( dot( n, n ) );

		facenormal( pb, pc, searchpt, n, 1, NULL );
		area_bcp = sqrt( dot( n, n ) );
		if(( area_bcp / area_abc ) < b->epsilon)
		{
			area_bcp = 0; // Rounding.
		}

		facenormal( pc, pa, searchpt, n, 1, NULL );
		area_cap = sqrt( dot( n, n ) );
		if(( area_cap / area_abc ) < b->epsilon)
		{
			area_cap = 0; // Rounding
		}

		if(( loc == ONFACE ) || ( loc == OUTSIDE ))
		{
			facenormal( pa, pb, searchpt, n, 1, NULL );
			area_abp = sqrt( dot( n, n ) );
			if(( area_abp / area_abc ) < b->epsilon)
			{
				area_abp = 0; // Rounding
			}
		}
		else
		{ // loc == ONEDGE
			area_abp = 0;
		}

		if(area_abp == 0)
		{
			if(area_bcp == 0)
			{
				senextself( *searchsh );
				loc = ONVERTEX; // p is close to b.
			}
			else
			{
				if(area_cap == 0)
				{
					loc = ONVERTEX; // p is close to a.
				}
				else
				{
					loc = ONEDGE; // p is on edge [a,b].
				}
			}
		}
		else if(area_bcp == 0)
		{
			if(area_cap == 0)
			{
				senext2self( *searchsh );
				loc = ONVERTEX; // p is close to c.
			}
			else
			{
				senextself( *searchsh );
				loc = ONEDGE; // p is on edge [b,c].
			}
		}
		else if(area_cap == 0)
		{
			senext2self( *searchsh );
			loc = ONEDGE; // p is on edge [c,a].
		}
		else
		{
			loc = ONFACE; // p is on face [a,b,c].
		}
	} // if (rflag)

	return loc;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// sscoutsegment()    Look for a segment in the surface triangulation.       //
//                                                                           //
// The segment is given by the origin of 'searchsh' and 'endpt'.             //
//                                                                           //
// If an edge in T is found matching this segment, the segment is "locked"   //
// in T at the edge.  Otherwise, flip the first edge in T that the segment   //
// crosses. Continue the search from the flipped face.                       //
//                                                                           //
// This routine uses 'orisent3d' to determine the search direction. It uses  //
// 'dummypoint' as the 'lifted point' in 3d, and it assumes that it (dummy-  //
// point) lies above the 'searchsh' (w.r.t the Right-hand rule).             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

enum tetgenmesh::interresult tetgenmesh::sscoutsegment( face *searchsh,
	point endpt, int insertsegflag, int reporterrorflag, int chkencflag )
{
	face flipshs[ 2 ], neighsh;
	point startpt, pa, pb, pc, pd;
	enum interresult dir;
	enum { MOVE_AB, MOVE_CA } nextmove;
	REAL ori_ab, ori_ca, len;

	// The origin of 'searchsh' is fixed.
	startpt = sorg( *searchsh );
	nextmove = MOVE_AB; // Avoid compiler warning.

	if(b->verbose > 2)
	{
		myprintf( "      Scout segment (%d, %d).\n", pointmark( startpt ),
				pointmark( endpt ) );
	}
	len = distance( startpt, endpt );

	// Search an edge in 'searchsh' on the path of this segment.
	while(1)
	{

		pb = sdest( *searchsh );
		if(pb == endpt)
		{
			dir = SHAREEDGE; // Found!
			break;
		}

		pc = sapex( *searchsh );
		if(pc == endpt)
		{
			senext2self( *searchsh );
			sesymself( *searchsh );
			dir = SHAREEDGE; // Found!
			break;
		}


		// Round the results.
		if(( sqrt( triarea( startpt, pb, endpt ) ) / len ) < b->epsilon)
		{
			ori_ab = 0.0;
		}
		else
		{
			ori_ab = orient3d( startpt, pb, dummypoint, endpt );
		}
		if(( sqrt( triarea( pc, startpt, endpt ) ) / len ) < b->epsilon)
		{
			ori_ca = 0.0;
		}
		else
		{
			ori_ca = orient3d( pc, startpt, dummypoint, endpt );
		}

		if(ori_ab < 0)
		{
			if(ori_ca < 0)
			{ // (--)
				// Both sides are viable moves.
				if(randomnation( 2 ))
				{
					nextmove = MOVE_CA;
				}
				else
				{
					nextmove = MOVE_AB;
				}
			}
			else
			{ // (-#)
				nextmove = MOVE_AB;
			}
		}
		else
		{
			if(ori_ca < 0)
			{ // (#-)
				nextmove = MOVE_CA;
			}
			else
			{
				if(ori_ab > 0)
				{
					if(ori_ca > 0)
					{ // (++)
						// The segment intersects with edge [b, c].
						dir = ACROSSEDGE;
						break;
					}
					else
					{ // (+0)
						// The segment collinear with edge [c, a].
						senext2self( *searchsh );
						sesymself( *searchsh );
						dir = ACROSSVERT;
						break;
					}
				}
				else
				{
					if(ori_ca > 0)
					{ // (0+)
						// The segment is collinear with edge [a, b].
						dir = ACROSSVERT;
						break;
					}
					else
					{ // (00)
						// startpt == endpt. Not possible.
						terminatetetgen( this, 2 );
					}
				}
			}
		}

		// Move 'searchsh' to the next face, keep the origin unchanged.
		if(nextmove == MOVE_AB)
		{
			if(chkencflag)
			{
				// Do not cross boundary.
				if(isshsubseg( *searchsh ))
				{
					return ACROSSEDGE; // ACROSS_SEG
				}
			}
			spivot( *searchsh, neighsh );
			if(neighsh.sh != NULL)
			{
				if(sorg( neighsh ) != pb) sesymself( neighsh );
				senext( neighsh, *searchsh );
			}
			else
			{
				// This side (startpt->pb) is outside. It is caused by rounding error.
				// Try the next side, i.e., (pc->startpt).
				senext2( *searchsh, neighsh );
				if(chkencflag)
				{
					// Do not cross boundary.
					if(isshsubseg( neighsh ))
					{
						*searchsh = neighsh;
						return ACROSSEDGE; // ACROSS_SEG
					}
				}
				spivotself( neighsh );
				if(sdest( neighsh ) != pc) sesymself( neighsh );
				*searchsh = neighsh;
			}
		}
		else
		{ // MOVE_CA
			senext2( *searchsh, neighsh );
			if(chkencflag)
			{
				// Do not cross boundary.
				if(isshsubseg( neighsh ))
				{
					*searchsh = neighsh;
					return ACROSSEDGE; // ACROSS_SEG
				}
			}
			spivotself( neighsh );
			if(neighsh.sh != NULL)
			{
				if(sdest( neighsh ) != pc) sesymself( neighsh );
				*searchsh = neighsh;
			}
			else
			{
				// The same reason as above. 
				// Try the next side, i.e., (startpt->pb).
				if(chkencflag)
				{
					// Do not cross boundary.
					if(isshsubseg( *searchsh ))
					{
						return ACROSSEDGE; // ACROSS_SEG
					}
				}
				spivot( *searchsh, neighsh );
				if(sorg( neighsh ) != pb) sesymself( neighsh );
				senext( neighsh, *searchsh );
			}
		}
	} // while

	if(dir == SHAREEDGE)
	{
		if(insertsegflag)
		{
			// Insert the segment into the triangulation.
			face newseg;
			makeshellface( subsegs, &newseg );
			setshvertices( newseg, startpt, endpt, NULL );
			// Set the default segment marker.
			setshellmark( newseg, -1 );
			ssbond( *searchsh, newseg );
			spivot( *searchsh, neighsh );
			if(neighsh.sh != NULL)
			{
				ssbond( neighsh, newseg );
			}
		}
		return dir;
	}

	if(dir == ACROSSVERT)
	{
		// A point is found collinear with this segment.
		if(reporterrorflag)
		{
			point pp = sdest( *searchsh );
			myprintf( "PLC Error:  A vertex lies in a segment in facet #%d.\n",
					shellmark( *searchsh ) );
			myprintf( "  Vertex:  [%d] (%g,%g,%g).\n", pointmark( pp ), pp[ 0 ], pp[ 1 ], pp[ 2 ] );
			myprintf( "  Segment: [%d, %d]\n", pointmark( startpt ), pointmark( endpt ) );
		}
		return dir;
	}

	if(dir == ACROSSEDGE)
	{
		// Edge [b, c] intersects with the segment.
		senext( *searchsh, flipshs[ 0 ] );
		if(isshsubseg( flipshs[ 0 ] ))
		{
			if(reporterrorflag)
			{
				REAL P[ 3 ], Q[ 3 ], tp = 0, tq = 0;
				linelineint( startpt, endpt, pb, pc, P, Q, &tp, &tq );
				myprintf( "PLC Error:  Two segments intersect at point (%g,%g,%g),",
						P[ 0 ], P[ 1 ], P[ 2 ] );
				myprintf( " in facet #%d.\n", shellmark( *searchsh ) );
				myprintf( "  Segment 1: [%d, %d]\n", pointmark( pb ), pointmark( pc ) );
				myprintf( "  Segment 2: [%d, %d]\n", pointmark( startpt ), pointmark( endpt ) );
			}
			return dir; // ACROSS_SEG
		}
		// Flip edge [b, c], queue unflipped edges (for Delaunay checks).
		spivot( flipshs[ 0 ], flipshs[ 1 ] );
		if(sorg( flipshs[ 1 ] ) != sdest( flipshs[ 0 ] )) sesymself( flipshs[ 1 ] );
		flip22( flipshs, 1, 0 );
		// The flip may create an inverted triangle, check it.
		pa = sapex( flipshs[ 1 ] );
		pb = sapex( flipshs[ 0 ] );
		pc = sorg( flipshs[ 0 ] );
		pd = sdest( flipshs[ 0 ] );
		// Check if pa and pb are on the different sides of [pc, pd]. 
		// Re-use ori_ab, ori_ca for the tests.
		ori_ab = orient3d( pc, pd, dummypoint, pb );
		ori_ca = orient3d( pd, pc, dummypoint, pa );
		if(ori_ab <= 0)
		{
			flipshpush( &( flipshs[ 0 ] ) );
		}
		else if(ori_ca <= 0)
		{
			flipshpush( &( flipshs[ 1 ] ) );
		}
		// Set 'searchsh' s.t. its origin is 'startpt'.
		*searchsh = flipshs[ 0 ];
	}

	return sscoutsegment( searchsh, endpt, insertsegflag, reporterrorflag,
						  chkencflag );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// scarveholes()    Remove triangles not in the facet.                       //
//                                                                           //
// This routine re-uses the two global arrays: caveshlist and caveshbdlist.  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::scarveholes( int holes, REAL* holelist )
{
	face *parysh, searchsh, neighsh;
	enum locateresult loc;
	int i, j;

	// Get all triangles. Infect unprotected convex hull triangles. 
	smarktest( recentsh );
	caveshlist->newindex( (void **)&parysh );
	*parysh = recentsh;
	for(i = 0; i < caveshlist->objects; i++)
	{
		parysh = (face *)fastlookup( caveshlist, i );
		searchsh = *parysh;
		searchsh.shver = 0;
		for(j = 0; j < 3; j++)
		{
			spivot( searchsh, neighsh );
			// Is this side on the convex hull?
			if(neighsh.sh != NULL)
			{
				if(!smarktested( neighsh ))
				{
					smarktest( neighsh );
					caveshlist->newindex( (void **)&parysh );
					*parysh = neighsh;
				}
			}
			else
			{
				// A hull side. Check if it is protected by a segment.
				if(!isshsubseg( searchsh ))
				{
					// Not protected. Save this face.
					if(!sinfected( searchsh ))
					{
						sinfect( searchsh );
						caveshbdlist->newindex( (void **)&parysh );
						*parysh = searchsh;
					}
				}
			}
			senextself( searchsh );
		}
	}

	// Infect the triangles in the holes.
	for(i = 0; i < 3 * holes; i += 3)
	{
		searchsh = recentsh;
		loc = slocate( &( holelist[ i ] ), &searchsh, 1, 1, 0 );
		if(loc != OUTSIDE)
		{
			sinfect( searchsh );
			caveshbdlist->newindex( (void **)&parysh );
			*parysh = searchsh;
		}
	}

	// Find and infect all exterior triangles.
	for(i = 0; i < caveshbdlist->objects; i++)
	{
		parysh = (face *)fastlookup( caveshbdlist, i );
		searchsh = *parysh;
		searchsh.shver = 0;
		for(j = 0; j < 3; j++)
		{
			spivot( searchsh, neighsh );
			if(neighsh.sh != NULL)
			{
				if(!isshsubseg( searchsh ))
				{
					if(!sinfected( neighsh ))
					{
						sinfect( neighsh );
						caveshbdlist->newindex( (void **)&parysh );
						*parysh = neighsh;
					}
				}
				else
				{
					sdissolve( neighsh ); // Disconnect a protected face.
				}
			}
			senextself( searchsh );
		}
	}

	// Delete exterior triangles, unmark interior triangles.
	for(i = 0; i < caveshlist->objects; i++)
	{
		parysh = (face *)fastlookup( caveshlist, i );
		if(sinfected( *parysh ))
		{
			shellfacedealloc( subfaces, parysh->sh );
		}
		else
		{
			sunmarktest( *parysh );
		}
	}

	caveshlist->restart( );
	caveshbdlist->restart( );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// triangulate()    Create a CDT for the facet.                              //
//                                                                           //
// All vertices of the triangulation have type FACETVERTEX.  The actual type //
//   of boundary vertices are set by the routine unifysements().             //
//                                                                           //
// All segments created here will have a default marker '-1'.  Some of these //
//   segments will get their actual marker defined in 'edgemarkerlist'.      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::triangulate( int shmark, arraypool* ptlist, arraypool* conlist,
							 int holes, REAL* holelist )
{
	face searchsh, newsh, *parysh;
	face newseg, *paryseg;
	point pa, pb, pc, *ppt, *cons;
	int iloc;
	int i, j;

	if(b->verbose > 2)
	{
		myprintf( "      f%d:  %ld vertices, %ld segments", shmark, ptlist->objects,
				conlist->objects );
		if(holes > 0)
		{
			myprintf( ", %d holes", holes );
		}
		myprintf( ".\n" );
	}

	if(ptlist->objects < 2l)
	{
		// Not a segment or a facet.
		return 1;
	}
	else if(ptlist->objects == 2l)
	{
		pa = *(point *)fastlookup( ptlist, 0 );
		pb = *(point *)fastlookup( ptlist, 1 );
		if(distance( pa, pb ) > 0)
		{
			// It is a single segment.
			makeshellface( subsegs, &newseg );
			setshvertices( newseg, pa, pb, NULL );
			setshellmark( newseg, -1 );
		}
		if(pointtype( pa ) == VOLVERTEX)
		{
			setpointtype( pa, FACETVERTEX );
		}
		if(pointtype( pb ) == VOLVERTEX)
		{
			setpointtype( pb, FACETVERTEX );
		}
		return 1;
	}
	else if(ptlist->objects == 3)
	{
		pa = *(point *)fastlookup( ptlist, 0 );
		pb = *(point *)fastlookup( ptlist, 1 );
		pc = *(point *)fastlookup( ptlist, 2 );
	}
	else
	{
		// Calculate an above point of this facet.
		if(!calculateabovepoint( ptlist, &pa, &pb, &pc ))
		{
			if(!b->quiet)
			{
				myprintf( "Warning:  Unable to triangulate facet #%d. Skipped!\n", shmark );
			}
			return 0; // The point set is degenerate.
		}
	}

	// Create an initial triangulation.
	makeshellface( subfaces, &newsh );
	setshvertices( newsh, pa, pb, pc );
	setshellmark( newsh, shmark );
	recentsh = newsh;

	if(pointtype( pa ) == VOLVERTEX)
	{
		setpointtype( pa, FACETVERTEX );
	}
	if(pointtype( pb ) == VOLVERTEX)
	{
		setpointtype( pb, FACETVERTEX );
	}
	if(pointtype( pc ) == VOLVERTEX)
	{
		setpointtype( pc, FACETVERTEX );
	}

	// Are there area constraints?
	if(b->quality && ( in->facetconstraintlist != NULL ))
	{
		for(i = 0; i < in->numberoffacetconstraints; i++)
		{
			if(shmark == ( (int)in->facetconstraintlist[ i * 2 ] ))
			{
				REAL area = in->facetconstraintlist[ i * 2 + 1 ];
				setareabound( newsh, area );
				break;
			}
		}
	}

	if(ptlist->objects == 3)
	{
		// The triangulation only has one element.
		for(i = 0; i < 3; i++)
		{
			makeshellface( subsegs, &newseg );
			setshvertices( newseg, sorg( newsh ), sdest( newsh ), NULL );
			setshellmark( newseg, -1 );
			ssbond( newsh, newseg );
			senextself( newsh );
		}
		return 1;
	}

	// Triangulate the facet. It may not success (due to rounding error, or
	//   incorrect input data), use 'caveencshlist' and 'caveencseglist' are
	//   re-used to store all the newly created subfaces and segments. So we
	//   can clean them if the triangulation is not successful.
	caveencshlist->newindex( (void **)&parysh );
	*parysh = newsh;

	// Incrementally build the triangulation.
	pinfect( pa );
	pinfect( pb );
	pinfect( pc );
	for(i = 0; i < ptlist->objects; i++)
	{
		ppt = (point *)fastlookup( ptlist, i );
		if(!pinfected( *ppt ))
		{
			searchsh = recentsh; // Start from 'recentsh'.
			iloc = (int)OUTSIDE;
			// Insert the vertex. Use Bowyer-Watson algo. Round the location.
			iloc = sinsertvertex( *ppt, &searchsh, NULL, iloc, 1, 1 );
			if(iloc != ( (int)ONVERTEX ))
			{
				// Point inserted successfully.
				if(pointtype( *ppt ) == VOLVERTEX)
				{
					setpointtype( *ppt, FACETVERTEX );
				}
				// Save the set of new subfaces.
				for(j = 0; j < caveshbdlist->objects; j++)
				{
					// Get an old subface at edge [a, b].
					parysh = (face *)fastlookup( caveshbdlist, j );
					spivot( *parysh, searchsh ); // The new subface [a, b, p].
					// Do not save a deleted new face (degenerated).
					if(searchsh.sh[ 3 ] != NULL)
					{
						caveencshlist->newindex( (void **)&parysh );
						*parysh = searchsh;
					}
				}
				// Delete all removed subfaces.
				for(j = 0; j < caveshlist->objects; j++)
				{
					parysh = (face *)fastlookup( caveshlist, j );
					shellfacedealloc( subfaces, parysh->sh );
				}
				// Clear the global lists.
				caveshbdlist->restart( );
				caveshlist->restart( );
				cavesegshlist->restart( );
			}
			else
			{
				// The facet triangulation is failed.
				break;
			}
		}
	} // i
	puninfect( pa );
	puninfect( pb );
	puninfect( pc );

	if(i < ptlist->objects)
	{
		//The facet triangulation is failed. Clean the new subfaces.
		//  There is no new segment be created yet.
		if(!b->quiet)
		{
			myprintf( "Warning:  Fail to triangulate facet #%d. Skipped!\n", shmark );
		}
		for(i = 0; i < caveencshlist->objects; i++)
		{
			parysh = (face *)fastlookup( caveencshlist, i );
			if(parysh->sh[ 3 ] != NULL)
			{
				shellfacedealloc( subfaces, parysh->sh );
			}
		}
		caveencshlist->restart( );
		return 0;
	}

	// Insert the segments.
	for(i = 0; i < conlist->objects; i++)
	{
		cons = (point *)fastlookup( conlist, i );
		searchsh = recentsh;
		iloc = (int)slocate( cons[ 0 ], &searchsh, 1, 1, 0 );
		if(iloc != (int)ONVERTEX)
		{
			// Not found due to roundoff errors. Do a brute-force search.
			subfaces->traversalinit( );
			searchsh.sh = shellfacetraverse( subfaces );
			while(searchsh.sh != NULL)
			{
				// Only search the subface in the same facet.
				if(shellmark( searchsh ) == shmark)
				{
					if((point)searchsh.sh[ 3 ] == cons[ 0 ])
					{
						searchsh.shver = 0; break;
					}
					else if((point)searchsh.sh[ 4 ] == cons[ 0 ])
					{
						searchsh.shver = 2; break;
					}
					else if((point)searchsh.sh[ 5 ] == cons[ 0 ])
					{
						searchsh.shver = 4; break;
					}
				}
				searchsh.sh = shellfacetraverse( subfaces );
			}
		}
		// Recover the segment. Some edges may be flipped.
		if(sscoutsegment( &searchsh, cons[ 1 ], 1, 1, 0 ) != SHAREEDGE)
		{
			break; // Fail to recover a segment.
		}
		// Save this newseg.
		sspivot( searchsh, newseg );
		caveencseglist->newindex( (void **)&paryseg );
		*paryseg = newseg;
		if(flipstack != NULL)
		{
			// Recover locally Delaunay edges.
			lawsonflip( );
		}
	} // i

	if(i < conlist->objects)
	{
		if(!b->quiet)
		{
			myprintf( "Warning:  Fail to recover a segment in facet #%d. Skipped!\n",
					shmark );
		}
		for(i = 0; i < caveencshlist->objects; i++)
		{
			parysh = (face *)fastlookup( caveencshlist, i );
			if(parysh->sh[ 3 ] != NULL)
			{
				shellfacedealloc( subfaces, parysh->sh );
			}
		}
		for(i = 0; i < caveencseglist->objects; i++)
		{
			paryseg = (face *)fastlookup( caveencseglist, i );
			if(paryseg->sh[ 3 ] != NULL)
			{
				shellfacedealloc( subsegs, paryseg->sh );
			}
		}
		caveencshlist->restart( );
		caveencseglist->restart( );
		return 0;
	}

	// Remove exterior and hole triangles.
	scarveholes( holes, holelist );

	caveencshlist->restart( );
	caveencseglist->restart( );
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// unifysegments()    Remove redundant segments and create face links.       //
//                                                                           //
// After this routine, although segments are unique, but some of them may be //
// removed later by mergefacet().  All vertices still have type FACETVERTEX. //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::unifysegments( )
{
	badface *facelink = NULL, *newlinkitem, *f1, *f2;
	face *facperverlist, sface;
	face subsegloop, testseg;
	point torg, tdest;
	REAL ori1, ori2, ori3;
	REAL n1[ 3 ], n2[ 3 ];
	REAL cosang, ang, ang_tol;
	int *idx2faclist;
	int idx, k, m;

	if(b->verbose > 1)
	{
		myprintf( "  Unifying segments.\n" );
	}
	// The limit dihedral angle that two facets are not overlapping.
	ang_tol = b->facet_overlap_ang_tol / 180.0 * PIconstant;
	if(ang_tol < 0.0) ang_tol = 0.0;

	// Create a mapping from vertices to subfaces.
	makepoint2submap( subfaces, idx2faclist, facperverlist );


	subsegloop.shver = 0;
	subsegs->traversalinit( );
	subsegloop.sh = shellfacetraverse( subsegs );
	while(subsegloop.sh != (shellface *)NULL)
	{
		torg = sorg( subsegloop );
		tdest = sdest( subsegloop );

		idx = pointmark( torg ) - in->firstnumber;
		// Loop through the set of subfaces containing 'torg'.  Get all the
		//   subfaces containing the edge (torg, tdest). Save and order them
		//   in 'sfacelist', the ordering is defined by the right-hand rule
		//   with thumb points from torg to tdest.
		for(k = idx2faclist[ idx ]; k < idx2faclist[ idx + 1 ]; k++)
		{
			sface = facperverlist[ k ];
			// The face may be deleted if it is a duplicated face.
			if(sface.sh[ 3 ] == NULL) continue;
			// Search the edge torg->tdest.
			if(sdest( sface ) != tdest)
			{
				senext2self( sface );
				sesymself( sface );
			}
			if(sdest( sface ) != tdest) continue;

			// Save the face f in facelink.
			if(flippool->items >= 2)
			{
				f1 = facelink;
				for(m = 0; m < flippool->items - 1; m++)
				{
					f2 = f1->nextitem;
					ori1 = orient3d( torg, tdest, sapex( f1->ss ), sapex( f2->ss ) );
					ori2 = orient3d( torg, tdest, sapex( f1->ss ), sapex( sface ) );
					if(ori1 > 0)
					{
						// apex(f2) is below f1.
						if(ori2 > 0)
						{
							// apex(f) is below f1 (see Fig.1). 
							ori3 = orient3d( torg, tdest, sapex( f2->ss ), sapex( sface ) );
							if(ori3 > 0)
							{
								// apex(f) is below f2, insert it.
								break;
							}
							else if(ori3 < 0)
							{
								// apex(f) is above f2, continue.
							}
							else
							{ // ori3 == 0; 
								// f is coplanar and codirection with f2.
								report_overlapping_facets( &( f2->ss ), &sface );
								break;
							}
						}
						else if(ori2 < 0)
						{
							// apex(f) is above f1 below f2, inset it (see Fig. 2).
							break;
						}
						else
						{ // ori2 == 0;
							// apex(f) is coplanar with f1 (see Fig. 5).
							ori3 = orient3d( torg, tdest, sapex( f2->ss ), sapex( sface ) );
							if(ori3 > 0)
							{
								// apex(f) is below f2, insert it.
								break;
							}
							else
							{
								// f is coplanar and codirection with f1.
								report_overlapping_facets( &( f1->ss ), &sface );
								break;
							}
						}
					}
					else if(ori1 < 0)
					{
						// apex(f2) is above f1.
						if(ori2 > 0)
						{
							// apex(f) is below f1, continue (see Fig. 3).
						}
						else if(ori2 < 0)
						{
							// apex(f) is above f1 (see Fig.4).
							ori3 = orient3d( torg, tdest, sapex( f2->ss ), sapex( sface ) );
							if(ori3 > 0)
							{
								// apex(f) is below f2, insert it.
								break;
							}
							else if(ori3 < 0)
							{
								// apex(f) is above f2, continue.
							}
							else
							{ // ori3 == 0;
								// f is coplanar and codirection with f2.
								report_overlapping_facets( &( f2->ss ), &sface );
								break;
							}
						}
						else
						{ // ori2 == 0;
							// f is coplanar and with f1 (see Fig. 6).
							ori3 = orient3d( torg, tdest, sapex( f2->ss ), sapex( sface ) );
							if(ori3 > 0)
							{
								// f is also codirection with f1.
								report_overlapping_facets( &( f1->ss ), &sface );
								break;
							}
							else
							{
								// f is above f2, continue.
							}
						}
					}
					else
					{ // ori1 == 0;
						// apex(f2) is coplanar with f1. By assumption, f1 is not
						//   coplanar and codirection with f2.
						if(ori2 > 0)
						{
							// apex(f) is below f1, continue (see Fig. 7).
						}
						else if(ori2 < 0)
						{
							// apex(f) is above f1, insert it (see Fig. 7).
							break;
						}
						else
						{ // ori2 == 0.
							// apex(f) is coplanar with f1 (see Fig. 8).
							// f is either codirection with f1 or is codirection with f2. 
							facenormal( torg, tdest, sapex( f1->ss ), n1, 1, NULL );
							facenormal( torg, tdest, sapex( sface ), n2, 1, NULL );
							if(dot( n1, n2 ) > 0)
							{
								report_overlapping_facets( &( f1->ss ), &sface );
							}
							else
							{
								report_overlapping_facets( &( f2->ss ), &sface );
							}
							break;
						}
					}
					// Go to the next item;
					f1 = f2;
				} // for (m = 0; ...)
				if(sface.sh[ 3 ] != NULL)
				{
					// Insert sface between f1 and f2.
					newlinkitem = (badface *)flippool->alloc( );
					newlinkitem->ss = sface;
					newlinkitem->nextitem = f1->nextitem;
					f1->nextitem = newlinkitem;
				}
			}
			else if(flippool->items == 1)
			{
				f1 = facelink;
				// Make sure that f is not coplanar and codirection with f1.
				ori1 = orient3d( torg, tdest, sapex( f1->ss ), sapex( sface ) );
				if(ori1 == 0)
				{
					// f is coplanar with f1 (see Fig. 8).
					facenormal( torg, tdest, sapex( f1->ss ), n1, 1, NULL );
					facenormal( torg, tdest, sapex( sface ), n2, 1, NULL );
					if(dot( n1, n2 ) > 0)
					{
						// The two faces are codirectional as well.
						report_overlapping_facets( &( f1->ss ), &sface );
					}
				}
				// Add this face to link if it is not deleted.
				if(sface.sh[ 3 ] != NULL)
				{
					// Add this face into link.
					newlinkitem = (badface *)flippool->alloc( );
					newlinkitem->ss = sface;
					newlinkitem->nextitem = NULL;
					f1->nextitem = newlinkitem;
				}
			}
			else
			{
				// The first face.
				newlinkitem = (badface *)flippool->alloc( );
				newlinkitem->ss = sface;
				newlinkitem->nextitem = NULL;
				facelink = newlinkitem;
			}
		} // for (k = idx2faclist[idx]; ...)


		// Set the connection between this segment and faces containing it,
		//   at the same time, remove redundant segments.
		f1 = facelink;
		for(k = 0; k < flippool->items; k++)
		{
			sspivot( f1->ss, testseg );
			// If 'testseg' is not 'subsegloop' and is not dead, it is redundant.
			if(( testseg.sh != subsegloop.sh ) && ( testseg.sh[ 3 ] != NULL ))
			{
				shellfacedealloc( subsegs, testseg.sh );
			}
			// Bonds the subface and the segment together.
			ssbond( f1->ss, subsegloop );
			f1 = f1->nextitem;
		}

		// Create the face ring at the segment.
		if(flippool->items > 1)
		{
			f1 = facelink;
			for(k = 1; k <= flippool->items; k++)
			{
				k < flippool->items ? f2 = f1->nextitem : f2 = facelink;
				// Calculate the dihedral angle between the two facet.
				facenormal( torg, tdest, sapex( f1->ss ), n1, 1, NULL );
				facenormal( torg, tdest, sapex( f2->ss ), n2, 1, NULL );
				cosang = dot( n1, n2 ) / ( sqrt( dot( n1, n1 ) ) * sqrt( dot( n2, n2 ) ) );
				// Rounding.
				if(cosang > 1.0) cosang = 1.0;
				else if(cosang < -1.0) cosang = -1.0;
				ang = acos( cosang );
				if(ang < ang_tol)
				{
					// Two facets are treated as overlapping each other.
					report_overlapping_facets( &( f1->ss ), &( f2->ss ), ang );
				}
				else
				{
					// Record the smallest input dihedral angle.
					if(ang < minfacetdihed)
					{
						minfacetdihed = ang;
					}
					sbond1( f1->ss, f2->ss );
				}
				f1 = f2;
			}
		}

		flippool->restart( );

		// Are there length constraints?
		if(b->quality && ( in->segmentconstraintlist != (REAL *)NULL ))
		{
			int e1, e2;
			REAL len;
			for(k = 0; k < in->numberofsegmentconstraints; k++)
			{
				e1 = (int)in->segmentconstraintlist[ k * 3 ];
				e2 = (int)in->segmentconstraintlist[ k * 3 + 1 ];
				if(( ( pointmark( torg ) == e1 ) && ( pointmark( tdest ) == e2 ) ) ||
					( ( pointmark( torg ) == e2 ) && ( pointmark( tdest ) == e1 ) ))
				{
					len = in->segmentconstraintlist[ k * 3 + 2 ];
					setareabound( subsegloop, len );
					break;
				}
			}
		}

		subsegloop.sh = shellfacetraverse( subsegs );
	}

	delete[ ] idx2faclist;
	delete[ ] facperverlist;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// identifyinputedges()    Identify input edges.                             //
//                                                                           //
// A set of input edges is provided in the 'in->edgelist'.  We find these    //
// edges in the surface mesh and make them segments of the mesh.             //
//                                                                           //
// It is possible that an input edge is not in any facet, i.e.,it is a float-//
// segment inside the volume.                                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::identifyinputedges( point *idx2verlist )
{
	face* shperverlist;
	int* idx2shlist;
	face searchsh, neighsh;
	face segloop, checkseg, newseg;
	point checkpt, pa = NULL, pb = NULL;
	int *endpts;
	int edgemarker;
	int idx, i, j;

	int e1, e2;
	REAL len;

	if(!b->quiet)
	{
		myprintf( "Inserting edges ...\n" );
	}

	// Construct a map from points to subfaces.
	makepoint2submap( subfaces, idx2shlist, shperverlist );

	// Process the set of input edges.
	for(i = 0; i < in->numberofedges; i++)
	{
		endpts = &( in->edgelist[ ( i << 1 ) ] );
		if(endpts[ 0 ] == endpts[ 1 ])
		{
			if(!b->quiet)
			{
				myprintf( "Warning:  Edge #%d is degenerated. Skipped.\n", i );
			}
			continue; // Skip a degenerated edge.
		}
		// Recall that all existing segments have a default marker '-1'. 
		// We assign all identified segments a default marker '-2'.
		edgemarker = in->edgemarkerlist ? in->edgemarkerlist[ i ] : -2;

		// Find a face contains the edge.
		newseg.sh = NULL;
		searchsh.sh = NULL;
		idx = endpts[ 0 ] - in->firstnumber;
		for(j = idx2shlist[ idx ]; j < idx2shlist[ idx + 1 ]; j++)
		{
			checkpt = sdest( shperverlist[ j ] );
			if(pointmark( checkpt ) == endpts[ 1 ])
			{
				searchsh = shperverlist[ j ];
				break; // Found.
			}
			else
			{
				checkpt = sapex( shperverlist[ j ] );
				if(pointmark( checkpt ) == endpts[ 1 ])
				{
					senext2( shperverlist[ j ], searchsh );
					sesymself( searchsh );
					break;
				}
			}
		} // j

		if(searchsh.sh != NULL)
		{
			// Check if this edge is already a segment of the mesh.
			sspivot( searchsh, checkseg );
			if(checkseg.sh != NULL)
			{
				// This segment already exist.
				newseg = checkseg;
			}
			else
			{
				// Create a new segment at this edge.
				pa = sorg( searchsh );
				pb = sdest( searchsh );
				makeshellface( subsegs, &newseg );
				setshvertices( newseg, pa, pb, NULL );
				ssbond( searchsh, newseg );
				spivot( searchsh, neighsh );
				if(neighsh.sh != NULL)
				{
					ssbond( neighsh, newseg );
				}
			}
		}
		else
		{
			// It is a dangling segment (not belong to any facets).
			// Get the two endpoints of this segment.
			pa = idx2verlist[ endpts[ 0 ] ];
			pb = idx2verlist[ endpts[ 1 ] ];
			if(pa == pb)
			{
				if(!b->quiet)
				{
					myprintf( "Warning:  Edge #%d is degenerated. Skipped.\n", i );
				}
				continue;
			}
			// Check if segment [a,b] already exists.
			// TODO: Change the brute-force search. Slow!
			point *ppt;
			subsegs->traversalinit( );
			segloop.sh = shellfacetraverse( subsegs );
			while(segloop.sh != NULL)
			{
				ppt = (point *)&( segloop.sh[ 3 ] );
				if(( ( ppt[ 0 ] == pa ) && ( ppt[ 1 ] == pb ) ) ||
					( ( ppt[ 0 ] == pb ) && ( ppt[ 1 ] == pa ) ))
				{
					// Found!
					newseg = segloop;
					break;
				}
				segloop.sh = shellfacetraverse( subsegs );
			}
			if(newseg.sh == NULL)
			{
				makeshellface( subsegs, &newseg );
				setshvertices( newseg, pa, pb, NULL );
			}
		}

		setshellmark( newseg, edgemarker );

		if(b->quality && ( in->segmentconstraintlist != (REAL *)NULL ))
		{
			for(i = 0; i < in->numberofsegmentconstraints; i++)
			{
				e1 = (int)in->segmentconstraintlist[ i * 3 ];
				e2 = (int)in->segmentconstraintlist[ i * 3 + 1 ];
				if(( ( pointmark( pa ) == e1 ) && ( pointmark( pb ) == e2 ) ) ||
					( ( pointmark( pa ) == e2 ) && ( pointmark( pb ) == e1 ) ))
				{
					len = in->segmentconstraintlist[ i * 3 + 2 ];
					setareabound( newseg, len );
					break;
				}
			}
		}
	} // i

	delete[ ] shperverlist;
	delete[ ] idx2shlist;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// mergefacets()    Merge adjacent facets.                                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::mergefacets( )
{
	face parentsh, neighsh, neineish;
	face segloop;
	point pa, pb, pc, pd;
	REAL n1[ 3 ], n2[ 3 ];
	REAL cosang, cosang_tol;


	// Allocate an array to save calcaulated dihedral angles at segments.
	arraypool *dihedangarray = new arraypool( sizeof( double ), 10 );
	REAL *paryang = NULL;

	// First, remove coplanar segments.
	// The dihedral angle bound for two different facets.
	cosang_tol = cos( b->facet_separate_ang_tol / 180.0 * PIconstant );

	subsegs->traversalinit( );
	segloop.sh = shellfacetraverse( subsegs );
	while(segloop.sh != (shellface *)NULL)
	{
		// Only remove a segment if it has a marker '-1'.
		if(shellmark( segloop ) != -1)
		{
			segloop.sh = shellfacetraverse( subsegs );
			continue;
		}
		spivot( segloop, parentsh );
		if(parentsh.sh != NULL)
		{
			spivot( parentsh, neighsh );
			if(neighsh.sh != NULL)
			{
				spivot( neighsh, neineish );
				if(neineish.sh == parentsh.sh)
				{
					// Exactly two subfaces at this segment.
					// Only merge them if they have the same boundary marker.
					if(shellmark( parentsh ) == shellmark( neighsh ))
					{
						pa = sorg( segloop );
						pb = sdest( segloop );
						pc = sapex( parentsh );
						pd = sapex( neighsh );
						// Calculate the dihedral angle at the segment [a,b].
						facenormal( pa, pb, pc, n1, 1, NULL );
						facenormal( pa, pb, pd, n2, 1, NULL );
						cosang = dot( n1, n2 ) / ( sqrt( dot( n1, n1 ) ) * sqrt( dot( n2, n2 ) ) );
						if(cosang < cosang_tol)
						{
							ssdissolve( parentsh );
							ssdissolve( neighsh );
							shellfacedealloc( subsegs, segloop.sh );
							// Add the edge to flip stack.
							flipshpush( &parentsh );
						}
						else
						{
							// Save 'cosang' to avoid re-calculate it.
							// Re-use the pointer at the first segment.
							dihedangarray->newindex( (void **)&paryang );
							*paryang = cosang;
							segloop.sh[ 6 ] = (shellface)paryang;
						}
					}
				} // if (neineish.sh == parentsh.sh)
			}
		}
		segloop.sh = shellfacetraverse( subsegs );
	}

	// Second, remove ridge segments at small angles.
	// The dihedral angle bound for two different facets.
	cosang_tol = cos( b->facet_small_ang_tol / 180.0 * PIconstant );
	REAL cosang_sep_tol = cos( ( b->facet_separate_ang_tol - 5.0 ) / 180.0 * PIconstant );
	face shloop;
	face seg1, seg2;
	REAL cosang1, cosang2;
	int i, j;

	subfaces->traversalinit( );
	shloop.sh = shellfacetraverse( subfaces );
	while(shloop.sh != (shellface *)NULL)
	{
		for(i = 0; i < 3; i++)
		{
			if(isshsubseg( shloop ))
			{
				senext( shloop, neighsh );
				if(isshsubseg( neighsh ))
				{
					// Found two segments sharing at one vertex.
					// Check if they form a small angle.
					pa = sorg( shloop );
					pb = sdest( shloop );
					pc = sapex( shloop );
					for(j = 0; j < 3; j++) n1[ j ] = pa[ j ] - pb[ j ];
					for(j = 0; j < 3; j++) n2[ j ] = pc[ j ] - pb[ j ];
					cosang = dot( n1, n2 ) / ( sqrt( dot( n1, n1 ) ) * sqrt( dot( n2, n2 ) ) );
					if(cosang > cosang_tol)
					{
						// Found a small angle.
						segloop.sh = NULL;
						sspivot( shloop, seg1 );
						sspivot( neighsh, seg2 );
						if(seg1.sh[ 6 ] != NULL)
						{
							paryang = (REAL *)( seg1.sh[ 6 ] );
							cosang1 = *paryang;
						}
						else
						{
							cosang1 = 1.0; // 0 degree;
						}
						if(seg2.sh[ 6 ] != NULL)
						{
							paryang = (REAL *)( seg2.sh[ 6 ] );
							cosang2 = *paryang;
						}
						else
						{
							cosang2 = 1.0; // 0 degree;
						}
						if(cosang1 < cosang_sep_tol)
						{
							if(cosang2 < cosang_sep_tol)
							{
								if(cosang1 < cosang2)
								{
									segloop = seg1;
								}
								else
								{
									segloop = seg2;
								}
							}
							else
							{
								segloop = seg1;
							}
						}
						else
						{
							if(cosang2 < cosang_sep_tol)
							{
								segloop = seg2;
							}
						}
						if(segloop.sh != NULL)
						{
							// Remove this segment.
							segloop.shver = 0;
							spivot( segloop, parentsh );
							spivot( parentsh, neighsh );
							ssdissolve( parentsh );
							ssdissolve( neighsh );
							shellfacedealloc( subsegs, segloop.sh );
							// Add the edge to flip stack.
							flipshpush( &parentsh );
							break;
						}
					}
				} // if (isshsubseg)
			} // if (isshsubseg)
			senextself( shloop );
		}
		shloop.sh = shellfacetraverse( subfaces );
	}

	delete dihedangarray;

	if(flipstack != NULL)
	{
		lawsonflip( ); // Recover Delaunayness.
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// meshsurface()    Create a surface mesh of the input PLC.                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::meshsurface( )
{
	arraypool *ptlist, *conlist;
	point *idx2verlist;
	point tstart, tend, *pnewpt, *cons;
	tetgenio::facet *f;
	tetgenio::polygon *p;
	int end1, end2;
	int shmark, i, j;

	if(!b->quiet)
	{
		myprintf( "Creating surface mesh ...\n" );
	}

	// Create a map from indices to points.
	makeindex2pointmap( idx2verlist );

	// Initialize arrays (block size: 2^8 = 256).
	ptlist = new arraypool( sizeof( point * ), 8 );
	conlist = new arraypool( 2 * sizeof( point * ), 8 );

	// Loop the facet list, triangulate each facet.
	for(shmark = 1; shmark <= in->numberoffacets; shmark++)
	{

		// Get a facet F.
		f = &in->facetlist[ shmark - 1 ];

		// Process the duplicated points first, they are marked with type
		//   DUPLICATEDVERTEX.  If p and q are duplicated, and p'index > q's,
		//   then p is substituted by q.
		if(dupverts > 0l)
		{
			// Loop all polygons of this facet.
			for(i = 0; i < f->numberofpolygons; i++)
			{
				p = &( f->polygonlist[ i ] );
				// Loop other vertices of this polygon.
				for(j = 0; j < p->numberofvertices; j++)
				{
					end1 = p->vertexlist[ j ];
					tstart = idx2verlist[ end1 ];
					if(pointtype( tstart ) == DUPLICATEDVERTEX)
					{
						// Reset the index of vertex-j.
						tend = point2ppt( tstart );
						end2 = pointmark( tend );
						p->vertexlist[ j ] = end2;
					}
				}
			}
		}

		// Loop polygons of F, get the set of vertices and segments.
		for(i = 0; i < f->numberofpolygons; i++)
		{
			// Get a polygon.
			p = &( f->polygonlist[ i ] );
			// Get the first vertex.
			end1 = p->vertexlist[ 0 ];
			if(( end1 < in->firstnumber ) ||
				( end1 >= in->firstnumber + in->numberofpoints ))
			{
				if(!b->quiet)
				{
					myprintf( "Warning:  Invalid the 1st vertex %d of polygon", end1 );
					myprintf( " %d in facet %d.\n", i + 1, shmark );
				}
				continue; // Skip this polygon.
			}
			tstart = idx2verlist[ end1 ];
			// Add tstart to V if it haven't been added yet.
			if(!pinfected( tstart ))
			{
				pinfect( tstart );
				ptlist->newindex( (void **)&pnewpt );
				*pnewpt = tstart;
			}
			// Loop other vertices of this polygon.
			for(j = 1; j <= p->numberofvertices; j++)
			{
				// get a vertex.
				if(j < p->numberofvertices)
				{
					end2 = p->vertexlist[ j ];
				}
				else
				{
					end2 = p->vertexlist[ 0 ];  // Form a loop from last to first.
				}
				if(( end2 < in->firstnumber ) ||
					( end2 >= in->firstnumber + in->numberofpoints ))
				{
					if(!b->quiet)
					{
						myprintf( "Warning:  Invalid vertex %d in polygon %d", end2, i + 1 );
						myprintf( " in facet %d.\n", shmark );
					}
				}
				else
				{
					if(end1 != end2)
					{
						// 'end1' and 'end2' form a segment.
						tend = idx2verlist[ end2 ];
						// Add tstart to V if it haven't been added yet.
						if(!pinfected( tend ))
						{
							pinfect( tend );
							ptlist->newindex( (void **)&pnewpt );
							*pnewpt = tend;
						}
						// Save the segment in S (conlist).
						conlist->newindex( (void **)&cons );
						cons[ 0 ] = tstart;
						cons[ 1 ] = tend;
						// Set the start for next continuous segment.
						end1 = end2;
						tstart = tend;
					}
					else
					{
						// Two identical vertices mean an isolated vertex of F.
						if(p->numberofvertices > 2)
						{
							// This may be an error in the input, anyway, we can continue
							//   by simply skipping this segment.
							if(!b->quiet)
							{
								myprintf( "Warning:  Polygon %d has two identical verts", i + 1 );
								myprintf( " in facet %d.\n", shmark );
							}
						}
						// Ignore this vertex.
					}
				}
				// Is the polygon degenerate (a segment or a vertex)?
				if(p->numberofvertices == 2) break;
			}
		}
		// Unmark vertices.
		for(i = 0; i < ptlist->objects; i++)
		{
			pnewpt = (point *)fastlookup( ptlist, i );
			puninfect( *pnewpt );
		}

		// Triangulate F into a CDT.
		// If in->facetmarklist is NULL, use the default marker -1. 
		triangulate( in->facetmarkerlist ? in->facetmarkerlist[ shmark - 1 ] : -1,
					 ptlist, conlist, f->numberofholes, f->holelist );

		// Clear working lists.
		ptlist->restart( );
		conlist->restart( );
	}

	if(!b->diagnose)
	{
		// Remove redundant segments and build the face links.
		unifysegments( );
		if(in->numberofedges > 0)
		{
			// There are input segments. Insert them.
			identifyinputedges( idx2verlist );
		}
		if(!b->psc && !b->nomergefacet &&
			( !b->nobisect || ( b->nobisect && !b->nobisect_nomerge ) ))
		{
			// Merge coplanar facets.
			mergefacets( );
		}
	}

	if(b->object == tetgenbehavior::STL)
	{
		// Remove redundant vertices (for .stl input mesh).
		jettisonnodes( );
	}

	if(b->verbose)
	{
		myprintf( "  %ld (%ld) subfaces (segments).\n", subfaces->items,
				subsegs->items );
	}

	// The total number of iunput segments.
	insegments = subsegs->items;

	delete[ ] idx2verlist;
	delete ptlist;
	delete conlist;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// interecursive()    Recursively do intersection test on a set of triangles.//
//                                                                           //
// Recursively split the set 'subfacearray' of subfaces into two sets using  //
// a cut plane parallel to x-, or, y-, or z-axis.  The split criteria are    //
// follows. Assume the cut plane is H, and H+ denotes the left halfspace of  //
// H, and H- denotes the right halfspace of H; and s be a subface:           //
//                                                                           //
//    (1) If all points of s lie at H+, put it into left array;              //
//    (2) If all points of s lie at H-, put it into right array;             //
//    (3) If some points of s lie at H+ and some of lie at H-, or some       //
//        points lie on H, put it into both arraies.                         //
//                                                                           //
// Partitions by x-axis if axis == '0'; by y-axis if axis == '1'; by z-axis  //
// if axis == '2'. If current cut plane is parallel to the x-axis, the next  //
// one will be parallel to y-axis, and the next one after the next is z-axis,//
// and then alternately return back to x-axis.                               //
//                                                                           //
// Stop splitting when the number of triangles of the input array is not     //
// decreased anymore. Do tests on the current set.                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::interecursive( shellface** subfacearray, int arraysize,
								int axis, REAL bxmin, REAL bxmax, REAL bymin,
								REAL bymax, REAL bzmin, REAL bzmax,
								int* internum )
{
	shellface **leftarray, **rightarray;
	face sface1, sface2;
	point p1, p2, p3;
	point p4, p5, p6;
	enum interresult intersect;
	REAL split;
	bool toleft, toright;
	int leftsize, rightsize;
	int i, j;

	if(b->verbose > 2)
	{
		myprintf( "      Recur %d faces. Bbox (%g, %g, %g),(%g, %g, %g). %s-axis\n",
				arraysize, bxmin, bymin, bzmin, bxmax, bymax, bzmax,
				axis == 0 ? "x" : ( axis == 1 ? "y" : "z" ) );
	}

	leftarray = new shellface*[ arraysize ];
	if(leftarray == NULL)
	{
		terminatetetgen( this, 1 );
	}
	rightarray = new shellface*[ arraysize ];
	if(rightarray == NULL)
	{
		terminatetetgen( this, 1 );
	}
	leftsize = rightsize = 0;

	if(axis == 0)
	{
		// Split along x-axis.
		split = 0.5 * ( bxmin + bxmax );
	}
	else if(axis == 1)
	{
		// Split along y-axis.
		split = 0.5 * ( bymin + bymax );
	}
	else
	{
		// Split along z-axis.
		split = 0.5 * ( bzmin + bzmax );
	}

	for(i = 0; i < arraysize; i++)
	{
		sface1.sh = subfacearray[ i ];
		p1 = (point)sface1.sh[ 3 ];
		p2 = (point)sface1.sh[ 4 ];
		p3 = (point)sface1.sh[ 5 ];
		toleft = toright = false;
		if(p1[ axis ] < split)
		{
			toleft = true;
			if(p2[ axis ] >= split || p3[ axis ] >= split)
			{
				toright = true;
			}
		}
		else if(p1[ axis ] > split)
		{
			toright = true;
			if(p2[ axis ] <= split || p3[ axis ] <= split)
			{
				toleft = true;
			}
		}
		else
		{
			// p1[axis] == split;
			toleft = true;
			toright = true;
		}
		if(toleft)
		{
			leftarray[ leftsize ] = sface1.sh;
			leftsize++;
		}
		if(toright)
		{
			rightarray[ rightsize ] = sface1.sh;
			rightsize++;
		}
	}

	if(leftsize < arraysize && rightsize < arraysize)
	{
		// Continue to partition the input set. Now 'subfacearray' has been
		//   split into two sets, it's memory can be freed. 'leftarray' and
		//   'rightarray' will be freed in the next recursive (after they're
		//   partitioned again or performing tests).
		delete[ ] subfacearray;
		// Continue to split these two sets.
		if(axis == 0)
		{
			interecursive( leftarray, leftsize, 1, bxmin, split, bymin, bymax,
						   bzmin, bzmax, internum );
			interecursive( rightarray, rightsize, 1, split, bxmax, bymin, bymax,
						   bzmin, bzmax, internum );
		}
		else if(axis == 1)
		{
			interecursive( leftarray, leftsize, 2, bxmin, bxmax, bymin, split,
						   bzmin, bzmax, internum );
			interecursive( rightarray, rightsize, 2, bxmin, bxmax, split, bymax,
						   bzmin, bzmax, internum );
		}
		else
		{
			interecursive( leftarray, leftsize, 0, bxmin, bxmax, bymin, bymax,
						   bzmin, split, internum );
			interecursive( rightarray, rightsize, 0, bxmin, bxmax, bymin, bymax,
						   split, bzmax, internum );
		}
	}
	else
	{
		if(b->verbose > 1)
		{
			myprintf( "  Checking intersecting faces.\n" );
		}
		// Perform a brute-force compare on the set.
		for(i = 0; i < arraysize; i++)
		{
			sface1.sh = subfacearray[ i ];
			p1 = (point)sface1.sh[ 3 ];
			p2 = (point)sface1.sh[ 4 ];
			p3 = (point)sface1.sh[ 5 ];
			for(j = i + 1; j < arraysize; j++)
			{
				sface2.sh = subfacearray[ j ];
				p4 = (point)sface2.sh[ 3 ];
				p5 = (point)sface2.sh[ 4 ];
				p6 = (point)sface2.sh[ 5 ];
				intersect = ( enum interresult ) tri_tri_inter( p1, p2, p3, p4, p5, p6 );
				if(intersect == INTERSECT || intersect == SHAREFACE)
				{
					if(!b->quiet)
					{
						if(intersect == INTERSECT)
						{
							myprintf( "  Facet #%d intersects facet #%d at triangles:\n",
									shellmark( sface1 ), shellmark( sface2 ) );
							myprintf( "    (%4d, %4d, %4d) and (%4d, %4d, %4d)\n",
									pointmark( p1 ), pointmark( p2 ), pointmark( p3 ),
									pointmark( p4 ), pointmark( p5 ), pointmark( p6 ) );
						}
						else
						{
							myprintf( "  Facet #%d duplicates facet #%d at triangle:\n",
									shellmark( sface1 ), shellmark( sface2 ) );
							myprintf( "    (%4d, %4d, %4d) and (%4d, %4d, %4d)\n",
									pointmark( p1 ), pointmark( p2 ), pointmark( p3 ),
									pointmark( p4 ), pointmark( p5 ), pointmark( p6 ) );
						}
					}
					// Increase the number of intersecting pairs.
					( *internum )++;
					// Infect these two faces (although they may already be infected).
					sinfect( sface1 );
					sinfect( sface2 );
				}
			}
		}
		// Don't forget to free all three arrays. No further partition.
		delete[ ] leftarray;
		delete[ ] rightarray;
		delete[ ] subfacearray;
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// detectinterfaces()    Detect intersecting triangles.                      //
//                                                                           //
// Given a set of triangles,  find the pairs of intersecting triangles from  //
// them.  Here the set of triangles is in 'subfaces' which is a surface mesh //
// of a PLC (.poly or .smesh).                                               //
//                                                                           //
// To detect whether two triangles are intersecting is done by the routine   //
// 'tri_tri_inter()'.  The algorithm for the test is very simple and stable. //
// It is based on geometric orientation test which uses exact arithmetics.   //
//                                                                           //
// Use divide-and-conquer algorithm for reducing the number of intersection  //
// tests.  Start from the bounding box of the input point set, recursively   //
// partition the box into smaller boxes, until the number of triangles in a  //
// box is not decreased anymore. Then perform triangle-triangle tests on the //
// remaining set of triangles.  The memory allocated in the input set is     //
// freed immediately after it has been partitioned into two arrays.  So it   //
// can be re-used for the consequent partitions.                             //
//                                                                           //
// On return, the pool 'subfaces' will be cleared, and only the intersecting //
// triangles remain for output (to a .face file).                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::detectinterfaces( )
{
	shellface **subfacearray;
	face shloop;
	int internum;
	int i;

	if(!b->quiet)
	{
		myprintf( "Detecting self-intersecting facets...\n" );
	}

	// Construct a map from indices to subfaces;
	subfacearray = new shellface*[ subfaces->items ];
	subfaces->traversalinit( );
	shloop.sh = shellfacetraverse( subfaces );
	i = 0;
	while(shloop.sh != (shellface *)NULL)
	{
		subfacearray[ i ] = shloop.sh;
		shloop.sh = shellfacetraverse( subfaces );
		i++;
	}

	internum = 0;
	// Recursively split the set of triangles into two sets using a cut plane
	//   parallel to x-, or, y-, or z-axis.  Stop splitting when the number
	//   of subfaces is not decreasing anymore. Do tests on the current set.
	interecursive( subfacearray, subfaces->items, 0, xmin, xmax, ymin, ymax,
				   zmin, zmax, &internum );

	if(!b->quiet)
	{
		if(internum > 0)
		{
			myprintf( "\n!! Found %d pairs of faces are intersecting.\n\n", internum );
		}
		else
		{
			myprintf( "\nNo faces are intersecting.\n\n" );
		}
	}

	if(internum > 0)
	{
		// Traverse all subfaces, deallocate those have not been infected (they
		//   are not intersecting faces). Uninfect those have been infected.
		//   After this loop, only intersecting faces remain.
		subfaces->traversalinit( );
		shloop.sh = shellfacetraverse( subfaces );
		while(shloop.sh != (shellface *)NULL)
		{
			if(sinfected( shloop ))
			{
				suninfect( shloop );
			}
			else
			{
				shellfacedealloc( subfaces, shloop.sh );
			}
			shloop.sh = shellfacetraverse( subfaces );
		}
	}
	else
	{
		// Deallocate all subfaces.
		subfaces->restart( );
	}
}

////                                                                       ////
////                                                                       ////
//// surface_cxx //////////////////////////////////////////////////////////////

//// constrained_cxx //////////////////////////////////////////////////////////
////                                                                       ////
////                                                                       ////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// makesegmentendpointsmap()    Create a map from a segment to its endpoints.//
//                                                                           //
// The map is saved in the array 'segmentendpointslist'. The length of this  //
// array is twice the number of segments.  Each segment is assigned a unique //
// index (starting from 0).                                                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::makesegmentendpointsmap( )
{
	arraypool *segptlist;
	face segloop, prevseg, nextseg;
	point eorg, edest, *parypt;
	int segindex = 0, idx = 0;
	int i;

	if(b->verbose > 0)
	{
		myprintf( "  Creating the segment-endpoints map.\n" );
	}

	segptlist = new arraypool( 2 * sizeof( point ), 10 );

	// A segment s may have been split into many subsegments. Operate the one
	//   which contains the origin of s. Then mark the rest of subsegments.
	subsegs->traversalinit( );
	segloop.sh = shellfacetraverse( subsegs );
	segloop.shver = 0;
	while(segloop.sh != NULL)
	{
		senext2( segloop, prevseg );
		spivotself( prevseg );
		if(prevseg.sh == NULL)
		{
			eorg = sorg( segloop );
			edest = sdest( segloop );
			setfacetindex( segloop, segindex );
			senext( segloop, nextseg );
			spivotself( nextseg );
			while(nextseg.sh != NULL)
			{
				setfacetindex( nextseg, segindex );
				nextseg.shver = 0;
				if(sorg( nextseg ) != edest) sesymself( nextseg );
				edest = sdest( nextseg );
				// Go the next connected subsegment at edest.
				senextself( nextseg );
				spivotself( nextseg );
			}
			segptlist->newindex( (void **)&parypt );
			parypt[ 0 ] = eorg;
			parypt[ 1 ] = edest;
			segindex++;
		}
		segloop.sh = shellfacetraverse( subsegs );
	}

	if(b->verbose)
	{
		myprintf( "  Found %ld segments.\n", segptlist->objects );
	}

	segmentendpointslist = new point[ segptlist->objects * 2 ];

	totalworkmemory += ( segptlist->objects * 2 ) * sizeof( point * );

	for(i = 0; i < segptlist->objects; i++)
	{
		parypt = (point *)fastlookup( segptlist, i );
		segmentendpointslist[ idx++ ] = parypt[ 0 ];
		segmentendpointslist[ idx++ ] = parypt[ 1 ];
	}

	delete segptlist;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// finddirection()    Find the tet on the path from one point to another.    //
//                                                                           //
// The path starts from 'searchtet''s origin and ends at 'endpt'. On finish, //
// 'searchtet' contains a tet on the path, its origin does not change.       //
//                                                                           //
// The return value indicates one of the following cases (let 'searchtet' be //
// abcd, a is the origin of the path):                                       //
//   - ACROSSVERT, edge ab is collinear with the path;                       //
//   - ACROSSEDGE, edge bc intersects with the path;                         //
//   - ACROSSFACE, face bcd intersects with the path.                        //
//                                                                           //
// WARNING: This routine is designed for convex triangulations, and will not //
// generally work after the holes and concavities have been carved.          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

enum tetgenmesh::interresult
	tetgenmesh::finddirection( triface* searchtet, point endpt )
{
		triface neightet;
		point pa, pb, pc, pd;
		enum { HMOVE, RMOVE, LMOVE } nextmove;
		REAL hori, rori, lori;
		int t1ver;
		int s;

		// The origin is fixed.
		pa = org( *searchtet );
		if((point)searchtet->tet[ 7 ] == dummypoint)
		{
			// A hull tet. Choose the neighbor of its base face.
			decode( searchtet->tet[ 3 ], *searchtet );
			// Reset the origin to be pa.
			if((point)searchtet->tet[ 4 ] == pa)
			{
				searchtet->ver = 11;
			}
			else if((point)searchtet->tet[ 5 ] == pa)
			{
				searchtet->ver = 3;
			}
			else if((point)searchtet->tet[ 6 ] == pa)
			{
				searchtet->ver = 7;
			}
			else
			{
				searchtet->ver = 0;
			}
		}

		pb = dest( *searchtet );
		// Check whether the destination or apex is 'endpt'.
		if(pb == endpt)
		{
			// pa->pb is the search edge.
			return ACROSSVERT;
		}

		pc = apex( *searchtet );
		if(pc == endpt)
		{
			// pa->pc is the search edge.
			eprevesymself( *searchtet );
			return ACROSSVERT;
		}

		// Walk through tets around pa until the right one is found.
		while(1)
		{

			pd = oppo( *searchtet );
			// Check whether the opposite vertex is 'endpt'.
			if(pd == endpt)
			{
				// pa->pd is the search edge.
				esymself( *searchtet );
				enextself( *searchtet );
				return ACROSSVERT;
			}
			// Check if we have entered outside of the domain.
			if(pd == dummypoint)
			{
				// This is possible when the mesh is non-convex.
				if(nonconvex)
				{
					return ACROSSFACE; // return ACROSSSUB; // Hit a bounday.
				}
				else
				{
					terminatetetgen( this, 2 );
				}
			}

			// Now assume that the base face abc coincides with the horizon plane,
			//   and d lies above the horizon.  The search point 'endpt' may lie
			//   above or below the horizon.  We test the orientations of 'endpt'
			//   with respect to three planes: abc (horizon), bad (right plane),
			//   and acd (left plane). 
			hori = orient3d( pa, pb, pc, endpt );
			rori = orient3d( pb, pa, pd, endpt );
			lori = orient3d( pa, pc, pd, endpt );

			// Now decide the tet to move.  It is possible there are more than one
			//   tets are viable moves. Is so, randomly choose one. 
			if(hori > 0)
			{
				if(rori > 0)
				{
					if(lori > 0)
					{
						// Any of the three neighbors is a viable move.
						s = randomnation( 3 );
						if(s == 0)
						{
							nextmove = HMOVE;
						}
						else if(s == 1)
						{
							nextmove = RMOVE;
						}
						else
						{
							nextmove = LMOVE;
						}
					}
					else
					{
						// Two tets, below horizon and below right, are viable.
						if(randomnation( 2 ))
						{
							nextmove = HMOVE;
						}
						else
						{
							nextmove = RMOVE;
						}
					}
				}
				else
				{
					if(lori > 0)
					{
						// Two tets, below horizon and below left, are viable.
						if(randomnation( 2 ))
						{
							nextmove = HMOVE;
						}
						else
						{
							nextmove = LMOVE;
						}
					}
					else
					{
						// The tet below horizon is chosen.
						nextmove = HMOVE;
					}
				}
			}
			else
			{
				if(rori > 0)
				{
					if(lori > 0)
					{
						// Two tets, below right and below left, are viable.
						if(randomnation( 2 ))
						{
							nextmove = RMOVE;
						}
						else
						{
							nextmove = LMOVE;
						}
					}
					else
					{
						// The tet below right is chosen.
						nextmove = RMOVE;
					}
				}
				else
				{
					if(lori > 0)
					{
						// The tet below left is chosen.
						nextmove = LMOVE;
					}
					else
					{
						// 'endpt' lies either on the plane(s) or across face bcd.
						if(hori == 0)
						{
							if(rori == 0)
							{
								// pa->'endpt' is COLLINEAR with pa->pb.
								return ACROSSVERT;
							}
							if(lori == 0)
							{
								// pa->'endpt' is COLLINEAR with pa->pc.
								eprevesymself( *searchtet ); // [a,c,d]
								return ACROSSVERT;
							}
							// pa->'endpt' crosses the edge pb->pc.
							return ACROSSEDGE;
						}
						if(rori == 0)
						{
							if(lori == 0)
							{
								// pa->'endpt' is COLLINEAR with pa->pd.
								esymself( *searchtet ); // face bad.
								enextself( *searchtet ); // face [a,d,b]
								return ACROSSVERT;
							}
							// pa->'endpt' crosses the edge pb->pd.
							esymself( *searchtet ); // face bad.
							enextself( *searchtet ); // face adb
							return ACROSSEDGE;
						}
						if(lori == 0)
						{
							// pa->'endpt' crosses the edge pc->pd.
							eprevesymself( *searchtet ); // [a,c,d]
							return ACROSSEDGE;
						}
						// pa->'endpt' crosses the face bcd.
						return ACROSSFACE;
					}
				}
			}

			// Move to the next tet, fix pa as its origin.
			if(nextmove == RMOVE)
			{
				fnextself( *searchtet );
			}
			else if(nextmove == LMOVE)
			{
				eprevself( *searchtet );
				fnextself( *searchtet );
				enextself( *searchtet );
			}
			else
			{ // HMOVE
				fsymself( *searchtet );
				enextself( *searchtet );
			}
			pb = dest( *searchtet );
			pc = apex( *searchtet );

		} // while (1)

	}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// scoutsegment()    Search an edge in the tetrahedralization.               //
//                                                                           //
// If the edge is found, it returns SHAREEDGE, and 'searchtet' returns the   //
// edge from startpt to endpt.                                               //
//                                                                           //
// If the edge is missing, it returns either ACROSSEDGE or ACROSSFACE, which //
// indicates that the edge intersects an edge or a face.  If 'refpt' is NULL,//
// 'searchtet' returns the edge or face. If 'refpt' is not NULL, it returns  //
// a vertex which encroaches upon this edge, and 'searchtet' returns a tet   //
// which containing 'refpt'.                                                 // 
//                                                                           //
// The parameter 'sedge' is used to report self-intersection. It is the      //
// whose endpoints are 'startpt' and 'endpt'. It must not be a NULL.
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

enum tetgenmesh::interresult tetgenmesh::scoutsegment( point startpt, point endpt,
	face *sedge, triface* searchtet, point* refpt, arraypool* intfacelist )
{
	point pd;
	enum interresult dir;
	int t1ver;

	if(b->verbose > 2)
	{
		myprintf( "      Scout seg (%d, %d).\n", pointmark( startpt ), pointmark( endpt ) );
	}

	point2tetorg( startpt, *searchtet );
	dir = finddirection( searchtet, endpt );

	if(dir == ACROSSVERT)
	{
		pd = dest( *searchtet );
		if(pd == endpt)
		{
			if(issubseg( *searchtet ))
			{
				report_selfint_edge( startpt, endpt, sedge, searchtet, dir );
			}
			return SHAREEDGE;
		}
		else
		{
			// A point is on the path.
			report_selfint_edge( startpt, endpt, sedge, searchtet, dir );
			return ACROSSVERT;
		}
	}

	// dir is either ACROSSEDGE or ACROSSFACE.
	enextesymself( *searchtet ); // Go to the opposite face.
	fsymself( *searchtet ); // Enter the adjacent tet.

	if(dir == ACROSSEDGE)
	{
		// Check whether two segments are intersecting.
		if(issubseg( *searchtet ))
		{
			report_selfint_edge( startpt, endpt, sedge, searchtet, dir );
		}
	}
	else if(dir == ACROSSFACE)
	{
		if(checksubfaceflag)
		{
			// Check whether a segment and a subface are intersecting.
			if(issubface( *searchtet ))
			{
				report_selfint_edge( startpt, endpt, sedge, searchtet, dir );
			}
		}
	}
	else
	{
		terminatetetgen( this, 2 );
	}

	if(refpt == NULL)
	{
		// Do not need a reference point. Return.
		return dir;
	}

	triface neightet, reftet;
	point pa, pb, pc;
	REAL angmax, ang;
	int types[ 2 ], poss[ 4 ];
	int pos = 0, i, j;

	pa = org( *searchtet );
	angmax = interiorangle( pa, startpt, endpt, NULL );
	*refpt = pa;
	pb = dest( *searchtet );
	ang = interiorangle( pb, startpt, endpt, NULL );
	if(ang > angmax)
	{
		angmax = ang;
		*refpt = pb;
	}
	pc = apex( *searchtet );
	ang = interiorangle( pc, startpt, endpt, NULL );
	if(ang > angmax)
	{
		angmax = ang;
		*refpt = pc;
	}
	reftet = *searchtet; // Save the tet containing the refpt.

	// Search intersecting faces along the segment.
	while(1)
	{


		pd = oppo( *searchtet );


		// Stop if we meet 'endpt'.
		if(pd == endpt) break;

		ang = interiorangle( pd, startpt, endpt, NULL );
		if(ang > angmax)
		{
			angmax = ang;
			*refpt = pd;
			reftet = *searchtet;
		}

		// Find a face intersecting the segment.
		if(dir == ACROSSFACE)
		{
			// One of the three oppo faces in 'searchtet' intersects the segment.
			neightet = *searchtet;
			j = ( neightet.ver & 3 ); // j is the current face number.
			for(i = j + 1; i < j + 4; i++)
			{
				neightet.ver = ( i % 4 );
				pa = org( neightet );
				pb = dest( neightet );
				pc = apex( neightet );
				pd = oppo( neightet ); // The above point.
				if(tri_edge_test( pa, pb, pc, startpt, endpt, pd, 1, types, poss ))
				{
					dir = ( enum interresult ) types[ 0 ];
					pos = poss[ 0 ];
					break;
				}
				else
				{
					dir = DISJOINT;
					pos = 0;
				}
			}
		}
		else if(dir == ACROSSEDGE)
		{
			// Check the two opposite faces (of the edge) in 'searchtet'.      
			for(i = 0; i < 2; i++)
			{
				if(i == 0)
				{
					enextesym( *searchtet, neightet );
				}
				else
				{
					eprevesym( *searchtet, neightet );
				}
				pa = org( neightet );
				pb = dest( neightet );
				pc = apex( neightet );
				pd = oppo( neightet ); // The above point.
				if(tri_edge_test( pa, pb, pc, startpt, endpt, pd, 1, types, poss ))
				{
					dir = ( enum interresult ) types[ 0 ];
					pos = poss[ 0 ];
					break;
				}
				else
				{
					dir = DISJOINT;
					pos = 0;
				}
			}
			if(dir == DISJOINT)
			{
				// No intersection. Rotate to the next tet at the edge.
				dir = ACROSSEDGE;
				fnextself( *searchtet );
				continue;
			}
		}

		if(dir == ACROSSVERT)
		{
			// This segment passing a vertex. Choose it and return.
			for(i = 0; i < pos; i++)
			{
				enextself( neightet );
			}
			eprev( neightet, *searchtet );
			// dest(*searchtet) lies on the segment.
			report_selfint_edge( startpt, endpt, sedge, searchtet, dir );
			return ACROSSVERT;
		}
		else if(dir == ACROSSEDGE)
		{
			// Get the edge intersects with the segment.
			for(i = 0; i < pos; i++)
			{
				enextself( neightet );
			}
		}
		// Go to the next tet.
		fsym( neightet, *searchtet );

		if(dir == ACROSSEDGE)
		{
			// Check whether two segments are intersecting.
			if(issubseg( *searchtet ))
			{
				report_selfint_edge( startpt, endpt, sedge, searchtet, dir );
			}
		}
		else if(dir == ACROSSFACE)
		{
			if(checksubfaceflag)
			{
				// Check whether a segment and a subface are intersecting.
				if(issubface( *searchtet ))
				{
					report_selfint_edge( startpt, endpt, sedge, searchtet, dir );
				}
			}
		}
		else
		{
			terminatetetgen( this, 2 );
		}

	} // while (1)

	// A valid reference point should inside the diametrial circumsphere of
	//   the missing segment, i.e., it encroaches upon it.
	if(2.0 * angmax < PIconstant)
	{
		*refpt = NULL;
	}


	*searchtet = reftet;
	return dir;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// getsteinerpointonsegment()    Get a Steiner point on a segment.           //
//                                                                           //
// Return '1' if 'refpt' lies on an adjacent segment of this segment. Other- //
// wise, return '0'.                                                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::getsteinerptonsegment( face* seg, point refpt, point steinpt, REAL* uv )
{
	point ei = sorg( *seg );
	point ej = sdest( *seg );
	int adjflag = 0, i;

	if(refpt != NULL)
	{
		REAL L, L1, t;

		if(pointtype( refpt ) == FREESEGVERTEX)
		{
			face parentseg;
			sdecode( point2sh( refpt ), parentseg );
			int sidx1 = getfacetindex( parentseg );
			point far_pi = segmentendpointslist[ sidx1 * 2 ];
			point far_pj = segmentendpointslist[ sidx1 * 2 + 1 ];
			int sidx2 = getfacetindex( *seg );
			point far_ei = segmentendpointslist[ sidx2 * 2 ];
			point far_ej = segmentendpointslist[ sidx2 * 2 + 1 ];
			if(( far_pi == far_ei ) || ( far_pj == far_ei ))
			{
				// Create a Steiner point at the intersection of the segment
				//   [far_ei, far_ej] and the sphere centered at far_ei with 
				//   radius |far_ei - refpt|.
				L = distance( far_ei, far_ej );
				L1 = distance( far_ei, refpt );
				t = L1 / L;
				for(i = 0; i < 3; i++)
				{
					steinpt[ i ] = far_ei[ i ] + t * ( far_ej[ i ] - far_ei[ i ] );
				}
				// UV
				if(uv)
				{
					for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
					{
						REAL far_eiuv, far_ejuv;
						far_eiuv = pointgeomuv( far_ei, tt );
						far_ejuv = pointgeomuv( far_ej, tt );
						uv[ tt ] = far_eiuv + t * ( far_ejuv - far_eiuv );
					}
				}
				adjflag = 1;
			}
			else if(( far_pi == far_ej ) || ( far_pj == far_ej ))
			{
				L = distance( far_ei, far_ej );
				L1 = distance( far_ej, refpt );
				t = L1 / L;
				for(i = 0; i < 3; i++)
				{
					steinpt[ i ] = far_ej[ i ] + t * ( far_ei[ i ] - far_ej[ i ] );
				}
				// UV
				if(uv)
				{
					for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
					{
						REAL far_eiuv, far_ejuv;
						far_eiuv = pointgeomuv( far_ei, tt );
						far_ejuv = pointgeomuv( far_ej, tt );
						uv[ tt ] = far_ejuv + t * ( far_eiuv - far_ejuv );
					}
				}
				adjflag = 1;
			}
			else
			{
				// Cut the segment by the projection point of refpt.
				projpt2edge( refpt, ei, ej, steinpt, uv );
			}
		}
		else
		{
			// Cut the segment by the projection point of refpt.
			projpt2edge( refpt, ei, ej, steinpt, uv );
		}

		// Make sure that steinpt is not too close to ei and ej.
		L = distance( ei, ej );
		L1 = distance( steinpt, ei );
		t = L1 / L;
		if(( t < 0.2 ) || ( t > 0.8 ))
		{
			// Split the point at the middle.
			for(i = 0; i < 3; i++)
			{
				steinpt[ i ] = ei[ i ] + 0.5 * ( ej[ i ] - ei[ i ] );
			}
			// UV
			if( uv )
			{
				for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
				{
					REAL iuv, juv;
					iuv = pointgeomuv( ei, tt );
					juv = pointgeomuv( ej, tt );
					uv[ tt ] = iuv + 0.5 * ( juv - iuv );
				}
			}
		}
	}
	else
	{
		// Split the point at the middle.
		for(i = 0; i < 3; i++)
		{
			steinpt[ i ] = ei[ i ] + 0.5 * ( ej[ i ] - ei[ i ] );
		}
		// UV
		if( uv ) 
		{
			for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
			{
				REAL iuv, juv;
				iuv = pointgeomuv( ei, tt );
				juv = pointgeomuv( ej, tt );
				uv[ tt ] = iuv + 0.5 * ( juv - iuv );
			}
		}

	}


	return adjflag;
}



///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// delaunizesegments()    Recover segments in a DT.                          //
//                                                                           //
// All segments need to be recovered are in 'subsegstack' (Q).  They will be //
// be recovered one by one (in a random order).                              //
//                                                                           //
// Given a segment s in the Q, this routine first queries s in the DT, if s  //
// matches an edge in DT, it is 'locked' at the edge. Otherwise, s is split  //
// by inserting a new point p in both the DT and itself. The two new subseg- //
// ments of s are queued in Q.  The process continues until Q is empty.      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::delaunizesegments( )
{
	triface searchtet, spintet;
	face searchsh;
	face sseg, *psseg;
	point refpt, newpt;
	enum interresult dir;
	insertvertexflags ivf;
	int t1ver;


	ivf.bowywat = 1; // Use Bowyer-Watson insertion.
	ivf.sloc = (int)ONEDGE; // on 'sseg'.
	ivf.sbowywat = 1; // Use Bowyer-Watson insertion.
	ivf.assignmeshsize = b->metric;
	ivf.smlenflag = useinsertradius; // Return the closet mesh vertex.

	// Loop until 'subsegstack' is empty.
	while(subsegstack->objects > 0l)
	{
		// seglist is used as a stack.
		subsegstack->objects--;
		psseg = (face *)fastlookup( subsegstack, subsegstack->objects );
		sseg = *psseg;

		// Check if this segment has been recovered.
		sstpivot1( sseg, searchtet );
		if(searchtet.tet != NULL)
		{
			continue; // Not a missing segment.
		}

		// Search the segment.
		dir = scoutsegment( sorg( sseg ), sdest( sseg ), &sseg, &searchtet, &refpt, NULL );

		if(dir == SHAREEDGE)
		{
			// Found this segment, insert it.
			// Let the segment remember an adjacent tet.
			sstbond1( sseg, searchtet );
			// Bond the segment to all tets containing it.
			spintet = searchtet;
			do
			{
				tssbond1( spintet, sseg );
				fnextself( spintet );
			}
			while(spintet.tet != searchtet.tet);
		}
		else
		{
			if(( dir == ACROSSFACE ) || ( dir == ACROSSEDGE ))
			{
				// The segment is missing. Split it.
				// Create a new point.
				REAL uv[ tetgenio::pointparam::maxuvs ];
				makepoint( &newpt, FREESEGVERTEX );
				//setpointtype(newpt, FREESEGVERTEX);
				getsteinerptonsegment( &sseg, refpt, newpt, uv );
				if(b->psc)
				{
					// UV: TODO: getsteinerptonsegment doesn't fix seams!
					for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
					{
						setpointgeomuv( newpt, tt, uv[ tt ] );
					}
				}

				// Start searching from 'searchtet'.
				ivf.iloc = (int)OUTSIDE;
				// Insert the new point into the tetrahedralization T.
				//   Missing segments and subfaces are queued for recovery.
				//   Note that T is convex (nonconvex = 0).
				if(insertpoint( newpt, &searchtet, &searchsh, &sseg, &ivf ))
				{
					// The new point has been inserted.
					st_segref_count++;
					if(steinerleft > 0) steinerleft--;
					if(useinsertradius)
					{
						save_segmentpoint_insradius( newpt, ivf.parentpt, ivf.smlen );
					}
				}
				else
				{
					if(ivf.iloc == (int)NEARVERTEX)
					{
						// The new point (in the segment) is very close to an existing 
						//   vertex -- a small feature is detected.
						point nearpt = org( searchtet );
						if(pointtype( nearpt ) == FREESEGVERTEX)
						{
							face parentseg;
							sdecode( point2sh( nearpt ), parentseg );
							point p1 = farsorg( sseg );
							point p2 = farsdest( sseg );
							point p3 = farsorg( parentseg );
							point p4 = farsdest( parentseg );
							myprintf( "Two segments are very close to each other.\n" );
							myprintf( "  Segment 1: [%d, %d] #%d\n", pointmark( p1 ),
									pointmark( p2 ), shellmark( sseg ) );
							myprintf( "  Segment 2: [%d, %d] #%d\n", pointmark( p3 ),
									pointmark( p4 ), shellmark( parentseg ) );
							terminatetetgen( this, 4 );
						}
						else
						{
							terminatetetgen( this, 2 );
						}
					}
					else if(ivf.iloc == (int)ONVERTEX)
					{
						// The new point (in the segment) is coincident with an existing 
						//   vertex -- a self-intersection is detected.
						eprevself( searchtet );
						report_selfint_edge( sorg( sseg ), sdest( sseg ), &sseg, &searchtet,
											 ACROSSVERT );
					}
					else
					{
						// An unknown case. Report a bug.
						terminatetetgen( this, 2 );
					}
				}
			}
			else
			{
				// An unknown case. Report a bug.
				terminatetetgen( this, 2 );
			}
		}
	} // while
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// scoutsubface()    Search subface in the tetrahedralization.               //
//                                                                           //
// 'searchsh' is searched in T. If it exists, it is 'locked' at the face in  //
// T. 'searchtet' refers to the face. Otherwise, it is missing.              //
//                                                                           //
// The parameter 'shflag' indicates whether 'searchsh' is a boundary face or //
// not. It is possible that 'searchsh' is a temporarily subface that is used //
// as a cavity boundary face.                                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::scoutsubface( face* searchsh, triface* searchtet, int shflag )
{
	point pa = sorg( *searchsh );
	point pb = sdest( *searchsh );

	// Get a tet whose origin is a.
	point2tetorg( pa, *searchtet );
	// Search the edge [a,b].
	enum interresult dir = finddirection( searchtet, pb );
	if(dir == ACROSSVERT)
	{
		// Check validity of a PLC.
		if(dest( *searchtet ) != pb)
		{
			if(shflag)
			{
				// A vertex lies on the search edge. 
				report_selfint_edge( pa, pb, searchsh, searchtet, dir );
			}
			else
			{
				terminatetetgen( this, 2 );
			}
		}
		int t1ver;
		// The edge exists. Check if the face exists.
		point pc = sapex( *searchsh );
		// Searchtet holds edge [a,b]. Search a face with apex c.
		triface spintet = *searchtet;
		while(1)
		{
			if(apex( spintet ) == pc)
			{
				// Found a face matching to 'searchsh'!
				if(!issubface( spintet ))
				{
					// Insert 'searchsh'.
					tsbond( spintet, *searchsh );
					fsymself( spintet );
					sesymself( *searchsh );
					tsbond( spintet, *searchsh );
					*searchtet = spintet;
					return 1;
				}
				else
				{
					terminatetetgen( this, 2 );
				}
			}
			fnextself( spintet );
			if(spintet.tet == searchtet->tet) break;
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// formregion()    Form the missing region of a missing subface.             //
//                                                                           //
// 'missh' is a missing subface. From it we form a missing region R which is //
// a connected region formed by a set of missing subfaces of a facet.        //
// Comment: There should be no segment inside R.                             //
//                                                                           //
// 'missingshs' returns the list of subfaces in R. All subfaces in this list //
// are oriented as the 'missh'.  'missingshbds' returns the list of boundary //
// edges (tetrahedral handles) of R.  'missingshverts' returns all vertices  //
// of R. They are all pmarktested.                                           //
//                                                                           //
// Except the first one (which is 'missh') in 'missingshs', each subface in  //
// this list represents an internal edge of R, i.e., it is missing in the    //
// tetrahedralization. Since R may contain interior vertices, not all miss-  //
// ing edges can be found by this way.                                       //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::formregion( face* missh, arraypool* missingshs,
							 arraypool* missingshbds, arraypool* missingshverts )
{
	triface searchtet, spintet;
	face neighsh, *parysh;
	face neighseg, fakeseg;
	point pa, pb, *parypt;
	enum interresult dir;
	int t1ver;
	int i, j;

	smarktest( *missh );
	missingshs->newindex( (void **)&parysh );
	*parysh = *missh;

	// Incrementally find other missing subfaces.
	for(i = 0; i < missingshs->objects; i++)
	{
		missh = (face *)fastlookup( missingshs, i );
		for(j = 0; j < 3; j++)
		{
			pa = sorg( *missh );
			pb = sdest( *missh );
			point2tetorg( pa, searchtet );
			dir = finddirection( &searchtet, pb );
			if(dir != ACROSSVERT)
			{
				// This edge is missing. Its neighbor is a missing subface.
				spivot( *missh, neighsh );
				if(!smarktested( neighsh ))
				{
					// Adjust the face orientation.
					if(sorg( neighsh ) != pb) sesymself( neighsh );
					smarktest( neighsh );
					missingshs->newindex( (void **)&parysh );
					*parysh = neighsh;
				}
			}
			else
			{
				if(dest( searchtet ) != pb)
				{
					// Report a PLC problem.
					report_selfint_edge( pa, pb, missh, &searchtet, dir );
				}
			}
			// Collect the vertices of R.
			if(!pmarktested( pa ))
			{
				pmarktest( pa );
				missingshverts->newindex( (void **)&parypt );
				*parypt = pa;
			}
			senextself( *missh );
		} // j
	} // i

	// Get the boundary edges of R.
	for(i = 0; i < missingshs->objects; i++)
	{
		missh = (face *)fastlookup( missingshs, i );
		for(j = 0; j < 3; j++)
		{
			spivot( *missh, neighsh );
			if(( neighsh.sh == NULL ) || !smarktested( neighsh ))
			{
				// A boundary edge of R.
				// Let the segment point to the adjacent tet.
				point2tetorg( sorg( *missh ), searchtet );
				finddirection( &searchtet, sdest( *missh ) );
				missingshbds->newindex( (void **)&parysh );
				*parysh = *missh;
				// Check if this edge is a segment.
				sspivot( *missh, neighseg );
				if(neighseg.sh == NULL)
				{
					// Temporarily create a segment at this edge.
					makeshellface( subsegs, &fakeseg );
					setsorg( fakeseg, sorg( *missh ) );
					setsdest( fakeseg, sdest( *missh ) );
					sinfect( fakeseg ); // Mark it as faked.
					// Connect it to all tets at this edge.
					spintet = searchtet;
					while(1)
					{
						tssbond1( spintet, fakeseg );
						fnextself( spintet );
						if(spintet.tet == searchtet.tet) break;
					}
					neighseg = fakeseg;
				}
				// Let the segment and the boundary edge point to each other.
				ssbond( *missh, neighseg );
				sstbond1( neighseg, searchtet );
			}
			senextself( *missh );
		} // j
	} // i


	// Unmarktest collected missing subfaces.
	for(i = 0; i < missingshs->objects; i++)
	{
		parysh = (face *)fastlookup( missingshs, i );
		sunmarktest( *parysh );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// scoutcrossedge()    Search an edge that crosses the missing region.       //
//                                                                           //
// Return 1 if a crossing edge is found. It is returned by 'crosstet'. More- //
// over, the edge is oriented such that its origin lies below R.  Return 0   //
// if no such edge is found.                                                 //
//                                                                           //
// Assumption: All vertices of the missing region are marktested.            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::scoutcrossedge( triface& crosstet, arraypool* missingshbds,
								arraypool* missingshs )
{
	triface searchtet, spintet, neightet;
	face oldsh, searchsh, *parysh;
	face neighseg;
	point pa, pb, pc, pd, pe;
	REAL ori;
	int types[ 2 ], poss[ 4 ];
	int searchflag, interflag;
	int t1ver;
	int i, j;

	searchflag = 0;

	// Search the first new subface to fill the region.
	for(i = 0; i < missingshbds->objects && !searchflag; i++)
	{
		parysh = (face *)fastlookup( missingshbds, i );
		sspivot( *parysh, neighseg );
		sstpivot1( neighseg, searchtet );
		if(org( searchtet ) != sorg( *parysh ))
		{
			esymself( searchtet );
		}
		spintet = searchtet;
		while(1)
		{
			if(pmarktested( apex( spintet ) ))
			{
				// A possible interior face.
				neightet = spintet;
				oldsh = *parysh;
				// Try to recover an interior edge.
				for(j = 0; j < 2; j++)
				{
					enextself( neightet );
					if(!issubseg( neightet ))
					{
						if(j == 0)
						{
							senext( oldsh, searchsh );
						}
						else
						{
							senext2( oldsh, searchsh );
							sesymself( searchsh );
							esymself( neightet );
						}
						// Calculate a lifted point. 
						pa = sorg( searchsh );
						pb = sdest( searchsh );
						pc = sapex( searchsh );
						pd = dest( neightet );
						calculateabovepoint4( pa, pb, pc, pd );
						// The lifted point must lie above 'searchsh'.
						ori = orient3d( pa, pb, pc, dummypoint );
						if(ori > 0)
						{
							sesymself( searchsh );
							senextself( searchsh );
						}
						else if(ori == 0)
						{
							terminatetetgen( this, 2 );
						}
						if(sscoutsegment( &searchsh, dest( neightet ), 0, 0, 1 ) == SHAREEDGE)
						{
							// Insert a temp segment to protect the recovered edge.
							face tmpseg;
							makeshellface( subsegs, &tmpseg );
							ssbond( searchsh, tmpseg );
							spivotself( searchsh );
							ssbond( searchsh, tmpseg );
							// Recover locally Delaunay edges.
							lawsonflip( );
							// Delete the tmp segment.
							spivot( tmpseg, searchsh );
							ssdissolve( searchsh );
							spivotself( searchsh );
							ssdissolve( searchsh );
							shellfacedealloc( subsegs, tmpseg.sh );
							searchflag = 1;
						}
						else
						{
							// Undo the performed flips.
							if(flipstack != NULL)
							{
								lawsonflip( );
							}
						}
						break;
					} // if (!issubseg(neightet))
				} // j
				if(searchflag) break;
			} // if (pmarktested(apex(spintet)))
			fnextself( spintet );
			if(spintet.tet == searchtet.tet) break;
		}
	} // i

	if(searchflag)
	{
		// Remove faked segments.
		face checkseg;
		// Remark: We should not use the array 'missingshbds', since the flips may
		//   change the subfaces. We search them from the subfaces in R.
		for(i = 0; i < missingshs->objects; i++)
		{
			parysh = (face *)fastlookup( missingshs, i );
			oldsh = *parysh;
			for(j = 0; j < 3; j++)
			{
				if(isshsubseg( oldsh ))
				{
					sspivot( oldsh, checkseg );
					if(sinfected( checkseg ))
					{
						// It's a faked segment. Delete it.
						sstpivot1( checkseg, searchtet );
						spintet = searchtet;
						while(1)
						{
							tssdissolve1( spintet );
							fnextself( spintet );
							if(spintet.tet == searchtet.tet) break;
						}
						shellfacedealloc( subsegs, checkseg.sh );
						ssdissolve( oldsh );
					}
				}
				senextself( oldsh );
			} // j
		}

		fillregioncount++;

		return 0;
	} // if (i < missingshbds->objects)

	searchflag = 0;

	for(j = 0; j < missingshbds->objects && !searchflag; j++)
	{
		parysh = (face *)fastlookup( missingshbds, j );
		sspivot( *parysh, neighseg );
		sstpivot1( neighseg, searchtet );
		interflag = 0;
		// Let 'spintet' be [#,#,d,e] where [#,#] is the boundary edge of R.
		spintet = searchtet;
		while(1)
		{
			pd = apex( spintet );
			pe = oppo( spintet );
			// Skip a hull edge.
			if(( pd != dummypoint ) && ( pe != dummypoint ))
			{
				// Skip an edge containing a vertex of R.
				if(!pmarktested( pd ) && !pmarktested( pe ))
				{
					// Check if [d,e] intersects R.
					for(i = 0; i < missingshs->objects && !interflag; i++)
					{
						parysh = (face *)fastlookup( missingshs, i );
						pa = sorg( *parysh );
						pb = sdest( *parysh );
						pc = sapex( *parysh );
						interflag = tri_edge_test( pa, pb, pc, pd, pe, NULL, 1, types, poss );
						if(interflag > 0)
						{
							if(interflag == 2)
							{
								// They intersect at a single point.
								if(( types[ 0 ] == (int)ACROSSFACE ) ||
									( types[ 0 ] == (int)ACROSSEDGE ))
								{
									// Go to the crossing edge [d,e,#,#].
									edestoppo( spintet, crosstet ); // // [d,e,#,#].
									if(issubseg( crosstet ))
									{
										// It is a segment. Report a PLC problem.
										report_selfint_face( pa, pb, pc, parysh, &crosstet,
															 interflag, types, poss );
									}
									else
									{
										triface chkface = crosstet;
										while(1)
										{
											if(issubface( chkface )) break;
											fsymself( chkface );
											if(chkface.tet == crosstet.tet) break;
										}
										if(issubface( chkface ))
										{
											// Two subfaces are intersecting.
											report_selfint_face( pa, pb, pc, parysh, &chkface,
																 interflag, types, poss );
										}
									}
									// Adjust the edge such that d lies below [a,b,c].
									ori = orient3d( pa, pb, pc, pd );
									if(ori < 0)
									{
										esymself( crosstet );
									}
									searchflag = 1;
								}
								else
								{
									// An improper intersection type, ACROSSVERT, TOUCHFACE,
									//   TOUCHEDGE, SHAREVERT, ...
									// Maybe it is due to a PLC problem.
									report_selfint_face( pa, pb, pc, parysh, &crosstet,
														 interflag, types, poss );
								}
							}
							break;
						} // if (interflag > 0)
					}
				}
			}
			// Leave search at this bdry edge if an intersection is found.
			if(interflag > 0) break;
			// Go to the next tetrahedron.
			fnextself( spintet );
			if(spintet.tet == searchtet.tet) break;
		} // while (1)
	} // j

	return searchflag;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// formcavity()    Form the cavity of a missing region.                      //
//                                                                           //
// The missing region R is formed by a set of missing subfaces 'missingshs'. //
// In the following, we assume R is horizontal and oriented. (All subfaces   //
// of R are oriented in the same way.)  'searchtet' is a tetrahedron [d,e,#, //
// #] which intersects R in its interior, where the edge [d,e] intersects R, //
// and d lies below R.                                                       //
//                                                                           //
// 'crosstets' returns the set of crossing tets. Every tet in it has the     //
// form [d,e,#,#] where [d,e] is a crossing edge, and d lies below R.  The   //
// set of tets form the cavity C, which is divided into two parts by R, one  //
// at top and one at bottom. 'topfaces' and 'botfaces' return the upper and  //
// lower boundary faces of C. 'toppoints' contains vertices of 'crosstets'   //
// in the top part of C, and so does 'botpoints'. Both 'toppoints' and       //
// 'botpoints' contain vertices of R.                                        //
//                                                                           //
// Important: This routine assumes all vertices of the facet containing this //
// subface are marked, i.e., pmarktested(p) returns true.                    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

bool tetgenmesh::formcavity( triface* searchtet, arraypool* missingshs,
							 arraypool* crosstets, arraypool* topfaces,
							 arraypool* botfaces, arraypool* toppoints,
							 arraypool* botpoints )
{
	arraypool *crossedges;
	triface spintet, neightet, chkface, *parytet;
	face *parysh = NULL;
	point pa, pd, pe, *parypt;
	bool testflag, invalidflag;
	int intflag, types[ 2 ], poss[ 4 ];
	int t1ver;
	int i, j, k;

	// Temporarily re-use 'topfaces' for all crossing edges.
	crossedges = topfaces;

	if(b->verbose > 2)
	{
		myprintf( "      Form the cavity of a missing region.\n" );
	}
	// Mark this edge to avoid testing it later.
	markedge( *searchtet );
	crossedges->newindex( (void **)&parytet );
	*parytet = *searchtet;

	invalidflag = 0;
	// Collect all crossing tets.  Each cross tet is saved in the standard
	//   form [d,e,#,#], where [d,e] is a crossing edge, d lies below R.
	//   NEITHER d NOR e is a vertex of R (!pmarktested). 
	for(i = 0; i < crossedges->objects && !invalidflag; i++)
	{
		// Get a crossing edge [d,e,#,#].
		searchtet = (triface *)fastlookup( crossedges, i );
		// Sort vertices into the bottom and top arrays.
		pd = org( *searchtet );
		if(!pinfected( pd ))
		{
			pinfect( pd );
			botpoints->newindex( (void **)&parypt );
			*parypt = pd;
		}
		pe = dest( *searchtet );
		if(!pinfected( pe ))
		{
			pinfect( pe );
			toppoints->newindex( (void **)&parypt );
			*parypt = pe;
		}

		// All tets sharing this edge are crossing tets.
		spintet = *searchtet;
		while(1)
		{
			if(!infected( spintet ))
			{
				infect( spintet );
				crosstets->newindex( (void **)&parytet );
				*parytet = spintet;
			}
			// Go to the next crossing tet.
			fnextself( spintet );
			if(spintet.tet == searchtet->tet) break;
		} // while (1)

		// Detect new crossing edges.
		spintet = *searchtet;
		while(1)
		{
			// spintet is [d,e,a,#], where d lies below R, and e lies above R. 
			pa = apex( spintet );
			if(pa != dummypoint)
			{
				if(!pmarktested( pa ))
				{
					// There exists a crossing edge, either [e,a] or [a,d]. First check
					//   if the crossing edge has already be added, i.e.,to check if one
					//   of the tetrahedron at this edge has been marked.
					testflag = true;
					for(j = 0; j < 2 && testflag; j++)
					{
						if(j == 0)
						{
							enext( spintet, neightet );
						}
						else
						{
							eprev( spintet, neightet );
						}
						while(1)
						{
							if(edgemarked( neightet ))
							{
								// This crossing edge has already been tested. Skip it.
								testflag = false;
								break;
							}
							fnextself( neightet );
							if(neightet.tet == spintet.tet) break;
						}
					} // j
					if(testflag)
					{
						// Test if [e,a] or [a,d] intersects R.
						// Do a brute-force search in the set of subfaces of R. Slow!
						//   Need to be improved!
						pd = org( spintet );
						pe = dest( spintet );
						for(k = 0; k < missingshs->objects; k++)
						{
							parysh = (face *)fastlookup( missingshs, k );
							intflag = tri_edge_test( sorg( *parysh ), sdest( *parysh ),
													 sapex( *parysh ), pe, pa, NULL, 1, types, poss );
							if(intflag > 0)
							{
								// Found intersection. 'a' lies below R.
								if(intflag == 2)
								{
									enext( spintet, neightet );
									if(( types[ 0 ] == (int)ACROSSFACE ) ||
										( types[ 0 ] == (int)ACROSSEDGE ))
									{
										// Only this case is valid.
									}
									else
									{
										// A non-valid intersection. Maybe a PLC problem.
										invalidflag = 1;
									}
								}
								else
								{
									// Coplanar intersection. Maybe a PLC problem.
									invalidflag = 1;
								}
								break;
							}
							intflag = tri_edge_test( sorg( *parysh ), sdest( *parysh ),
													 sapex( *parysh ), pa, pd, NULL, 1, types, poss );
							if(intflag > 0)
							{
								// Found intersection. 'a' lies above R.
								if(intflag == 2)
								{
									eprev( spintet, neightet );
									if(( types[ 0 ] == (int)ACROSSFACE ) ||
										( types[ 0 ] == (int)ACROSSEDGE ))
									{
										// Only this case is valid.
									}
									else
									{
										// A non-valid intersection. Maybe a PLC problem.
										invalidflag = 1;
									}
								}
								else
								{
									// Coplanar intersection. Maybe a PLC problem.
									invalidflag = 1;
								}
								break;
							}
						} // k
						if(k < missingshs->objects)
						{
							// Found a pair of triangle - edge intersection.
							if(invalidflag)
							{
								break; // the while (1) loop
							}
							// Adjust the edge direction, so that its origin lies below R,
							//   and its destination lies above R.
							esymself( neightet );
							// This edge may be a segment.
							if(issubseg( neightet ))
							{
								report_selfint_face( sorg( *parysh ), sdest( *parysh ),
													 sapex( *parysh ), parysh, &neightet, intflag, types, poss );
							}
							// Check if it is an edge of a subface.
							chkface = neightet;
							while(1)
							{
								if(issubface( chkface )) break;
								fsymself( chkface );
								if(chkface.tet == neightet.tet) break;
							}
							if(issubface( chkface ))
							{
								// Two subfaces are intersecting.
								report_selfint_face( sorg( *parysh ), sdest( *parysh ),
													 sapex( *parysh ), parysh, &chkface, intflag, types, poss );
							}

							// Mark this edge to avoid testing it again.
							markedge( neightet );
							crossedges->newindex( (void **)&parytet );
							*parytet = neightet;
						}
						else
						{
							// No intersection is found. It may be a PLC problem.
							invalidflag = 1;
							break; // the while (1) loop
						} // if (k == missingshs->objects)
					} // if (testflag)
				}
			} // if (pa != dummypoint)
			// Go to the next crossing tet.
			fnextself( spintet );
			if(spintet.tet == searchtet->tet) break;
		} // while (1)
	} // i

	// Unmark all marked edges.
	for(i = 0; i < crossedges->objects; i++)
	{
		searchtet = (triface *)fastlookup( crossedges, i );
		unmarkedge( *searchtet );
	}
	crossedges->restart( );


	if(invalidflag)
	{
		// Unmark all collected tets.
		for(i = 0; i < crosstets->objects; i++)
		{
			searchtet = (triface *)fastlookup( crosstets, i );
			uninfect( *searchtet );
		}
		// Unmark all collected vertices.
		for(i = 0; i < botpoints->objects; i++)
		{
			parypt = (point *)fastlookup( botpoints, i );
			puninfect( *parypt );
		}
		for(i = 0; i < toppoints->objects; i++)
		{
			parypt = (point *)fastlookup( toppoints, i );
			puninfect( *parypt );
		}
		crosstets->restart( );
		botpoints->restart( );
		toppoints->restart( );

		// Randomly split an interior edge of R.
		i = randomnation( missingshs->objects - 1 );
		recentsh = *(face *)fastlookup( missingshs, i );
		return false;
	}

	if(b->verbose > 2)
	{
		myprintf( "      Formed cavity: %ld (%ld) cross tets (edges).\n",
				crosstets->objects, crossedges->objects );
	}

	// Collect the top and bottom faces and the middle vertices. Since all top
	//   and bottom vertices have been infected. Uninfected vertices must be
	//   middle vertices (i.e., the vertices of R).
	// NOTE 1: Hull tets may be collected. Process them as a normal one.
	// NOTE 2: Some previously recovered subfaces may be completely inside the
	//   cavity. In such case, we remove these subfaces from the cavity and put
	//   them into 'subfacstack'. They will be recovered later.
	// NOTE 3: Some segments may be completely inside the cavity, e.g., they
	//   attached to a subface which is inside the cavity. Such segments are
	//   put in 'subsegstack'. They will be recovered later. 
	// NOTE4 : The interior subfaces and segments mentioned in NOTE 2 and 3
	//   are identified in the routine "carvecavity()". 

	for(i = 0; i < crosstets->objects; i++)
	{
		searchtet = (triface *)fastlookup( crosstets, i );
		// searchtet is [d,e,a,b].
		eorgoppo( *searchtet, spintet );
		fsym( spintet, neightet ); // neightet is [a,b,e,#]
		if(!infected( neightet ))
		{
			// A top face.
			topfaces->newindex( (void **)&parytet );
			*parytet = neightet;
		}
		edestoppo( *searchtet, spintet );
		fsym( spintet, neightet ); // neightet is [b,a,d,#]
		if(!infected( neightet ))
		{
			// A bottom face.
			botfaces->newindex( (void **)&parytet );
			*parytet = neightet;
		}
		// Add middle vertices if there are (skip dummypoint).
		pa = org( neightet );
		if(!pinfected( pa ))
		{
			if(pa != dummypoint)
			{
				pinfect( pa );
				botpoints->newindex( (void **)&parypt );
				*parypt = pa;
				toppoints->newindex( (void **)&parypt );
				*parypt = pa;
			}
		}
		pa = dest( neightet );
		if(!pinfected( pa ))
		{
			if(pa != dummypoint)
			{
				pinfect( pa );
				botpoints->newindex( (void **)&parypt );
				*parypt = pa;
				toppoints->newindex( (void **)&parypt );
				*parypt = pa;
			}
		}
	} // i

	// Uninfect all collected top, bottom, and middle vertices.
	for(i = 0; i < toppoints->objects; i++)
	{
		parypt = (point *)fastlookup( toppoints, i );
		puninfect( *parypt );
	}
	for(i = 0; i < botpoints->objects; i++)
	{
		parypt = (point *)fastlookup( botpoints, i );
		puninfect( *parypt );
	}
	cavitycount++;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// delaunizecavity()    Fill a cavity by Delaunay tetrahedra.                //
//                                                                           //
// The cavity C to be tetrahedralized is the top or bottom part of a whole   //
// cavity. 'cavfaces' contains the boundary faces of C. NOTE: faces in 'cav- //
// faces' do not form a closed polyhedron.  The "open" side are subfaces of  //
// the missing facet. These faces will be recovered later in fillcavity().   //
//                                                                           //
// This routine first constructs the DT of the vertices. Then it identifies  //
// the half boundary faces of the cavity in DT. Possiblely the cavity C will //
// be enlarged.                                                              //
//                                                                           //
// The DT is returned in 'newtets'.                                          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::delaunizecavity( arraypool *cavpoints, arraypool *cavfaces,
								  arraypool *cavshells, arraypool *newtets,
								  arraypool *crosstets, arraypool *misfaces )
{
	triface searchtet, neightet, *parytet, *parytet1;
	face tmpsh, *parysh;
	point pa, pb, pc, pd, pt[ 3 ], *parypt;
	insertvertexflags ivf;
	REAL ori;
	long baknum, bakhullsize;
	int bakchecksubsegflag, bakchecksubfaceflag;
	int t1ver;
	int i, j;

	if(b->verbose > 2)
	{
		myprintf( "      Delaunizing cavity: %ld points, %ld faces.\n",
				cavpoints->objects, cavfaces->objects );
	}
	// Remember the current number of crossing tets. It may be enlarged later.
	baknum = crosstets->objects;
	bakhullsize = hullsize;
	bakchecksubsegflag = checksubsegflag;
	bakchecksubfaceflag = checksubfaceflag;
	hullsize = 0l;
	checksubsegflag = 0;
	checksubfaceflag = 0;
	b->verbose--;  // Suppress informations for creating Delaunay tetra.
	b->plc = 0; // Do not check near vertices.

	ivf.bowywat = 1; // Use Bowyer-Watson algorithm.

	// Get four non-coplanar points (no dummypoint).
	pa = pb = pc = NULL;
	for(i = 0; i < cavfaces->objects; i++)
	{
		parytet = (triface *)fastlookup( cavfaces, i );
		parytet->ver = epivot[ parytet->ver ];
		if(apex( *parytet ) != dummypoint)
		{
			pa = org( *parytet );
			pb = dest( *parytet );
			pc = apex( *parytet );
			break;
		}
	}
	pd = NULL;
	for(; i < cavfaces->objects; i++)
	{
		parytet = (triface *)fastlookup( cavfaces, i );
		pt[ 0 ] = org( *parytet );
		pt[ 1 ] = dest( *parytet );
		pt[ 2 ] = apex( *parytet );
		for(j = 0; j < 3; j++)
		{
			if(pt[ j ] != dummypoint)
			{ // Do not include a hull point.
				ori = orient3d( pa, pb, pc, pt[ j ] );
				if(ori != 0)
				{
					pd = pt[ j ];
					if(ori > 0)
					{  // Swap pa and pb.
						pt[ j ] = pa; pa = pb; pb = pt[ j ];
					}
					break;
				}
			}
		}
		if(pd != NULL) break;
	}

	// Create an init DT.
	initialdelaunay( pa, pb, pc, pd );

	// Incrementally insert the vertices (duplicated vertices are ignored).
	for(i = 0; i < cavpoints->objects; i++)
	{
		pt[ 0 ] = *(point *)fastlookup( cavpoints, i );
		searchtet = recenttet;
		ivf.iloc = (int)OUTSIDE;
		insertpoint( pt[ 0 ], &searchtet, NULL, NULL, &ivf );
	}

	if(b->verbose > 2)
	{
		myprintf( "      Identifying %ld boundary faces of the cavity.\n",
				cavfaces->objects );
	}

	while(1)
	{

		// Identify boundary faces. Mark interior tets. Save missing faces.
		for(i = 0; i < cavfaces->objects; i++)
		{
			parytet = (triface *)fastlookup( cavfaces, i );
			// Skip an interior face (due to the enlargement of the cavity).
			if(infected( *parytet )) continue;
			parytet->ver = epivot[ parytet->ver ];
			pt[ 0 ] = org( *parytet );
			pt[ 1 ] = dest( *parytet );
			pt[ 2 ] = apex( *parytet );
			// Create a temp subface.
			makeshellface( subfaces, &tmpsh );
			setshvertices( tmpsh, pt[ 0 ], pt[ 1 ], pt[ 2 ] );
			// Insert tmpsh in DT.
			searchtet.tet = NULL;
			if(scoutsubface( &tmpsh, &searchtet, 0 ))
			{ // shflag = 0
				// Inserted! 'tmpsh' must face toward the inside of the cavity.
				// Remember the boundary tet (outside the cavity) in tmpsh 
				//   (use the adjacent tet slot). 
				tmpsh.sh[ 0 ] = (shellface)encode( *parytet );
				// Save this subface.
				cavshells->newindex( (void **)&parysh );
				*parysh = tmpsh;
			}
			else
			{
				// This boundary face is missing.
				shellfacedealloc( subfaces, tmpsh.sh );
				// Save this face in list.
				misfaces->newindex( (void **)&parytet1 );
				*parytet1 = *parytet;
			}
		} // i

		if(misfaces->objects > 0)
		{
			if(b->verbose > 2)
			{
				myprintf( "      Enlarging the cavity. %ld missing bdry faces\n",
						misfaces->objects );
			}

			// Removing all temporary subfaces.
			for(i = 0; i < cavshells->objects; i++)
			{
				parysh = (face *)fastlookup( cavshells, i );
				stpivot( *parysh, neightet );
				tsdissolve( neightet ); // Detach it from adj. tets.
				fsymself( neightet );
				tsdissolve( neightet );
				shellfacedealloc( subfaces, parysh->sh );
			}
			cavshells->restart( );

			// Infect the points which are of the cavity.
			for(i = 0; i < cavpoints->objects; i++)
			{
				pt[ 0 ] = *(point *)fastlookup( cavpoints, i );
				pinfect( pt[ 0 ] ); // Mark it as inserted.
			}

			// Enlarge the cavity.
			for(i = 0; i < misfaces->objects; i++)
			{
				// Get a missing face.
				parytet = (triface *)fastlookup( misfaces, i );
				if(!infected( *parytet ))
				{
					// Put it into crossing tet list.
					infect( *parytet );
					crosstets->newindex( (void **)&parytet1 );
					*parytet1 = *parytet;
					// Insert the opposite point if it is not in DT.
					pd = oppo( *parytet );
					if(!pinfected( pd ))
					{
						searchtet = recenttet;
						ivf.iloc = (int)OUTSIDE;
						insertpoint( pd, &searchtet, NULL, NULL, &ivf );
						pinfect( pd );
						cavpoints->newindex( (void **)&parypt );
						*parypt = pd;
					}
					// Add three opposite faces into the boundary list.
					for(j = 0; j < 3; j++)
					{
						esym( *parytet, neightet );
						fsymself( neightet );
						if(!infected( neightet ))
						{
							cavfaces->newindex( (void **)&parytet1 );
							*parytet1 = neightet;
						}
						enextself( *parytet );
					} // j
				} // if (!infected(parytet))
			} // i

			// Uninfect the points which are of the cavity.
			for(i = 0; i < cavpoints->objects; i++)
			{
				pt[ 0 ] = *(point *)fastlookup( cavpoints, i );
				puninfect( pt[ 0 ] );
			}

			misfaces->restart( );
			continue;
		} // if (misfaces->objects > 0)

		break;

	} // while (1)

	// Collect all tets of the DT. All new tets are marktested.
	marktest( recenttet );
	newtets->newindex( (void **)&parytet );
	*parytet = recenttet;
	for(i = 0; i < newtets->objects; i++)
	{
		searchtet = *(triface *)fastlookup( newtets, i );
		for(j = 0; j < 4; j++)
		{
			decode( searchtet.tet[ j ], neightet );
			if(!marktested( neightet ))
			{
				marktest( neightet );
				newtets->newindex( (void **)&parytet );
				*parytet = neightet;
			}
		}
	}

	cavpoints->restart( );
	cavfaces->restart( );

	if(crosstets->objects > baknum)
	{
		// The cavity has been enlarged.
		cavityexpcount++;
	}

	// Restore the original values.
	hullsize = bakhullsize;
	checksubsegflag = bakchecksubsegflag;
	checksubfaceflag = bakchecksubfaceflag;
	b->verbose++;
	b->plc = 1;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// fillcavity()    Fill new tets into the cavity.                            //
//                                                                           //
// The new tets are stored in two disjoint sets(which share the same facet). //
// 'topfaces' and 'botfaces' are the boundaries of these two sets, respect-  //
// ively. 'midfaces' is empty on input, and will store faces in the facet.   //
//                                                                           //
// Important: This routine assumes all vertices of the missing region R are  //
// marktested, i.e., pmarktested(p) returns true.                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

bool tetgenmesh::fillcavity( arraypool* topshells, arraypool* botshells,
							 arraypool* midfaces, arraypool* missingshs,
							 arraypool* topnewtets, arraypool* botnewtets,
							 triface* crossedge )
{
	arraypool *cavshells;
	triface bdrytet, neightet, *parytet;
	triface searchtet, spintet;
	face *parysh;
	face checkseg;
	point pa, pb, pc;
	bool mflag;
	int t1ver;
	int i, j;

	// Connect newtets to tets outside the cavity.  These connections are needed
	//   for identifying the middle faces (which belong to R).
	for(j = 0; j < 2; j++)
	{
		cavshells = ( j == 0 ? topshells : botshells );
		if(cavshells != NULL)
		{
			for(i = 0; i < cavshells->objects; i++)
			{
				// Get a temp subface.
				parysh = (face *)fastlookup( cavshells, i );
				// Get the boundary tet outside the cavity (saved in sh[0]).
				decode( parysh->sh[ 0 ], bdrytet );
				pa = org( bdrytet );
				pb = dest( bdrytet );
				pc = apex( bdrytet );
				// Get the adjacent new tet inside the cavity.
				stpivot( *parysh, neightet );
				// Mark neightet as an interior tet of this cavity.
				infect( neightet );
				// Connect the two tets (the old connections are replaced).
				bond( bdrytet, neightet );
				tsdissolve( neightet ); // Clear the pointer to tmpsh.
				// Update the point-to-tets map.
				setpoint2tet( pa, (tetrahedron)neightet.tet );
				setpoint2tet( pb, (tetrahedron)neightet.tet );
				setpoint2tet( pc, (tetrahedron)neightet.tet );
			} // i
		} // if (cavshells != NULL)
	} // j

	if(crossedge != NULL)
	{
		// Glue top and bottom tets at their common facet.
		triface toptet, bottet, spintet, *midface;
		point pd, pe;
		REAL ori;
		int types[ 2 ], poss[ 4 ];
		int interflag;
		int bflag;

		mflag = false;
		pd = org( *crossedge );
		pe = dest( *crossedge );

		// Search the first (middle) face in R. 
		// Since R may be non-convex, we must make sure that the face is in the
		//   interior of R.  We search a face in 'topnewtets' whose three vertices
		//   are on R and it intersects 'crossedge' in its interior. Then search
		//   a matching face in 'botnewtets'.
		for(i = 0; i < topnewtets->objects && !mflag; i++)
		{
			searchtet = *(triface *)fastlookup( topnewtets, i );
			for(searchtet.ver = 0; searchtet.ver < 4 && !mflag; searchtet.ver++)
			{
				pa = org( searchtet );
				if(pmarktested( pa ))
				{
					pb = dest( searchtet );
					if(pmarktested( pb ))
					{
						pc = apex( searchtet );
						if(pmarktested( pc ))
						{
							// Check if this face intersects [d,e].
							interflag = tri_edge_test( pa, pb, pc, pd, pe, NULL, 1, types, poss );
							if(interflag == 2)
							{
								// They intersect at a single point. Found.
								toptet = searchtet;
								// The face lies in the interior of R.
								// Get the tet (in topnewtets) which lies above R.
								ori = orient3d( pa, pb, pc, pd );
								if(ori < 0)
								{
									fsymself( toptet );
									pa = org( toptet );
									pb = dest( toptet );
								}
								else if(ori == 0)
								{
									terminatetetgen( this, 2 );
								}
								// Search the face [b,a,c] in 'botnewtets'.
								for(j = 0; j < botnewtets->objects; j++)
								{
									neightet = *(triface *)fastlookup( botnewtets, j );
									// Is neightet contains 'b'.
									if((point)neightet.tet[ 4 ] == pb)
									{
										neightet.ver = 11;
									}
									else if((point)neightet.tet[ 5 ] == pb)
									{
										neightet.ver = 3;
									}
									else if((point)neightet.tet[ 6 ] == pb)
									{
										neightet.ver = 7;
									}
									else if((point)neightet.tet[ 7 ] == pb)
									{
										neightet.ver = 0;
									}
									else
									{
										continue;
									}
									// Is the 'neightet' contains edge [b,a].
									if(dest( neightet ) == pa)
									{
										// 'neightet' is just the edge.
									}
									else if(apex( neightet ) == pa)
									{
										eprevesymself( neightet );
									}
									else if(oppo( neightet ) == pa)
									{
										esymself( neightet );
										enextself( neightet );
									}
									else
									{
										continue;
									}
									// Is 'neightet' the face [b,a,c]. 
									if(apex( neightet ) == pc)
									{
										bottet = neightet;
										mflag = true;
										break;
									}
								} // j
							} // if (interflag == 2)
						} // pc
					} // pb
				} // pa
			} // toptet.ver
		} // i

		if(mflag)
		{
			// Found a pair of matched faces in 'toptet' and 'bottet'.
			bond( toptet, bottet );
			// Both are interior tets.
			infect( toptet );
			infect( bottet );
			// Add this face into search list.
			markface( toptet );
			midfaces->newindex( (void **)&parytet );
			*parytet = toptet;
		}
		else
		{
			// No pair of 'toptet' and 'bottet'.
			toptet.tet = NULL;
			// Randomly split an interior edge of R.
			i = randomnation( missingshs->objects - 1 );
			recentsh = *(face *)fastlookup( missingshs, i );
		}

		// Find other middle faces, connect top and bottom tets.
		for(i = 0; i < midfaces->objects && mflag; i++)
		{
			// Get a matched middle face [a, b, c]
			midface = (triface *)fastlookup( midfaces, i );
			// Check the neighbors at the edges of this face. 
			for(j = 0; j < 3 && mflag; j++)
			{
				toptet = *midface;
				bflag = false;
				while(1)
				{
					// Go to the next face in the same tet.
					esymself( toptet );
					pc = apex( toptet );
					if(pmarktested( pc ))
					{
						break; // Find a subface.
					}
					if(pc == dummypoint)
					{
						terminatetetgen( this, 2 ); // Check this case.
						break; // Find a subface.
					}
					// Go to the adjacent tet.
					fsymself( toptet );
					// Do we walk outside the cavity? 
					if(!marktested( toptet ))
					{
						// Yes, the adjacent face is not a middle face.
						bflag = true; break;
					}
				}
				if(!bflag)
				{
					if(!facemarked( toptet ))
					{
						fsym( *midface, bottet );
						spintet = bottet;
						while(1)
						{
							esymself( bottet );
							pd = apex( bottet );
							if(pd == pc) break; // Face matched.
							fsymself( bottet );
							if(bottet.tet == spintet.tet)
							{
								// Not found a matched bottom face.
								mflag = false;
								break;
							}
						} // while (1)
						if(mflag)
						{
							if(marktested( bottet ))
							{
								// Connect two tets together.
								bond( toptet, bottet );
								// Both are interior tets.
								infect( toptet );
								infect( bottet );
								// Add this face into list.
								markface( toptet );
								midfaces->newindex( (void **)&parytet );
								*parytet = toptet;
							}
							else
							{
								// The 'bottet' is not inside the cavity! 
								terminatetetgen( this, 2 ); // Check this case
							}
						}
						else
						{ // mflag == false
							// Adjust 'toptet' and 'bottet' to be the crossing edges.
							fsym( *midface, bottet );
							spintet = bottet;
							while(1)
							{
								esymself( bottet );
								pd = apex( bottet );
								if(pmarktested( pd ))
								{
									// assert(pd != pc);
									// Let 'toptet' be [a,b,c,#], and 'bottet' be [b,a,d,*].
									// Adjust 'toptet' and 'bottet' to be the crossing edges.
									// Test orient3d(b,c,#,d).
									ori = orient3d( dest( toptet ), pc, oppo( toptet ), pd );
									if(ori < 0)
									{
										// Edges [a,d] and [b,c] cross each other.
										enextself( toptet ); // [b,c]
										enextself( bottet ); // [a,d]
									}
									else if(ori > 0)
									{
										// Edges [a,c] and [b,d] cross each other. 
										eprevself( toptet ); // [c,a]
										eprevself( bottet ); // [d,b]
									}
									else
									{
										// b,c,#,and d are coplanar!.
										terminatetetgen( this, 2 ); //assert(0);
									}
									break; // Not matched
								}
								fsymself( bottet );
							}
						} // if (!mflag)
					} // if (!facemarked(toptet))
				} // if (!bflag)
				enextself( *midface );
			} // j
		} // i

		if(mflag)
		{
			if(b->verbose > 2)
			{
				myprintf( "      Found %ld middle subfaces.\n", midfaces->objects );
			}
			face oldsh, newsh, casout, casin, neighsh;

			oldsh = *(face *)fastlookup( missingshs, 0 );

			// Create new subfaces to fill the region R.
			for(i = 0; i < midfaces->objects; i++)
			{
				// Get a matched middle face [a, b, c]
				midface = (triface *)fastlookup( midfaces, i );
				unmarkface( *midface );
				makeshellface( subfaces, &newsh );
				setsorg( newsh, org( *midface ) );
				setsdest( newsh, dest( *midface ) );
				setsapex( newsh, apex( *midface ) );
				// The new subface gets its markers from the old one.
				setshellmark( newsh, shellmark( oldsh ) );
				if(checkconstraints)
				{
					setareabound( newsh, areabound( oldsh ) );
				}
				if(useinsertradius)
				{
					setfacetindex( newsh, getfacetindex( oldsh ) );
				}
				// Connect the new subface to adjacent tets.
				tsbond( *midface, newsh );
				fsym( *midface, neightet );
				sesymself( newsh );
				tsbond( neightet, newsh );
			}

			// Connect new subfaces together and to the bdry of R.
			// Delete faked segments.
			for(i = 0; i < midfaces->objects; i++)
			{
				// Get a matched middle face [a, b, c]
				midface = (triface *)fastlookup( midfaces, i );
				for(j = 0; j < 3; j++)
				{
					tspivot( *midface, newsh );
					spivot( newsh, casout );
					if(casout.sh == NULL)
					{
						// Search its neighbor.
						fnext( *midface, searchtet );
						while(1)
						{
							// (1) First check if this side is a bdry edge of R.
							tsspivot1( searchtet, checkseg );
							if(checkseg.sh != NULL)
							{
								// It's a bdry edge of R.
								// Get the old subface.
								checkseg.shver = 0;
								spivot( checkseg, oldsh );
								if(sinfected( checkseg ))
								{
									// It's a faked segment. Delete it.
									spintet = searchtet;
									while(1)
									{
										tssdissolve1( spintet );
										fnextself( spintet );
										if(spintet.tet == searchtet.tet) break;
									}
									shellfacedealloc( subsegs, checkseg.sh );
									ssdissolve( oldsh );
									checkseg.sh = NULL;
								}
								spivot( oldsh, casout );
								if(casout.sh != NULL)
								{
									casin = casout;
									if(checkseg.sh != NULL)
									{
										// Make sure that the subface has the right ori at the 
										//   segment.
										checkseg.shver = 0;
										if(sorg( newsh ) != sorg( checkseg ))
										{
											sesymself( newsh );
										}
										spivot( casin, neighsh );
										while(neighsh.sh != oldsh.sh)
										{
											casin = neighsh;
											spivot( casin, neighsh );
										}
									}
									sbond1( newsh, casout );
									sbond1( casin, newsh );
								}
								if(checkseg.sh != NULL)
								{
									ssbond( newsh, checkseg );
								}
								break;
							} // if (checkseg.sh != NULL)
							// (2) Second check if this side is an interior edge of R.
							tspivot( searchtet, neighsh );
							if(neighsh.sh != NULL)
							{
								// Found an adjacent subface of newsh (an interior edge).
								sbond( newsh, neighsh );
								break;
							}
							fnextself( searchtet );
						} // while (1)
					} // if (casout.sh == NULL)
					enextself( *midface );
				} // j
			} // i

			// Delete old subfaces.
			for(i = 0; i < missingshs->objects; i++)
			{
				parysh = (face *)fastlookup( missingshs, i );
				shellfacedealloc( subfaces, parysh->sh );
			}
		}
		else
		{
			if(toptet.tet != NULL)
			{
				// Faces at top and bottom are not matched. 
				// Choose a Steiner point in R.
				// Split one of the crossing edges.
				pa = org( toptet );
				pb = dest( toptet );
				pc = org( bottet );
				pd = dest( bottet );
				// Search an edge in R which is either [a,b] or [c,d].
				// Reminder:  Subfaces in this list 'missingshs', except the first
				//   one, represents an interior edge of R. 
				for(i = 1; i < missingshs->objects; i++)
				{
					parysh = (face *)fastlookup( missingshs, i );
					if(( ( sorg( *parysh ) == pa ) && ( sdest( *parysh ) == pb ) ) ||
						( ( sorg( *parysh ) == pb ) && ( sdest( *parysh ) == pa ) )) break;
					if(( ( sorg( *parysh ) == pc ) && ( sdest( *parysh ) == pd ) ) ||
						( ( sorg( *parysh ) == pd ) && ( sdest( *parysh ) == pc ) )) break;
				}
				if(i < missingshs->objects)
				{
					// Found. Return it.
					recentsh = *parysh;
				}
				else
				{
					terminatetetgen( this, 2 ); //assert(0);
				}
			}
		}

		midfaces->restart( );
	}
	else
	{
		mflag = true;
	}

	// Delete the temp subfaces.
	for(j = 0; j < 2; j++)
	{
		cavshells = ( j == 0 ? topshells : botshells );
		if(cavshells != NULL)
		{
			for(i = 0; i < cavshells->objects; i++)
			{
				parysh = (face *)fastlookup( cavshells, i );
				shellfacedealloc( subfaces, parysh->sh );
			}
		}
	}

	topshells->restart( );
	if(botshells != NULL)
	{
		botshells->restart( );
	}

	return mflag;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// carvecavity()    Delete old tets and outer new tets of the cavity.        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::carvecavity( arraypool *crosstets, arraypool *topnewtets,
							  arraypool *botnewtets )
{
	arraypool *newtets;
	shellface *sptr, *ssptr;
	triface *parytet, *pnewtet, newtet, neightet, spintet;
	face checksh, *parysh;
	face checkseg, *paryseg;
	int t1ver;
	int i, j;

	if(b->verbose > 2)
	{
		myprintf( "      Carve cavity: %ld old tets.\n", crosstets->objects );
	}

	// First process subfaces and segments which are adjacent to the cavity.
	//   They must be re-connected to new tets in the cavity.
	// Comment: It is possible that some subfaces and segments are completely
	//   inside the cavity. This can happen even if the cavity is not enlarged. 
	//   Before deleting the old tets, find and queue all interior subfaces
	//   and segments. They will be recovered later. 2010-05-06.

	// Collect all subfaces and segments which attached to the old tets.
	for(i = 0; i < crosstets->objects; i++)
	{
		parytet = (triface *)fastlookup( crosstets, i );
		if(( sptr = (shellface*)parytet->tet[ 9 ] ) != NULL)
		{
			for(j = 0; j < 4; j++)
			{
				if(sptr[ j ])
				{
					sdecode( sptr[ j ], checksh );
					if(!sinfected( checksh ))
					{
						sinfect( checksh );
						cavetetshlist->newindex( (void **)&parysh );
						*parysh = checksh;
					}
				}
			} // j
		}
		if(( ssptr = (shellface*)parytet->tet[ 8 ] ) != NULL)
		{
			for(j = 0; j < 6; j++)
			{
				if(ssptr[ j ])
				{
					sdecode( ssptr[ j ], checkseg );
					// Skip a deleted segment (was a faked segment)
					if(checkseg.sh[ 3 ] != NULL)
					{
						if(!sinfected( checkseg ))
						{
							sinfect( checkseg );
							cavetetseglist->newindex( (void **)&paryseg );
							*paryseg = checkseg;
						}
					}
				}
			} // j
		}
	} // i

	// Uninfect collected subfaces.
	for(i = 0; i < cavetetshlist->objects; i++)
	{
		parysh = (face *)fastlookup( cavetetshlist, i );
		suninfect( *parysh );
	}
	// Uninfect collected segments.
	for(i = 0; i < cavetetseglist->objects; i++)
	{
		paryseg = (face *)fastlookup( cavetetseglist, i );
		suninfect( *paryseg );
	}

	// Connect subfaces to new tets.
	for(i = 0; i < cavetetshlist->objects; i++)
	{
		parysh = (face *)fastlookup( cavetetshlist, i );
		// Get an adjacent tet at this subface.
		stpivot( *parysh, neightet );
		// Does this tet lie inside the cavity.
		if(infected( neightet ))
		{
			// Yes. Get the other adjacent tet at this subface.
			sesymself( *parysh );
			stpivot( *parysh, neightet );
			// Does this tet lie inside the cavity.
			if(infected( neightet ))
			{
				checksh = *parysh;
				stdissolve( checksh );
				caveencshlist->newindex( (void **)&parysh );
				*parysh = checksh;
			}
		}
		if(!infected( neightet ))
		{
			// Found an outside tet. Re-connect this subface to a new tet.
			fsym( neightet, newtet );
			sesymself( *parysh );
			tsbond( newtet, *parysh );
		}
	} // i


	for(i = 0; i < cavetetseglist->objects; i++)
	{
		checkseg = *(face *)fastlookup( cavetetseglist, i );
		// Check if the segment is inside the cavity.
		sstpivot1( checkseg, neightet );
		spintet = neightet;
		while(1)
		{
			if(!infected( spintet ))
			{
				// This segment is on the boundary of the cavity.
				break;
			}
			fnextself( spintet );
			if(spintet.tet == neightet.tet)
			{
				sstdissolve1( checkseg );
				caveencseglist->newindex( (void **)&paryseg );
				*paryseg = checkseg;
				break;
			}
		}
		if(!infected( spintet ))
		{
			// A boundary segment. Connect this segment to the new tets.
			sstbond1( checkseg, spintet );
			neightet = spintet;
			while(1)
			{
				tssbond1( spintet, checkseg );
				fnextself( spintet );
				if(spintet.tet == neightet.tet) break;
			}
		}
	} // i


	cavetetshlist->restart( );
	cavetetseglist->restart( );

	// Delete the old tets in cavity.
	for(i = 0; i < crosstets->objects; i++)
	{
		parytet = (triface *)fastlookup( crosstets, i );
		if(ishulltet( *parytet ))
		{
			hullsize--;
		}
		tetrahedrondealloc( parytet->tet );
	}

	crosstets->restart( ); // crosstets will be re-used.

	// Collect new tets in cavity.  Some new tets have already been found 
	//   (and infected) in the fillcavity(). We first collect them.
	for(j = 0; j < 2; j++)
	{
		newtets = ( j == 0 ? topnewtets : botnewtets );
		if(newtets != NULL)
		{
			for(i = 0; i < newtets->objects; i++)
			{
				parytet = (triface *)fastlookup( newtets, i );
				if(infected( *parytet ))
				{
					crosstets->newindex( (void **)&pnewtet );
					*pnewtet = *parytet;
				}
			} // i
		}
	} // j

	// Now we collect all new tets in cavity.
	for(i = 0; i < crosstets->objects; i++)
	{
		parytet = (triface *)fastlookup( crosstets, i );
		for(j = 0; j < 4; j++)
		{
			decode( parytet->tet[ j ], neightet );
			if(marktested( neightet ))
			{ // Is it a new tet?
				if(!infected( neightet ))
				{
					// Find an interior tet.
					//assert((point) neightet.tet[7] != dummypoint); // SELF_CHECK
					infect( neightet );
					crosstets->newindex( (void **)&pnewtet );
					*pnewtet = neightet;
				}
			}
		} // j
	} // i

	parytet = (triface *)fastlookup( crosstets, 0 );
	recenttet = *parytet; // Remember a live handle.

	// Delete outer new tets.
	for(j = 0; j < 2; j++)
	{
		newtets = ( j == 0 ? topnewtets : botnewtets );
		if(newtets != NULL)
		{
			for(i = 0; i < newtets->objects; i++)
			{
				parytet = (triface *)fastlookup( newtets, i );
				if(infected( *parytet ))
				{
					// This is an interior tet.
					uninfect( *parytet );
					unmarktest( *parytet );
					if(ishulltet( *parytet ))
					{
						hullsize++;
					}
				}
				else
				{
					// An outer tet. Delete it.
					tetrahedrondealloc( parytet->tet );
				}
			}
		}
	}

	crosstets->restart( );
	topnewtets->restart( );
	if(botnewtets != NULL)
	{
		botnewtets->restart( );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// restorecavity()    Reconnect old tets and delete new tets of the cavity.  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::restorecavity( arraypool *crosstets, arraypool *topnewtets,
								arraypool *botnewtets, arraypool *missingshbds )
{
	triface *parytet, neightet, spintet;
	face *parysh;
	face checkseg;
	point *ppt;
	int t1ver;
	int i, j;

	// Reconnect crossing tets to cavity boundary.
	for(i = 0; i < crosstets->objects; i++)
	{
		parytet = (triface *)fastlookup( crosstets, i );
		parytet->ver = 0;
		for(parytet->ver = 0; parytet->ver < 4; parytet->ver++)
		{
			fsym( *parytet, neightet );
			if(!infected( neightet ))
			{
				// Restore the old connections of tets.
				bond( *parytet, neightet );
			}
		}
		// Update the point-to-tet map.
		parytet->ver = 0;
		ppt = (point *)&( parytet->tet[ 4 ] );
		for(j = 0; j < 4; j++)
		{
			setpoint2tet( ppt[ j ], encode( *parytet ) );
		}
	}

	// Uninfect all crossing tets.
	for(i = 0; i < crosstets->objects; i++)
	{
		parytet = (triface *)fastlookup( crosstets, i );
		uninfect( *parytet );
	}

	// Remember a live handle.
	recenttet = *(triface *)fastlookup( crosstets, 0 );

	// Delete faked segments.
	for(i = 0; i < missingshbds->objects; i++)
	{
		parysh = (face *)fastlookup( missingshbds, i );
		sspivot( *parysh, checkseg );
		if(checkseg.sh[ 3 ] != NULL)
		{
			if(sinfected( checkseg ))
			{
				// It's a faked segment. Delete it.
				sstpivot1( checkseg, neightet );
				spintet = neightet;
				while(1)
				{
					tssdissolve1( spintet );
					fnextself( spintet );
					if(spintet.tet == neightet.tet) break;
				}
				shellfacedealloc( subsegs, checkseg.sh );
				ssdissolve( *parysh );
				//checkseg.sh = NULL;
			}
		}
	} // i

	// Delete new tets.
	for(i = 0; i < topnewtets->objects; i++)
	{
		parytet = (triface *)fastlookup( topnewtets, i );
		tetrahedrondealloc( parytet->tet );
	}

	if(botnewtets != NULL)
	{
		for(i = 0; i < botnewtets->objects; i++)
		{
			parytet = (triface *)fastlookup( botnewtets, i );
			tetrahedrondealloc( parytet->tet );
		}
	}

	crosstets->restart( );
	topnewtets->restart( );
	if(botnewtets != NULL)
	{
		botnewtets->restart( );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// flipcertify()    Insert a crossing face into priority queue.              //
//                                                                           //
// A crossing face of a facet must have at least one top and one bottom ver- //
// tex of the facet.                                                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::flipcertify( triface *chkface, badface **pqueue, point plane_pa,
							  point plane_pb, point plane_pc )
{
	badface *parybf, *prevbf, *nextbf;
	triface neightet;
	face checksh;
	point p[ 5 ];
	REAL w[ 5 ];
	REAL insph, ori4;
	int topi, boti;
	int i;

	// Compute the flip time \tau.
	fsym( *chkface, neightet );

	p[ 0 ] = org( *chkface );
	p[ 1 ] = dest( *chkface );
	p[ 2 ] = apex( *chkface );
	p[ 3 ] = oppo( *chkface );
	p[ 4 ] = oppo( neightet );

	// Check if the face is a crossing face.
	topi = boti = 0;
	for(i = 0; i < 3; i++)
	{
		if(pmarktest2ed( p[ i ] )) topi++;
		if(pmarktest3ed( p[ i ] )) boti++;
	}
	if(( topi == 0 ) || ( boti == 0 ))
	{
		// It is not a crossing face.
		// return;
		for(i = 3; i < 5; i++)
		{
			if(pmarktest2ed( p[ i ] )) topi++;
			if(pmarktest3ed( p[ i ] )) boti++;
		}
		if(( topi == 0 ) || ( boti == 0 ))
		{
			// The two tets sharing at this face are on one side of the facet.
			// Check if this face is locally Delaunay (due to rounding error).
			if(( p[ 3 ] != dummypoint ) && ( p[ 4 ] != dummypoint ))
			{
				// Do not check it if it is a subface.
				tspivot( *chkface, checksh );
				if(checksh.sh == NULL)
				{
					insph = insphere_s( p[ 1 ], p[ 0 ], p[ 2 ], p[ 3 ], p[ 4 ] );
					if(insph > 0)
					{
						// Add the face into queue.
						if(b->verbose > 2)
						{
							myprintf( "      A locally non-Delanay face (%d, %d, %d)-%d,%d\n",
									pointmark( p[ 0 ] ), pointmark( p[ 1 ] ), pointmark( p[ 2 ] ),
									pointmark( p[ 3 ] ), pointmark( p[ 4 ] ) );
						}
						parybf = (badface *)flippool->alloc( );
						parybf->key = 0.;  // tau = 0, do immediately.
						parybf->tt = *chkface;
						parybf->forg = p[ 0 ];
						parybf->fdest = p[ 1 ];
						parybf->fapex = p[ 2 ];
						parybf->foppo = p[ 3 ];
						parybf->noppo = p[ 4 ];
						// Add it at the top of the priority queue.
						if(*pqueue == NULL)
						{
							*pqueue = parybf;
							parybf->nextitem = NULL;
						}
						else
						{
							parybf->nextitem = *pqueue;
							*pqueue = parybf;
						}
					} // if (insph > 0)
				} // if (checksh.sh == NULL)
			}
		}
		return; // Test: omit this face.
	}

	// Decide the "height" for each point.
	for(i = 0; i < 5; i++)
	{
		if(pmarktest2ed( p[ i ] ))
		{
			// A top point has a positive weight.
			w[ i ] = orient3dfast( plane_pa, plane_pb, plane_pc, p[ i ] );
			if(w[ i ] < 0) w[ i ] = -w[ i ];
		}
		else
		{
			w[ i ] = 0;
		}
	}

	insph = insphere( p[ 1 ], p[ 0 ], p[ 2 ], p[ 3 ], p[ 4 ] );
	ori4 = orient4d( p[ 1 ], p[ 0 ], p[ 2 ], p[ 3 ], p[ 4 ], w[ 1 ], w[ 0 ], w[ 2 ], w[ 3 ], w[ 4 ] );
	if(ori4 > 0)
	{
		// Add the face into queue.
		if(b->verbose > 2)
		{
			myprintf( "      Insert face (%d, %d, %d) - %d, %d\n", pointmark( p[ 0 ] ),
					pointmark( p[ 1 ] ), pointmark( p[ 2 ] ), pointmark( p[ 3 ] ), pointmark( p[ 4 ] ) );
		}

		parybf = (badface *)flippool->alloc( );

		parybf->key = -insph / ori4;
		parybf->tt = *chkface;
		parybf->forg = p[ 0 ];
		parybf->fdest = p[ 1 ];
		parybf->fapex = p[ 2 ];
		parybf->foppo = p[ 3 ];
		parybf->noppo = p[ 4 ];

		// Push the face into priority queue.
		//pq.push(bface);
		if(*pqueue == NULL)
		{
			*pqueue = parybf;
			parybf->nextitem = NULL;
		}
		else
		{
			// Search an item whose key is larger or equal to current key.
			prevbf = NULL;
			nextbf = *pqueue;
			//if (!b->flipinsert_random) { // Default use a priority queue.
			// Insert the item into priority queue.
			while(nextbf != NULL)
			{
				if(nextbf->key < parybf->key)
				{
					prevbf = nextbf;
					nextbf = nextbf->nextitem;
				}
				else
				{
					break;
				}
			}
			//} // if (!b->flipinsert_random)
			// Insert the new item between prev and next items.
			if(prevbf == NULL)
			{
				*pqueue = parybf;
			}
			else
			{
				prevbf->nextitem = parybf;
			}
			parybf->nextitem = nextbf;
		}
	}
	else if(ori4 == 0)
	{

	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// flipinsertfacet()    Insert a facet into a CDT by flips.                  //
//                                                                           //
// The algorithm is described in Shewchuk's paper "Updating and Constructing //
// Constrained Delaunay and Constrained Regular Triangulations by Flips", in //
// Proc. 19th Ann. Symp. on Comput. Geom., 86--95, 2003.                     //
//                                                                           //
// 'crosstets' contains the set of crossing tetrahedra (infected) of the     //
// facet.  'toppoints' and 'botpoints' are points lies above and below the   //
// facet, not on the facet.                                                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::flipinsertfacet( arraypool *crosstets, arraypool *toppoints,
								  arraypool *botpoints, arraypool *midpoints )
{
	arraypool *crossfaces, *bfacearray;
	triface fliptets[ 6 ], baktets[ 2 ], fliptet, newface;
	triface neightet, *parytet;
	badface *pqueue;
	badface *popbf, bface;
	point plane_pa, plane_pb, plane_pc;
	point p1, p2, pd, pe;
	point *parypt;
	flipconstraints fc;
	REAL ori[ 3 ];
	int convcount, copcount;
	int flipflag, fcount;
	int n, i;
	long f23count, f32count, f44count;
	long totalfcount;

	f23count = flip23count;
	f32count = flip32count;
	f44count = flip44count;

	// Get three affinely independent vertices in the missing region R.
	calculateabovepoint( midpoints, &plane_pa, &plane_pb, &plane_pc );

	// Mark top and bottom points. Do not mark midpoints.
	for(i = 0; i < toppoints->objects; i++)
	{
		parypt = (point *)fastlookup( toppoints, i );
		if(!pmarktested( *parypt ))
		{
			pmarktest2( *parypt );
		}
	}
	for(i = 0; i < botpoints->objects; i++)
	{
		parypt = (point *)fastlookup( botpoints, i );
		if(!pmarktested( *parypt ))
		{
			pmarktest3( *parypt );
		}
	}

	// Collect crossing faces. 
	crossfaces = cavetetlist;  // Re-use array 'cavetetlist'.

	// Each crossing face contains at least one bottom vertex and
	//   one top vertex.
	for(i = 0; i < crosstets->objects; i++)
	{
		parytet = (triface *)fastlookup( crosstets, i );
		fliptet = *parytet;
		for(fliptet.ver = 0; fliptet.ver < 4; fliptet.ver++)
		{
			fsym( fliptet, neightet );
			if(infected( neightet ))
			{ // It is an interior face.
				if(!marktested( neightet ))
				{ // It is an unprocessed face.
					crossfaces->newindex( (void **)&parytet );
					*parytet = fliptet;
				}
			}
		}
		marktest( fliptet );
	}

	if(b->verbose > 1)
	{
		myprintf( "    Found %ld crossing faces.\n", crossfaces->objects );
	}

	for(i = 0; i < crosstets->objects; i++)
	{
		parytet = (triface *)fastlookup( crosstets, i );
		unmarktest( *parytet );
		uninfect( *parytet );
	}

	// Initialize the priority queue.
	pqueue = NULL;

	for(i = 0; i < crossfaces->objects; i++)
	{
		parytet = (triface *)fastlookup( crossfaces, i );
		flipcertify( parytet, &pqueue, plane_pa, plane_pb, plane_pc );
	}
	crossfaces->restart( );

	// The list for temporarily storing unflipable faces.
	bfacearray = new arraypool( sizeof( triface ), 4 );


	fcount = 0;  // Count the number of flips.

	// Flip insert the facet.
	while(pqueue != NULL)
	{

		// Pop a face from the priority queue.
		popbf = pqueue;
		bface = *popbf;
		// Update the queue.
		pqueue = pqueue->nextitem;
		// Delete the popped item from the pool.
		flippool->dealloc( (void *)popbf );

		if(!isdeadtet( bface.tt ))
		{
			if(( org( bface.tt ) == bface.forg ) && ( dest( bface.tt ) == bface.fdest ) &&
				( apex( bface.tt ) == bface.fapex ) && ( oppo( bface.tt ) == bface.foppo ))
			{
				// It is still a crossing face of R.
				fliptet = bface.tt;
				fsym( fliptet, neightet );
				if(oppo( neightet ) == bface.noppo)
				{
					pd = oppo( fliptet );
					pe = oppo( neightet );

					if(b->verbose > 2)
					{
						myprintf( "      Get face (%d, %d, %d) - %d, %d, tau = %.17g\n",
								pointmark( bface.forg ), pointmark( bface.fdest ),
								pointmark( bface.fapex ), pointmark( bface.foppo ),
								pointmark( bface.noppo ), bface.key );
					}
					flipflag = 0;

					// Check for which type of flip can we do.
					convcount = 3;
					copcount = 0;
					for(i = 0; i < 3; i++)
					{
						p1 = org( fliptet );
						p2 = dest( fliptet );
						ori[ i ] = orient3d( p1, p2, pd, pe );
						if(ori[ i ] < 0)
						{
							convcount--;
							//break;
						}
						else if(ori[ i ] == 0)
						{
							convcount--; // Possible 4-to-4 flip.
							copcount++;
							//break;
						}
						enextself( fliptet );
					}

					if(convcount == 3)
					{
						// A 2-to-3 flip is found.
						fliptets[ 0 ] = fliptet; // abcd, d may be the new vertex.
						fliptets[ 1 ] = neightet; // bace.
						flip23( fliptets, 1, &fc );
						// Put the link faces into check list.
						for(i = 0; i < 3; i++)
						{
							eprevesym( fliptets[ i ], newface );
							crossfaces->newindex( (void **)&parytet );
							*parytet = newface;
						}
						for(i = 0; i < 3; i++)
						{
							enextesym( fliptets[ i ], newface );
							crossfaces->newindex( (void **)&parytet );
							*parytet = newface;
						}
						flipflag = 1;
					}
					else if(convcount == 2)
					{
						//if (copcount <= 1) {
						// A 3-to-2 or 4-to-4 may be possible.
						// Get the edge which is locally non-convex or flat. 
						for(i = 0; i < 3; i++)
						{
							if(ori[ i ] <= 0) break;
							enextself( fliptet );
						}

						// Collect tets sharing at this edge.
						esym( fliptet, fliptets[ 0 ] ); // [b,a,d,c]
						n = 0;
						do
						{
							p1 = apex( fliptets[ n ] );
							if(!( pmarktested( p1 ) || pmarktest2ed( p1 ) || pmarktest3ed( p1 ) ))
							{
								// This apex is not on the cavity. Hence the face does not
								//   lie inside the cavity. Do not flip this edge.
								n = 1000; break;
							}
							fnext( fliptets[ n ], fliptets[ n + 1 ] );
							n++;
						}
						while(( fliptets[ n ].tet != fliptet.tet ) && ( n < 5 ));

						if(n == 3)
						{
							// Found a 3-to-2 flip.
							flip32( fliptets, 1, &fc );
							// Put the link faces into check list.
							for(i = 0; i < 3; i++)
							{
								esym( fliptets[ 0 ], newface );
								crossfaces->newindex( (void **)&parytet );
								*parytet = newface;
								enextself( fliptets[ 0 ] );
							}
							for(i = 0; i < 3; i++)
							{
								esym( fliptets[ 1 ], newface );
								crossfaces->newindex( (void **)&parytet );
								*parytet = newface;
								enextself( fliptets[ 1 ] );
							}
							flipflag = 1;
						}
						else if(n == 4)
						{
							if(copcount == 1)
							{
								// Found a 4-to-4 flip. 
								// Let the six vertices are: a,b,c,d,e,f, where
								//   fliptets[0] = [b,a,d,c]
								//           [1] = [b,a,c,e]
								//           [2] = [b,a,e,f]
								//           [3] = [b,a,f,d]
								// After the 4-to-4 flip, edge [a,b] is flipped, edge [e,d]
								//   is created.
								// First do a 2-to-3 flip.
								// Comment: This flip temporarily creates a degenerated
								//   tet (whose volume is zero). It will be removed by the 
								//   followed 3-to-2 flip.
								fliptets[ 0 ] = fliptet; // = [a,b,c,d], d is the new vertex.
								// fliptets[1];        // = [b,a,c,e].
								baktets[ 0 ] = fliptets[ 2 ]; // = [b,a,e,f]
								baktets[ 1 ] = fliptets[ 3 ]; // = [b,a,f,d]
								// The flip may involve hull tets.
								flip23( fliptets, 1, &fc );
								// Put the "outer" link faces into check list.
								//   fliptets[0] = [e,d,a,b] => will be flipped, so 
								//   [a,b,d] and [a,b,e] are not "outer" link faces.
								for(i = 1; i < 3; i++)
								{
									eprevesym( fliptets[ i ], newface );
									crossfaces->newindex( (void **)&parytet );
									*parytet = newface;
								}
								for(i = 1; i < 3; i++)
								{
									enextesym( fliptets[ i ], newface );
									crossfaces->newindex( (void **)&parytet );
									*parytet = newface;
								}
								// Then do a 3-to-2 flip.
								enextesymself( fliptets[ 0 ] );  // fliptets[0] is [e,d,a,b].
								eprevself( fliptets[ 0 ] ); // = [b,a,d,c], d is the new vertex.
								fliptets[ 1 ] = baktets[ 0 ]; // = [b,a,e,f]
								fliptets[ 2 ] = baktets[ 1 ]; // = [b,a,f,d]
								flip32( fliptets, 1, &fc );
								// Put the "outer" link faces into check list.
								//   fliptets[0] = [d,e,f,a]
								//   fliptets[1] = [e,d,f,b]
								//   Faces [a,b,d] and [a,b,e] are not "outer" link faces.
								enextself( fliptets[ 0 ] );
								for(i = 1; i < 3; i++)
								{
									esym( fliptets[ 0 ], newface );
									crossfaces->newindex( (void **)&parytet );
									*parytet = newface;
									enextself( fliptets[ 0 ] );
								}
								enextself( fliptets[ 1 ] );
								for(i = 1; i < 3; i++)
								{
									esym( fliptets[ 1 ], newface );
									crossfaces->newindex( (void **)&parytet );
									*parytet = newface;
									enextself( fliptets[ 1 ] );
								}
								flip23count--;
								flip32count--;
								flip44count++;
								flipflag = 1;
							}
						}
					}
					else
					{
						// There are more than 1 non-convex or coplanar cases.
						flipflag = -1; // Ignore this face.
						if(b->verbose > 2)
						{
							myprintf( "        Ignore face (%d, %d, %d) - %d, %d, tau = %.17g\n",
									pointmark( bface.forg ), pointmark( bface.fdest ),
									pointmark( bface.fapex ), pointmark( bface.foppo ),
									pointmark( bface.noppo ), bface.key );
						}
					} // if (convcount == 1)

					if(flipflag == 1)
					{
						// Update the priority queue.
						for(i = 0; i < crossfaces->objects; i++)
						{
							parytet = (triface *)fastlookup( crossfaces, i );
							flipcertify( parytet, &pqueue, plane_pa, plane_pb, plane_pc );
						}
						crossfaces->restart( );
						if(1)
						{ // if (!b->flipinsert_random) {
							// Insert all queued unflipped faces.
							for(i = 0; i < bfacearray->objects; i++)
							{
								parytet = (triface *)fastlookup( bfacearray, i );
								// This face may be changed.
								if(!isdeadtet( *parytet ))
								{
									flipcertify( parytet, &pqueue, plane_pa, plane_pb, plane_pc );
								}
							}
							bfacearray->restart( );
						}
						fcount++;
					}
					else if(flipflag == 0)
					{
						// Queue an unflippable face. To process it later.
						bfacearray->newindex( (void **)&parytet );
						*parytet = fliptet;
					}
				} // if (pe == bface.noppo)  
			} // if ((pa == bface.forg) && ...)
		} // if (bface.tt != NULL)

	} // while (pqueue != NULL)

	if(bfacearray->objects > 0)
	{
		if(fcount == 0)
		{
			myprintf( "!! No flip is found in %ld faces.\n", bfacearray->objects );
			terminatetetgen( this, 2 ); //assert(0);
		}
	}

	delete bfacearray;

	// Un-mark top and bottom points.
	for(i = 0; i < toppoints->objects; i++)
	{
		parypt = (point *)fastlookup( toppoints, i );
		punmarktest2( *parypt );
	}
	for(i = 0; i < botpoints->objects; i++)
	{
		parypt = (point *)fastlookup( botpoints, i );
		punmarktest3( *parypt );
	}

	f23count = flip23count - f23count;
	f32count = flip32count - f32count;
	f44count = flip44count - f44count;
	totalfcount = f23count + f32count + f44count;
	if(b->verbose > 2)
	{
		myprintf( "      Total %ld flips. f23(%ld), f32(%ld), f44(%ld).\n",
				totalfcount, f23count, f32count, f44count );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// insertpoint_cdt()    Insert a new point into a CDT.                       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::insertpoint_cdt( point newpt, triface *searchtet, face *splitsh,
								 face *splitseg, insertvertexflags *ivf,
								 arraypool *cavpoints, arraypool *cavfaces,
								 arraypool *cavshells, arraypool *newtets,
								 arraypool *crosstets, arraypool *misfaces )
{
	triface neightet, *parytet;
	face checksh, *parysh, *parysh1;
	face *paryseg, *paryseg1;
	point *parypt;
	int t1ver;
	int i;

	if(b->verbose > 2)
	{
		myprintf( "      Insert point %d into CDT\n", pointmark( newpt ) );
	}

	if(!insertpoint( newpt, searchtet, NULL, NULL, ivf ))
	{
		// Point is not inserted. Check ivf->iloc for reason.
		return 0;
	}


	for(i = 0; i < cavetetvertlist->objects; i++)
	{
		cavpoints->newindex( (void **)&parypt );
		*parypt = *(point *)fastlookup( cavetetvertlist, i );
	}
	// Add the new point into the point list.
	cavpoints->newindex( (void **)&parypt );
	*parypt = newpt;

	for(i = 0; i < cavebdrylist->objects; i++)
	{
		cavfaces->newindex( (void **)&parytet );
		*parytet = *(triface *)fastlookup( cavebdrylist, i );
	}

	for(i = 0; i < caveoldtetlist->objects; i++)
	{
		crosstets->newindex( (void **)&parytet );
		*parytet = *(triface *)fastlookup( caveoldtetlist, i );
	}

	cavetetvertlist->restart( );
	cavebdrylist->restart( );
	caveoldtetlist->restart( );

	// Insert the point using the cavity algorithm.
	delaunizecavity( cavpoints, cavfaces, cavshells, newtets, crosstets,
					 misfaces );
	fillcavity( cavshells, NULL, NULL, NULL, NULL, NULL, NULL );
	carvecavity( crosstets, newtets, NULL );

	if(( splitsh != NULL ) || ( splitseg != NULL ))
	{
		// Insert the point into the surface mesh.
		sinsertvertex( newpt, splitsh, splitseg, ivf->sloc, ivf->sbowywat, 0 );

		// Put all new subfaces into stack.
		for(i = 0; i < caveshbdlist->objects; i++)
		{
			// Get an old subface at edge [a, b].
			parysh = (face *)fastlookup( caveshbdlist, i );
			spivot( *parysh, checksh ); // The new subface [a, b, p].
			// Do not recover a deleted new face (degenerated).
			if(checksh.sh[ 3 ] != NULL)
			{
				subfacstack->newindex( (void **)&parysh );
				*parysh = checksh;
			}
		}

		if(splitseg != NULL)
		{
			// Queue two new subsegments in C(p) for recovery.
			for(i = 0; i < cavesegshlist->objects; i++)
			{
				paryseg = (face *)fastlookup( cavesegshlist, i );
				subsegstack->newindex( (void **)&paryseg1 );
				*paryseg1 = *paryseg;
			}
		} // if (splitseg != NULL)

		// Delete the old subfaces in sC(p).
		for(i = 0; i < caveshlist->objects; i++)
		{
			parysh = (face *)fastlookup( caveshlist, i );
			if(checksubfaceflag)
			{
				// It is possible that this subface still connects to adjacent
				//   tets which are not in C(p). If so, clear connections in the
				//   adjacent tets at this subface.
				stpivot( *parysh, neightet );
				if(neightet.tet != NULL)
				{
					if(neightet.tet[ 4 ] != NULL)
					{
						// Found an adjacent tet. It must be not in C(p).
						tsdissolve( neightet );
						fsymself( neightet );
						tsdissolve( neightet );
					}
				}
			}
			shellfacedealloc( subfaces, parysh->sh );
		}
		if(splitseg != NULL)
		{
			// Delete the old segment in sC(p).
			shellfacedealloc( subsegs, splitseg->sh );
		}

		// Clear working lists.
		caveshlist->restart( );
		caveshbdlist->restart( );
		cavesegshlist->restart( );
	} // if ((splitsh != NULL) || (splitseg != NULL)) 

	// Put all interior subfaces into stack for recovery.
	// They were collected in carvecavity().
	// Note: Some collected subfaces may be deleted by sinsertvertex().
	for(i = 0; i < caveencshlist->objects; i++)
	{
		parysh = (face *)fastlookup( caveencshlist, i );
		if(parysh->sh[ 3 ] != NULL)
		{
			subfacstack->newindex( (void **)&parysh1 );
			*parysh1 = *parysh;
		}
	}

	// Put all interior segments into stack for recovery.
	// They were collected in carvecavity().
	// Note: Some collected segments may be deleted by sinsertvertex().
	for(i = 0; i < caveencseglist->objects; i++)
	{
		paryseg = (face *)fastlookup( caveencseglist, i );
		if(paryseg->sh[ 3 ] != NULL)
		{
			subsegstack->newindex( (void **)&paryseg1 );
			*paryseg1 = *paryseg;
		}
	}

	caveencshlist->restart( );
	caveencseglist->restart( );

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// refineregion()    Refine a missing region by inserting points.            //
//                                                                           //
// 'splitsh' represents an edge of the facet to be split. It must be not a   //
// segment. 
//                                                                           //
// Assumption: The current mesh is a CDT and is convex.                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::refineregion( face &splitsh, arraypool *cavpoints,
							   arraypool *cavfaces, arraypool *cavshells,
							   arraypool *newtets, arraypool *crosstets,
							   arraypool *misfaces )
{
	triface searchtet, spintet;
	face splitseg, *paryseg;
	point steinpt, pa, pb, refpt;
	insertvertexflags ivf;
	enum interresult dir;
	long baknum = points->items;
	int t1ver;
	int i;

	if(b->verbose > 2)
	{
		myprintf( "      Refining region at edge (%d, %d, %d).\n",
				pointmark( sorg( splitsh ) ), pointmark( sdest( splitsh ) ),
				pointmark( sapex( splitsh ) ) );
	}

	// Add the Steiner point at the barycenter of the face.
	pa = sorg( splitsh );
	pb = sdest( splitsh );
	// Create a new point.
	makepoint( &steinpt, FREEFACETVERTEX );
	for(i = 0; i < 3; i++)
	{
		steinpt[ i ] = 0.5 * ( pa[ i ] + pb[ i ] );
	}
	// UV interpolation
	if(b->psc)
	{
		for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
		{
			REAL auv, buv, newuv;
			auv = pointgeomuv( pa, tt );
			buv = pointgeomuv( pb, tt );
			newuv = 0.5 * ( auv + buv );
			setpointgeomuv( steinpt, tt, newuv );
		}
	}

	ivf.bowywat = 1; // Use the Bowyer-Watson algorrithm.
	ivf.cdtflag = 1; // Only create the initial cavity.
	ivf.sloc = (int)ONEDGE;
	ivf.sbowywat = 1;
	ivf.assignmeshsize = b->metric;
	ivf.smlenflag = useinsertradius; // Return the closet mesh vertex.

	point2tetorg( pa, searchtet ); // Start location from it.
	ivf.iloc = (int)OUTSIDE;

	ivf.rejflag = 1; // Reject it if it encroaches upon any segment.
	if(!insertpoint_cdt( steinpt, &searchtet, &splitsh, NULL, &ivf, cavpoints,
		cavfaces, cavshells, newtets, crosstets, misfaces ))
	{
		if(ivf.iloc == (int)ENCSEGMENT)
		{
			pointdealloc( steinpt );
			// Split an encroached segment.
			i = randomnation( encseglist->objects );
			paryseg = (face *)fastlookup( encseglist, i );
			splitseg = *paryseg;
			encseglist->restart( );

			// Split the segment.
			pa = sorg( splitseg );
			pb = sdest( splitseg );
			// Create a new point.
			makepoint( &steinpt, FREESEGVERTEX );
			for(i = 0; i < 3; i++)
			{
				steinpt[ i ] = 0.5 * ( pa[ i ] + pb[ i ] );
			}
			// UV interpolation
			if(b->psc)
			{
				for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
				{
					REAL auv, buv, newuv;
					auv = pointgeomuv( pa, tt );
					buv = pointgeomuv( pb, tt );
					newuv = 0.5 * ( auv + buv );
					setpointgeomuv( steinpt, tt, newuv );
				}
			}

			point2tetorg( pa, searchtet );
			ivf.iloc = (int)OUTSIDE;
			ivf.rejflag = 0;
			if(!insertpoint_cdt( steinpt, &searchtet, &splitsh, &splitseg, &ivf,
				cavpoints, cavfaces, cavshells, newtets,
				crosstets, misfaces ))
			{
				terminatetetgen( this, 2 );
			}
			if(useinsertradius)
			{
				save_segmentpoint_insradius( steinpt, ivf.parentpt, ivf.smlen );
			}
			st_segref_count++;
			if(steinerleft > 0) steinerleft--;
		}
		else
		{
			terminatetetgen( this, 2 ); // assert(0);
		}
	}
	else
	{
		if(useinsertradius)
		{
			save_facetpoint_insradius( steinpt, ivf.parentpt, ivf.smlen );
		}
		st_facref_count++;
		if(steinerleft > 0) steinerleft--;
	}

	while(subsegstack->objects > 0l)
	{
		// seglist is used as a stack.
		subsegstack->objects--;
		paryseg = (face *)fastlookup( subsegstack, subsegstack->objects );
		splitseg = *paryseg;

		// Check if this segment has been recovered.
		sstpivot1( splitseg, searchtet );
		if(searchtet.tet != NULL) continue;

		// Search the segment.
		dir = scoutsegment( sorg( splitseg ), sdest( splitseg ), &splitseg, &searchtet,
							&refpt, NULL );
		if(dir == SHAREEDGE)
		{
			// Found this segment, insert it.
			// Let the segment remember an adjacent tet.
			sstbond1( splitseg, searchtet );
			// Bond the segment to all tets containing it.
			spintet = searchtet;
			do
			{
				tssbond1( spintet, splitseg );
				fnextself( spintet );
			}
			while(spintet.tet != searchtet.tet);
		}
		else
		{
			if(( dir == ACROSSFACE ) || ( dir == ACROSSEDGE ))
			{
				// Split the segment.
				REAL uv[ tetgenio::pointparam::maxuvs ];
				makepoint( &steinpt, FREESEGVERTEX );
				getsteinerptonsegment( &splitseg, refpt, steinpt, uv );
				if(b->psc)
				{
					// UV
					for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
					{
						setpointgeomuv( steinpt, tt, uv[ tt ] );
					}
				}
				ivf.iloc = (int)OUTSIDE;
				ivf.rejflag = 0;
				if(!insertpoint_cdt( steinpt, &searchtet, &splitsh, &splitseg, &ivf,
					cavpoints, cavfaces, cavshells, newtets,
					crosstets, misfaces ))
				{
					terminatetetgen( this, 2 );
				}
				if(useinsertradius)
				{
					save_segmentpoint_insradius( steinpt, ivf.parentpt, ivf.smlen );
				}
				st_segref_count++;
				if(steinerleft > 0) steinerleft--;
			}
			else
			{
				terminatetetgen( this, 2 );
			}
		}
	} // while

	if(b->verbose > 2)
	{
		myprintf( "      Added %ld Steiner points.\n", points->items - baknum );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// constrainedfacets()    Recover constrained facets in a CDT.               //
//                                                                           //
// All unrecovered subfaces are queued in 'subfacestack'.                    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::constrainedfacets( )
{
	arraypool *tg_crosstets, *tg_topnewtets, *tg_botnewtets;
	arraypool *tg_topfaces, *tg_botfaces, *tg_midfaces;
	arraypool *tg_topshells, *tg_botshells, *tg_facfaces;
	arraypool *tg_toppoints, *tg_botpoints;
	arraypool *tg_missingshs, *tg_missingshbds, *tg_missingshverts;
	triface searchtet, neightet, crossedge;
	face searchsh, *parysh, *parysh1;
	face *paryseg;
	point *parypt;
	enum interresult dir;
	int facetcount;
	int success;
	int t1ver;
	int i, j;

	// Initialize arrays.
	tg_crosstets = new arraypool( sizeof( triface ), 10 );
	tg_topnewtets = new arraypool( sizeof( triface ), 10 );
	tg_botnewtets = new arraypool( sizeof( triface ), 10 );
	tg_topfaces = new arraypool( sizeof( triface ), 10 );
	tg_botfaces = new arraypool( sizeof( triface ), 10 );
	tg_midfaces = new arraypool( sizeof( triface ), 10 );
	tg_toppoints = new arraypool( sizeof( point ), 8 );
	tg_botpoints = new arraypool( sizeof( point ), 8 );
	tg_facfaces = new arraypool( sizeof( face ), 10 );
	tg_topshells = new arraypool( sizeof( face ), 10 );
	tg_botshells = new arraypool( sizeof( face ), 10 );
	tg_missingshs = new arraypool( sizeof( face ), 10 );
	tg_missingshbds = new arraypool( sizeof( face ), 10 );
	tg_missingshverts = new arraypool( sizeof( point ), 8 );
	// This is a global array used by refineregion().
	encseglist = new arraypool( sizeof( face ), 4 );

	facetcount = 0;

	while(subfacstack->objects > 0l)
	{

		subfacstack->objects--;
		parysh = (face *)fastlookup( subfacstack, subfacstack->objects );
		searchsh = *parysh;

		if(searchsh.sh[ 3 ] == NULL) continue; // It is dead.
		if(isshtet( searchsh )) continue; // It is recovered.

		// Collect all unrecovered subfaces which are co-facet.
		smarktest( searchsh );
		tg_facfaces->newindex( (void **)&parysh );
		*parysh = searchsh;
		for(i = 0; i < tg_facfaces->objects; i++)
		{
			parysh = (face *)fastlookup( tg_facfaces, i );
			for(j = 0; j < 3; j++)
			{
				if(!isshsubseg( *parysh ))
				{
					spivot( *parysh, searchsh );
					if(!smarktested( searchsh ))
					{
						if(!isshtet( searchsh ))
						{
							smarktest( searchsh );
							tg_facfaces->newindex( (void **)&parysh1 );
							*parysh1 = searchsh;
						}
					}
				}
				senextself( *parysh );
			} // j
		} // i
		// Have found all facet subfaces. Unmark them.
		for(i = 0; i < tg_facfaces->objects; i++)
		{
			parysh = (face *)fastlookup( tg_facfaces, i );
			sunmarktest( *parysh );
		}

		if(b->verbose > 1)
		{
			myprintf( "    Recovering facet #%d: %ld subfaces.\n", facetcount + 1,
					tg_facfaces->objects );
		}
		facetcount++;

		while(tg_facfaces->objects > 0l)
		{

			tg_facfaces->objects--;
			parysh = (face *)fastlookup( tg_facfaces, tg_facfaces->objects );
			searchsh = *parysh;

			if(searchsh.sh[ 3 ] == NULL) continue; // It is dead.
			if(isshtet( searchsh )) continue; // It is recovered.

			searchtet.tet = NULL;
			if(scoutsubface( &searchsh, &searchtet, 1 )) continue;

			// The subface is missing. Form the missing region.
			//   Re-use 'tg_crosstets' for 'adjtets'.
			formregion( &searchsh, tg_missingshs, tg_missingshbds, tg_missingshverts );

			if(scoutcrossedge( searchtet, tg_missingshbds, tg_missingshs ))
			{
				// Save this crossing edge, will be used by fillcavity().
				crossedge = searchtet;
				// Form a cavity of crossing tets.
				success = formcavity( &searchtet, tg_missingshs, tg_crosstets,
									  tg_topfaces, tg_botfaces, tg_toppoints,
									  tg_botpoints );
				if(success)
				{
					if(!b->flipinsert)
					{
						// Tetrahedralize the top part. Re-use 'tg_midfaces'.
						delaunizecavity( tg_toppoints, tg_topfaces, tg_topshells,
										 tg_topnewtets, tg_crosstets, tg_midfaces );
						// Tetrahedralize the bottom part. Re-use 'tg_midfaces'.
						delaunizecavity( tg_botpoints, tg_botfaces, tg_botshells,
										 tg_botnewtets, tg_crosstets, tg_midfaces );
						// Fill the cavity with new tets.
						success = fillcavity( tg_topshells, tg_botshells, tg_midfaces,
											  tg_missingshs, tg_topnewtets, tg_botnewtets,
											  &crossedge );
						if(success)
						{
							// Cavity is remeshed. Delete old tets and outer new tets.
							carvecavity( tg_crosstets, tg_topnewtets, tg_botnewtets );
						}
						else
						{
							restorecavity( tg_crosstets, tg_topnewtets, tg_botnewtets,
										   tg_missingshbds );
						}
					}
					else
					{
						// Use the flip algorithm of Shewchuk to recover the subfaces.
						flipinsertfacet( tg_crosstets, tg_toppoints, tg_botpoints,
										 tg_missingshverts );
						// Put all subfaces in R back to tg_facfaces.
						for(i = 0; i < tg_missingshs->objects; i++)
						{
							parysh = (face *)fastlookup( tg_missingshs, i );
							tg_facfaces->newindex( (void **)&parysh1 );
							*parysh1 = *parysh;
						}
						success = 1;
						// Clear working lists.
						tg_crosstets->restart( );
						tg_topfaces->restart( );
						tg_botfaces->restart( );
						tg_toppoints->restart( );
						tg_botpoints->restart( );
					} // b->flipinsert

					if(success)
					{
						// Recover interior subfaces.
						for(i = 0; i < caveencshlist->objects; i++)
						{
							parysh = (face *)fastlookup( caveencshlist, i );
							if(!scoutsubface( parysh, &searchtet, 1 ))
							{
								// Add this face at the end of the list, so it will be
								//   processed immediately.
								tg_facfaces->newindex( (void **)&parysh1 );
								*parysh1 = *parysh;
							}
						}
						caveencshlist->restart( );
						// Recover interior segments. This should always be recovered.
						for(i = 0; i < caveencseglist->objects; i++)
						{
							paryseg = (face *)fastlookup( caveencseglist, i );
							dir = scoutsegment( sorg( *paryseg ), sdest( *paryseg ), paryseg,
												&searchtet, NULL, NULL );
							if(dir != SHAREEDGE)
							{
								terminatetetgen( this, 2 );
							}
							// Insert this segment.
							// Let the segment remember an adjacent tet.
							sstbond1( *paryseg, searchtet );
							// Bond the segment to all tets containing it.
							neightet = searchtet;
							do
							{
								tssbond1( neightet, *paryseg );
								fnextself( neightet );
							}
							while(neightet.tet != searchtet.tet);
						}
						caveencseglist->restart( );
					} // success - remesh cavity
				} // success - form cavity
			}
			else
			{
				// Put all subfaces in R back to tg_facfaces.
				for(i = 0; i < tg_missingshs->objects; i++)
				{
					parysh = (face *)fastlookup( tg_missingshs, i );
					tg_facfaces->newindex( (void **)&parysh1 );
					*parysh1 = *parysh;
				}
				success = 1;
			} // if (scoutcrossedge)

			// Unmarktest all points of the missing region.
			for(i = 0; i < tg_missingshverts->objects; i++)
			{
				parypt = (point *)fastlookup( tg_missingshverts, i );
				punmarktest( *parypt );
			}
			tg_missingshverts->restart( );
			tg_missingshbds->restart( );
			tg_missingshs->restart( );

			if(!success)
			{
				// The missing region can not be recovered. Refine it.
				refineregion( recentsh, tg_toppoints, tg_topfaces, tg_topshells,
							  tg_topnewtets, tg_crosstets, tg_midfaces );
			}
		} // while (tg_facfaces->objects)

	} // while ((subfacstack->objects)

	// Accumulate the dynamic memory.
	totalworkmemory += ( tg_crosstets->totalmemory + tg_topnewtets->totalmemory +
						 tg_botnewtets->totalmemory + tg_topfaces->totalmemory +
						 tg_botfaces->totalmemory + tg_midfaces->totalmemory +
						 tg_toppoints->totalmemory + tg_botpoints->totalmemory +
						 tg_facfaces->totalmemory + tg_topshells->totalmemory +
						 tg_botshells->totalmemory + tg_missingshs->totalmemory +
						 tg_missingshbds->totalmemory +
						 tg_missingshverts->totalmemory +
						 encseglist->totalmemory );

	// Delete arrays.
	delete tg_crosstets;
	delete tg_topnewtets;
	delete tg_botnewtets;
	delete tg_topfaces;
	delete tg_botfaces;
	delete tg_midfaces;
	delete tg_toppoints;
	delete tg_botpoints;
	delete tg_facfaces;
	delete tg_topshells;
	delete tg_botshells;
	delete tg_missingshs;
	delete tg_missingshbds;
	delete tg_missingshverts;
	delete encseglist;
	encseglist = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// constraineddelaunay()    Create a constrained Delaunay tetrahedralization.//
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::constraineddelaunay( clock_t& tv )
{
	face searchsh, *parysh;
	face searchseg, *paryseg;
	int s, i;

	// Statistics.
	long bakfillregioncount;
	long bakcavitycount, bakcavityexpcount;
	long bakseg_ref_count;

	if(!b->quiet)
	{
		myprintf( "Constrained Delaunay...\n" );
	}

	makesegmentendpointsmap( );
	makefacetverticesmap( );

	if(b->verbose)
	{
		myprintf( "  Delaunizing segments.\n" );
	}

	checksubsegflag = 1;

	// Put all segments into the list (in random order).
	subsegs->traversalinit( );
	for(i = 0; i < subsegs->items; i++)
	{
		s = randomnation( i + 1 );
		// Move the s-th seg to the i-th.
		subsegstack->newindex( (void **)&paryseg );
		*paryseg = *(face *)fastlookup( subsegstack, s );
		// Put i-th seg to be the s-th.
		searchseg.sh = shellfacetraverse( subsegs );
		//sinfect(searchseg);  // Only save it once.
		paryseg = (face *)fastlookup( subsegstack, s );
		*paryseg = searchseg;
	}

	// Recover non-Delaunay segments.
	delaunizesegments( );

	if(b->verbose)
	{
		myprintf( "  Inserted %ld Steiner points.\n", st_segref_count );
	}

	tv = clock( );

	if(b->verbose)
	{
		myprintf( "  Constraining facets.\n" );
	}

	// Subfaces will be introduced.
	checksubfaceflag = 1;

	bakfillregioncount = fillregioncount;
	bakcavitycount = cavitycount;
	bakcavityexpcount = cavityexpcount;
	bakseg_ref_count = st_segref_count;

	// Randomly order the subfaces.
	subfaces->traversalinit( );
	for(i = 0; i < subfaces->items; i++)
	{
		s = randomnation( i + 1 );
		// Move the s-th subface to the i-th.
		subfacstack->newindex( (void **)&parysh );
		*parysh = *(face *)fastlookup( subfacstack, s );
		// Put i-th subface to be the s-th.
		searchsh.sh = shellfacetraverse( subfaces );
		parysh = (face *)fastlookup( subfacstack, s );
		*parysh = searchsh;
	}

	// Recover facets.
	constrainedfacets( );

	if(b->verbose)
	{
		if(fillregioncount > bakfillregioncount)
		{
			myprintf( "  Remeshed %ld regions.\n", fillregioncount - bakfillregioncount );
		}
		if(cavitycount > bakcavitycount)
		{
			myprintf( "  Remeshed %ld cavities", cavitycount - bakcavitycount );
			if(cavityexpcount - bakcavityexpcount)
			{
				myprintf( " (%ld enlarged)", cavityexpcount - bakcavityexpcount );
			}
			myprintf( ".\n" );
		}
		if(st_segref_count + st_facref_count - bakseg_ref_count > 0)
		{
			myprintf( "  Inserted %ld (%ld, %ld) refine points.\n",
					st_segref_count + st_facref_count - bakseg_ref_count,
					st_segref_count - bakseg_ref_count, st_facref_count );
		}
	}
}

////                                                                       ////
////                                                                       ////
//// constrained_cxx //////////////////////////////////////////////////////////

//// steiner_cxx //////////////////////////////////////////////////////////////
////                                                                       ////
////                                                                       ////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// checkflipeligibility()    A call back function for boundary recovery.     //
//                                                                           //
// 'fliptype' indicates which elementary flip will be performed: 1 : 2-to-3, //
// and 2 : 3-to-2, respectively.                                             //
//                                                                           //
// 'pa, ..., pe' are the vertices involved in this flip, where [a,b,c] is    //
// the flip face, and [d,e] is the flip edge. NOTE: 'pc' may be 'dummypoint',//
// other points must not be 'dummypoint'.                                    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::checkflipeligibility( int fliptype, point pa, point pb,
									  point pc, point pd, point pe,
									  int level, int edgepivot,
									  flipconstraints* fc )
{
	point tmppts[ 3 ];
	enum interresult dir;
	int types[ 2 ], poss[ 4 ];
	int intflag;
	int rejflag = 0;
	int i;

	if(fc->seg[ 0 ] != NULL)
	{
		// A constraining edge is given (e.g., for edge recovery).
		if(fliptype == 1)
		{
			// A 2-to-3 flip: [a,b,c] => [e,d,a], [e,d,b], [e,d,c].
			tmppts[ 0 ] = pa;
			tmppts[ 1 ] = pb;
			tmppts[ 2 ] = pc;
			for(i = 0; i < 3 && !rejflag; i++)
			{
				if(tmppts[ i ] != dummypoint)
				{
					// Test if the face [e,d,#] intersects the edge.
					intflag = tri_edge_test( pe, pd, tmppts[ i ], fc->seg[ 0 ], fc->seg[ 1 ],
											 NULL, 1, types, poss );
					if(intflag == 2)
					{
						// They intersect at a single point.
						dir = ( enum interresult ) types[ 0 ];
						if(dir == ACROSSFACE)
						{
							// The interior of [e,d,#] intersect the segment.
							rejflag = 1;
						}
						else if(dir == ACROSSEDGE)
						{
							if(poss[ 0 ] == 0)
							{
								// The interior of [e,d] intersect the segment.
								// Since [e,d] is the newly created edge. Reject this flip.
								rejflag = 1;
							}
						}
					}
					else if(intflag == 4)
					{
						// They may intersect at either a point or a line segment.
						dir = ( enum interresult ) types[ 0 ];
						if(dir == ACROSSEDGE)
						{
							if(poss[ 0 ] == 0)
							{
								// The interior of [e,d] intersect the segment.
								// Since [e,d] is the newly created edge. Reject this flip.
								rejflag = 1;
							}
						}
					}
				} // if (tmppts[0] != dummypoint)
			} // i
		}
		else if(fliptype == 2)
		{
			// A 3-to-2 flip: [e,d,a], [e,d,b], [e,d,c] => [a,b,c]
			if(pc != dummypoint)
			{
				// Check if the new face [a,b,c] intersect the edge in its interior.
				intflag = tri_edge_test( pa, pb, pc, fc->seg[ 0 ], fc->seg[ 1 ], NULL,
										 1, types, poss );
				if(intflag == 2)
				{
					// They intersect at a single point.
					dir = ( enum interresult ) types[ 0 ];
					if(dir == ACROSSFACE)
					{
						// The interior of [a,b,c] intersect the segment.
						rejflag = 1; // Do not flip.
					}
				}
				else if(intflag == 4)
				{
					// [a,b,c] is coplanar with the edge. 
					dir = ( enum interresult ) types[ 0 ];
					if(dir == ACROSSEDGE)
					{
						// The boundary of [a,b,c] intersect the segment.            
						rejflag = 1; // Do not flip.
					}
				}
			} // if (pc != dummypoint)
		}
	} // if (fc->seg[0] != NULL)

	if(( fc->fac[ 0 ] != NULL ) && !rejflag)
	{
		// A constraining face is given (e.g., for face recovery).
		if(fliptype == 1)
		{
			// A 2-to-3 flip.
			// Test if the new edge [e,d] intersects the face.
			intflag = tri_edge_test( fc->fac[ 0 ], fc->fac[ 1 ], fc->fac[ 2 ], pe, pd,
									 NULL, 1, types, poss );
			if(intflag == 2)
			{
				// They intersect at a single point.
				dir = ( enum interresult ) types[ 0 ];
				if(dir == ACROSSFACE)
				{
					rejflag = 1;
				}
				else if(dir == ACROSSEDGE)
				{
					rejflag = 1;
				}
			}
			else if(intflag == 4)
			{
				// The edge [e,d] is coplanar with the face.
				// There may be two intersections.
				for(i = 0; i < 2 && !rejflag; i++)
				{
					dir = ( enum interresult ) types[ i ];
					if(dir == ACROSSFACE)
					{
						rejflag = 1;
					}
					else if(dir == ACROSSEDGE)
					{
						rejflag = 1;
					}
				}
			}
		} // if (fliptype == 1)
	} // if (fc->fac[0] != NULL)

	if(( fc->remvert != NULL ) && !rejflag)
	{
		// The vertex is going to be removed. Do not create a new edge which
		//   contains this vertex.
		if(fliptype == 1)
		{
			// A 2-to-3 flip.
			if(( pd == fc->remvert ) || ( pe == fc->remvert ))
			{
				rejflag = 1;
			}
		}
	}

	if(fc->remove_large_angle && !rejflag)
	{
		// Remove a large dihedral angle. Do not create a new small angle.
		REAL cosmaxd = 0, diff;
		if(fliptype == 1)
		{
			// We assume that neither 'a' nor 'b' is dummypoint.
			// A 2-to-3 flip: [a,b,c] => [e,d,a], [e,d,b], [e,d,c].
			// The new tet [e,d,a,b] will be flipped later. Only two new tets:
			//   [e,d,b,c] and [e,d,c,a] need to be checked.
			if(( pc != dummypoint ) && ( pe != dummypoint ) && ( pd != dummypoint ))
			{
				// Get the largest dihedral angle of [e,d,b,c].
				tetalldihedral( pe, pd, pb, pc, NULL, &cosmaxd, NULL );
				diff = cosmaxd - fc->cosdihed_in;
				if(fabs( diff / fc->cosdihed_in ) < b->epsilon) diff = 0.0; // Rounding.
				if(diff <= 0)
				{ //if (cosmaxd <= fc->cosdihed_in) {
					rejflag = 1;
				}
				else
				{
					// Record the largest new angle.
					if(cosmaxd < fc->cosdihed_out)
					{
						fc->cosdihed_out = cosmaxd;
					}
					// Get the largest dihedral angle of [e,d,c,a].
					tetalldihedral( pe, pd, pc, pa, NULL, &cosmaxd, NULL );
					diff = cosmaxd - fc->cosdihed_in;
					if(fabs( diff / fc->cosdihed_in ) < b->epsilon) diff = 0.0; // Rounding.
					if(diff <= 0)
					{ //if (cosmaxd <= fc->cosdihed_in) {
						rejflag = 1;
					}
					else
					{
						// Record the largest new angle.
						if(cosmaxd < fc->cosdihed_out)
						{
							fc->cosdihed_out = cosmaxd;
						}
					}
				}
			} // if (pc != dummypoint && ...)
		}
		else if(fliptype == 2)
		{
			// A 3-to-2 flip: [e,d,a], [e,d,b], [e,d,c] => [a,b,c]
			// We assume that neither 'e' nor 'd' is dummypoint.
			if(level == 0)
			{
				// Both new tets [a,b,c,d] and [b,a,c,e] are new tets.
				if(( pa != dummypoint ) && ( pb != dummypoint ) && ( pc != dummypoint ))
				{
					// Get the largest dihedral angle of [a,b,c,d].
					tetalldihedral( pa, pb, pc, pd, NULL, &cosmaxd, NULL );
					diff = cosmaxd - fc->cosdihed_in;
					if(fabs( diff / fc->cosdihed_in ) < b->epsilon) diff = 0.0; // Rounding
					if(diff <= 0)
					{ //if (cosmaxd <= fc->cosdihed_in) {
						rejflag = 1;
					}
					else
					{
						// Record the largest new angle.
						if(cosmaxd < fc->cosdihed_out)
						{
							fc->cosdihed_out = cosmaxd;
						}
						// Get the largest dihedral angle of [b,a,c,e].
						tetalldihedral( pb, pa, pc, pe, NULL, &cosmaxd, NULL );
						diff = cosmaxd - fc->cosdihed_in;
						if(fabs( diff / fc->cosdihed_in ) < b->epsilon) diff = 0.0;// Rounding
						if(diff <= 0)
						{ //if (cosmaxd <= fc->cosdihed_in) {
							rejflag = 1;
						}
						else
						{
							// Record the largest new angle.
							if(cosmaxd < fc->cosdihed_out)
							{
								fc->cosdihed_out = cosmaxd;
							}
						}
					}
				}
			}
			else
			{ // level > 0
				if(edgepivot == 1)
				{
					// The new tet [a,b,c,d] will be flipped. Only check [b,a,c,e].
					if(( pa != dummypoint ) && ( pb != dummypoint ) && ( pc != dummypoint ))
					{
						// Get the largest dihedral angle of [b,a,c,e].
						tetalldihedral( pb, pa, pc, pe, NULL, &cosmaxd, NULL );
						diff = cosmaxd - fc->cosdihed_in;
						if(fabs( diff / fc->cosdihed_in ) < b->epsilon) diff = 0.0;// Rounding
						if(diff <= 0)
						{ //if (cosmaxd <= fc->cosdihed_in) {
							rejflag = 1;
						}
						else
						{
							// Record the largest new angle.
							if(cosmaxd < fc->cosdihed_out)
							{
								fc->cosdihed_out = cosmaxd;
							}
						}
					}
				}
				else
				{
					// The new tet [b,a,c,e] will be flipped. Only check [a,b,c,d].
					if(( pa != dummypoint ) && ( pb != dummypoint ) && ( pc != dummypoint ))
					{
						// Get the largest dihedral angle of [b,a,c,e].
						tetalldihedral( pa, pb, pc, pd, NULL, &cosmaxd, NULL );
						diff = cosmaxd - fc->cosdihed_in;
						if(fabs( diff / fc->cosdihed_in ) < b->epsilon) diff = 0.0;// Rounding
						if(diff <= 0)
						{ //if (cosmaxd <= fc->cosdihed_in) {
							rejflag = 1;
						}
						else
						{
							// Record the largest new angle.
							if(cosmaxd < fc->cosdihed_out)
							{
								fc->cosdihed_out = cosmaxd;
							}
						}
					}
				} // edgepivot
			} // level
		}
	}

	return rejflag;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// removeedgebyflips()    Attempt to remove an edge by flips.                //
//                                                                           //
// 'flipedge' is a non-convex or flat edge [a,b,#,#] to be removed.          //
//                                                                           //
// The return value is a positive integer, it indicates whether the edge is  //
// removed or not.  A value "2" means the edge is removed, otherwise, the    //
// edge is not removed and the value (must >= 3) is the current number of    //
// tets in the edge star.                                                    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::removeedgebyflips( triface *flipedge, flipconstraints* fc )
{
	triface *abtets, spintet;
	int t1ver;
	int n, nn, i;


	if(checksubsegflag)
	{
		// Do not flip a segment.
		if(issubseg( *flipedge ))
		{
			if(fc->collectencsegflag)
			{
				face checkseg, *paryseg;
				tsspivot1( *flipedge, checkseg );
				if(!sinfected( checkseg ))
				{
					// Queue this segment in list.
					sinfect( checkseg );
					caveencseglist->newindex( (void **)&paryseg );
					*paryseg = checkseg;
				}
			}
			return 0;
		}
	}

	// Count the number of tets at edge [a,b].
	n = 0;
	spintet = *flipedge;
	while(1)
	{
		n++;
		fnextself( spintet );
		if(spintet.tet == flipedge->tet) break;
	}
	if(n < 3)
	{
		// It is only possible when the mesh contains inverted tetrahedra.  
		terminatetetgen( this, 2 ); // Report a bug
	}

	if(( b->flipstarsize > 0 ) && ( n > b->flipstarsize ))
	{
		// The star size exceeds the limit.
		return 0; // Do not flip it.
	}

	// Allocate spaces.
	abtets = new triface[ n ];
	// Collect the tets at edge [a,b].
	spintet = *flipedge;
	i = 0;
	while(1)
	{
		abtets[ i ] = spintet;
		setelemcounter( abtets[ i ], 1 );
		i++;
		fnextself( spintet );
		if(spintet.tet == flipedge->tet) break;
	}


	// Try to flip the edge (level = 0, edgepivot = 0).
	nn = flipnm( abtets, n, 0, 0, fc );


	if(nn > 2)
	{
		// Edge is not flipped. Unmarktest the remaining tets in Star(ab).
		for(i = 0; i < nn; i++)
		{
			setelemcounter( abtets[ i ], 0 );
		}
		// Restore the input edge (needed by Lawson's flip).
		*flipedge = abtets[ 0 ];
	}

	// Release the temporary allocated spaces.
	// NOTE: fc->unflip must be 0.
	int bakunflip = fc->unflip;
	fc->unflip = 0;
	flipnm_post( abtets, n, nn, 0, fc );
	fc->unflip = bakunflip;

	delete[ ] abtets;

	return nn;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// removefacebyflips()    Remove a face by flips.                            //
//                                                                           //
// Return 1 if the face is removed. Otherwise, return 0.                     //
//                                                                           //
// ASSUMPTIONS:                                                              //
//   - 'flipface' must not be a subface.                                     //
//   - 'flipface' must not be a hull face.                                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::removefacebyflips( triface *flipface, flipconstraints* fc )
{
	triface fliptets[ 3 ], flipedge;
	point pa, pb, pc, pd, pe;
	REAL ori;
	int reducflag = 0;

	fliptets[ 0 ] = *flipface;
	fsym( *flipface, fliptets[ 1 ] );
	pa = org( fliptets[ 0 ] );
	pb = dest( fliptets[ 0 ] );
	pc = apex( fliptets[ 0 ] );
	pd = oppo( fliptets[ 0 ] );
	pe = oppo( fliptets[ 1 ] );

	ori = orient3d( pa, pb, pd, pe );
	if(ori > 0)
	{
		ori = orient3d( pb, pc, pd, pe );
		if(ori > 0)
		{
			ori = orient3d( pc, pa, pd, pe );
			if(ori > 0)
			{
				// Found a 2-to-3 flip.
				reducflag = 1;
			}
			else
			{
				eprev( *flipface, flipedge ); // [c,a]
			}
		}
		else
		{
			enext( *flipface, flipedge ); // [b,c]
		}
	}
	else
	{
		flipedge = *flipface; // [a,b]
	}

	if(reducflag)
	{
		// A 2-to-3 flip is found.
		flip23( fliptets, 0, fc );
		return 1;
	}
	else
	{
		// Try to flip the selected edge of this face.
		if(removeedgebyflips( &flipedge, fc ) == 2)
		{
			return 1;
		}
	}

	// Face is not removed.
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// recoveredge()    Recover an edge in current tetrahedralization.           //
//                                                                           //
// If the edge is recovered, 'searchtet' returns a tet containing the edge.  //
//                                                                           //
// This edge may intersect a set of faces and edges in the mesh.  All these  //
// faces or edges are needed to be removed.                                  //
//                                                                           //
// If the parameter 'fullsearch' is set, it tries to flip any face or edge   //
// that intersects the recovering edge.  Otherwise, only the face or edge    //
// which is visible by 'startpt' is tried.                                   //
//                                                                           //
// The parameter 'sedge' is used to report self-intersection. If it is not   //
// a NULL, it is EITHER a segment OR a subface that contains this edge.      //
//                                                                           //
// Note that this routine assumes that the tetrahedralization is convex.     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::recoveredgebyflips( point startpt, point endpt, face *sedge,
									triface* searchtet, int fullsearch )
{
	flipconstraints fc;
	enum interresult dir;

	fc.seg[ 0 ] = startpt;
	fc.seg[ 1 ] = endpt;
	fc.checkflipeligibility = 1;

	// The mainloop of the edge reocvery.
	while(1)
	{ // Loop I

		// Search the edge from 'startpt'.
		point2tetorg( startpt, *searchtet );
		dir = finddirection( searchtet, endpt );
		if(dir == ACROSSVERT)
		{
			if(dest( *searchtet ) == endpt)
			{
				return 1; // Edge is recovered.
			}
			else
			{
				if(sedge)
				{
					return report_selfint_edge( startpt, endpt, sedge, searchtet, dir );
				}
				else
				{
					return 0;
				}
			}
		}

		// The edge is missing. 

		// Try to remove the first intersecting face/edge.
		enextesymself( *searchtet ); // Go to the opposite face.
		if(dir == ACROSSFACE)
		{
			if(checksubfaceflag)
			{
				if(issubface( *searchtet ))
				{
					if(sedge)
					{
						return report_selfint_edge( startpt, endpt, sedge, searchtet, dir );
					}
					else
					{
						return 0; // Cannot flip a subface.
					}
				}
			}
			// Try to flip a crossing face.
			if(removefacebyflips( searchtet, &fc ))
			{
				continue;
			}
		}
		else if(dir == ACROSSEDGE)
		{
			if(checksubsegflag)
			{
				if(issubseg( *searchtet ))
				{
					if(sedge)
					{
						return report_selfint_edge( startpt, endpt, sedge, searchtet, dir );
					}
					else
					{
						return 0; // Cannot flip a segment.
					}
				}
			}
			// Try to flip an intersecting edge.
			if(removeedgebyflips( searchtet, &fc ) == 2)
			{
				continue;
			}
		}

		// The edge is missing.

		if(fullsearch)
		{
			// Try to flip one of the faces/edges which intersects the edge.
			triface neightet, spintet;
			point pa, pb, pc, pd;
			badface bakface;
			enum interresult dir1;
			int types[ 2 ], poss[ 4 ], pos = 0;
			int success = 0;
			int t1ver;
			int i, j;

			// Loop through the sequence of intersecting faces/edges from
			//   'startpt' to 'endpt'.
			point2tetorg( startpt, *searchtet );
			dir = finddirection( searchtet, endpt );

			// Go to the face/edge intersecting the searching edge.
			enextesymself( *searchtet ); // Go to the opposite face.
			// This face/edge has been tried in previous step.

			while(1)
			{ // Loop I-I

				// Find the next intersecting face/edge.
				fsymself( *searchtet );
				if(dir == ACROSSFACE)
				{
					neightet = *searchtet;
					j = ( neightet.ver & 3 ); // j is the current face number.
					for(i = j + 1; i < j + 4; i++)
					{
						neightet.ver = ( i % 4 );
						pa = org( neightet );
						pb = dest( neightet );
						pc = apex( neightet );
						pd = oppo( neightet ); // The above point.
						if(tri_edge_test( pa, pb, pc, startpt, endpt, pd, 1, types, poss ))
						{
							dir = ( enum interresult ) types[ 0 ];
							pos = poss[ 0 ];
							break;
						}
						else
						{
							dir = DISJOINT;
							pos = 0;
						}
					} // i
					// There must be an intersection face/edge.
					if(dir == DISJOINT)
					{
						terminatetetgen( this, 2 );
					}
				}
				else if(dir == ACROSSEDGE)
				{
					while(1)
					{ // Loop I-I-I
						// Check the two opposite faces (of the edge) in 'searchtet'.  
						for(i = 0; i < 2; i++)
						{
							if(i == 0)
							{
								enextesym( *searchtet, neightet );
							}
							else
							{
								eprevesym( *searchtet, neightet );
							}
							pa = org( neightet );
							pb = dest( neightet );
							pc = apex( neightet );
							pd = oppo( neightet ); // The above point.
							if(tri_edge_test( pa, pb, pc, startpt, endpt, pd, 1, types, poss ))
							{
								dir = ( enum interresult ) types[ 0 ];
								pos = poss[ 0 ];
								break; // for loop
							}
							else
							{
								dir = DISJOINT;
								pos = 0;
							}
						} // i
						if(dir != DISJOINT)
						{
							// Find an intersection face/edge.
							break;  // Loop I-I-I
						}
						// No intersection. Rotate to the next tet at the edge.
						fnextself( *searchtet );
					} // while (1) // Loop I-I-I
				}
				else
				{
					terminatetetgen( this, 2 ); // Report a bug
				}

				// Adjust to the intersecting edge/vertex.
				for(i = 0; i < pos; i++)
				{
					enextself( neightet );
				}

				if(dir == SHAREVERT)
				{
					// Check if we have reached the 'endpt'.
					pd = org( neightet );
					if(pd == endpt)
					{
						// Failed to recover the edge.
						break; // Loop I-I
					}
					else
					{
						terminatetetgen( this, 2 ); // Report a bug
					}
				}

				// The next to be flipped face/edge.
				*searchtet = neightet;

				// Bakup this face (tetrahedron).
				bakface.forg = org( *searchtet );
				bakface.fdest = dest( *searchtet );
				bakface.fapex = apex( *searchtet );
				bakface.foppo = oppo( *searchtet );

				// Try to flip this intersecting face/edge.
				if(dir == ACROSSFACE)
				{
					if(checksubfaceflag)
					{
						if(issubface( *searchtet ))
						{
							if(sedge)
							{
								return report_selfint_edge( startpt, endpt, sedge, searchtet, dir );
							}
							else
							{
								return 0; // Cannot flip a subface.
							}
						}
					}
					if(removefacebyflips( searchtet, &fc ))
					{
						success = 1;
						break; // Loop I-I 
					}
				}
				else if(dir == ACROSSEDGE)
				{
					if(checksubsegflag)
					{
						if(issubseg( *searchtet ))
						{
							if(sedge)
							{
								return report_selfint_edge( startpt, endpt, sedge, searchtet, dir );
							}
							else
							{
								return 0; // Cannot flip a segment.
							}
						}
					}
					if(removeedgebyflips( searchtet, &fc ) == 2)
					{
						success = 1;
						break; // Loop I-I
					}
				}
				else if(dir == ACROSSVERT)
				{
					if(sedge)
					{
						//return report_selfint_edge(startpt, endpt, sedge, searchtet, dir);
						terminatetetgen( this, 2 );
					}
					else
					{
						return 0;
					}
				}
				else
				{
					terminatetetgen( this, 2 );
				}

				// The face/edge is not flipped.
				if(( searchtet->tet == NULL ) ||
					( org( *searchtet ) != bakface.forg ) ||
					( dest( *searchtet ) != bakface.fdest ) ||
					( apex( *searchtet ) != bakface.fapex ) ||
					( oppo( *searchtet ) != bakface.foppo ))
				{
					// 'searchtet' was flipped. We must restore it.
					point2tetorg( bakface.forg, *searchtet );
					dir1 = finddirection( searchtet, bakface.fdest );
					if(dir1 == ACROSSVERT)
					{
						if(dest( *searchtet ) == bakface.fdest)
						{
							spintet = *searchtet;
							while(1)
							{
								if(apex( spintet ) == bakface.fapex)
								{
									// Found the face.
									*searchtet = spintet;
									break;
								}
								fnextself( spintet );
								if(spintet.tet == searchtet->tet)
								{
									searchtet->tet = NULL;
									break; // Not find.
								}
							} // while (1)
							if(searchtet->tet != NULL)
							{
								if(oppo( *searchtet ) != bakface.foppo)
								{
									fsymself( *searchtet );
									if(oppo( *searchtet ) != bakface.foppo)
									{
										// The original (intersecting) tet has been flipped.
										searchtet->tet = NULL;
										break; // Not find.
									}
								}
							}
						}
						else
						{
							searchtet->tet = NULL; // Not find.
						}
					}
					else
					{
						searchtet->tet = NULL; // Not find.
					}
					if(searchtet->tet == NULL)
					{
						success = 0; // This face/edge has been destroyed.
						break; // Loop I-I 
					}
				}
			} // while (1) // Loop I-I

			if(success)
			{
				// One of intersecting faces/edges is flipped.
				continue;
			}

		} // if (fullsearch)

		// The edge is missing.
		break; // Loop I

	} // while (1) // Loop I

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// add_steinerpt_in_schoenhardtpoly()    Insert a Steiner point in a Schoen- //
//                                       hardt polyhedron.                   //
//                                                                           //
// 'abtets' is an array of n tets which all share at the edge [a,b]. Let the //
// tets are [a,b,p0,p1], [a,b,p1,p2], ..., [a,b,p_(n-2),p_(n-1)].  Moreover, //
// the edge [p0,p_(n-1)] intersects all of the tets in 'abtets'.  A special  //
// case is that the edge [p0,p_(n-1)] is coplanar with the edge [a,b].       //
// Such set of tets arises when we want to recover an edge from 'p0' to 'p_  //
// (n-1)', and the number of tets at [a,b] can not be reduced by any flip.   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::add_steinerpt_in_schoenhardtpoly( triface *abtets, int n,
												  int chkencflag )
{
	triface worktet, *parytet;
	triface faketet1, faketet2;
	point pc, pd, steinerpt;
	insertvertexflags ivf;
	optparameters opm;
	REAL vcd[ 3 ], sampt[ 3 ], smtpt[ 3 ];
	REAL maxminvol = 0.0, minvol = 0.0, ori;
	int success, maxidx = 0;
	int it, i;
	REAL vuv[ tetgenio::pointparam::maxuvs ], cuv[ tetgenio::pointparam::maxuvs ], duv[ tetgenio::pointparam::maxuvs ], samuv[ tetgenio::pointparam::maxuvs ], smtuv[ tetgenio::pointparam::maxuvs ];

	pc = apex( abtets[ 0 ] );   // pc = p0
	pd = oppo( abtets[ n - 1 ] ); // pd = p_(n-1)


	// Find an optimial point in edge [c,d]. It is visible by all outer faces
	//   of 'abtets', and it maxmizes the min volume.

	// initialize the list of 2n boundary faces.
	for(i = 0; i < n; i++)
	{
		edestoppo( abtets[ i ], worktet ); // [p_i,p_i+1,a]
		cavetetlist->newindex( (void **)&parytet );
		*parytet = worktet;
		eorgoppo( abtets[ i ], worktet );  // [p_i+1,p_i,b]
		cavetetlist->newindex( (void **)&parytet );
		*parytet = worktet;
	}

	int N = 100;
	REAL stepi = 0.01;

	// Search the point along the edge [c,d].
	for(i = 0; i < 3; i++) vcd[ i ] = pd[ i ] - pc[ i ];

	// UV
	for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
	{
		cuv[ tt ] = pointgeomuv( pc, tt );
		duv[ tt ] = pointgeomuv( pd, tt );
		vuv[ tt ] = duv[ tt ] - cuv[ tt ];
	}

	// Sample N points in edge [c,d].
	for(it = 1; it < N; it++)
	{
		for(i = 0; i < 3; i++)
		{
			sampt[ i ] = pc[ i ] + ( stepi * (double)it ) * vcd[ i ];
		}
		// UV
		for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
		{
			samuv[ tt ] = cuv[ tt ] + ( stepi * (double)it ) * vuv[ tt ];
		}
		for(i = 0; i < cavetetlist->objects; i++)
		{
			parytet = (triface *)fastlookup( cavetetlist, i );
			ori = orient3d( dest( *parytet ), org( *parytet ), apex( *parytet ), sampt );
			if(i == 0)
			{
				minvol = ori;
			}
			else
			{
				if(minvol > ori) minvol = ori;
			}
		} // i
		if(it == 1)
		{
			maxminvol = minvol;
			maxidx = it;
		}
		else
		{
			if(maxminvol < minvol)
			{
				maxminvol = minvol;
				maxidx = it;
			}
		}
	} // it

	if(maxminvol <= 0)
	{
		cavetetlist->restart( );
		return 0;
	}

	for(i = 0; i < 3; i++)
	{
		smtpt[ i ] = pc[ i ] + ( stepi * (double)maxidx ) * vcd[ i ];
	}
	// UV
	for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
	{
		smtuv[ tt ] = cuv[ tt ] + ( stepi * (double)maxidx ) * vuv[ tt ];
	}

	// Create two faked tets to hold the two non-existing boundary faces:
	//   [d,c,a] and [c,d,b].
	maketetrahedron( &faketet1 );
	setvertices( faketet1, pd, pc, org( abtets[ 0 ] ), dummypoint );
	cavetetlist->newindex( (void **)&parytet );
	*parytet = faketet1;
	maketetrahedron( &faketet2 );
	setvertices( faketet2, pc, pd, dest( abtets[ 0 ] ), dummypoint );
	cavetetlist->newindex( (void **)&parytet );
	*parytet = faketet2;

	// Point smooth options.
	opm.max_min_volume = 1;
	opm.numofsearchdirs = 20;
	opm.searchstep = 0.001;
	opm.maxiter = 100; // Limit the maximum iterations.
	opm.initval = 0.0; // Initial volume is zero.

	// Try to relocate the point into the inside of the polyhedron.
	success = smoothpoint( smtpt, cavetetlist, 1, &opm );
	// UV: TODO

	if(success)
	{
		while(opm.smthiter == 100)
		{
			// It was relocated and the prescribed maximum iteration reached. 
			// Try to increase the search stepsize.
			opm.searchstep *= 10.0;
			//opm.maxiter = 100; // Limit the maximum iterations.
			opm.initval = opm.imprval;
			opm.smthiter = 0; // Init.
			smoothpoint( smtpt, cavetetlist, 1, &opm );
			// UV: TODO
		}
	} // if (success)

	// Delete the two faked tets.
	tetrahedrondealloc( faketet1.tet );
	tetrahedrondealloc( faketet2.tet );

	cavetetlist->restart( );

	if(!success)
	{
		return 0;
	}


	// Insert the Steiner point.
	makepoint( &steinerpt, FREEVOLVERTEX );
	for(i = 0; i < 3; i++) steinerpt[ i ] = smtpt[ i ];
	if(b->psc)
	{
		// UV
		for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
		{
			setpointgeomuv( steinerpt, tt, smtuv[ tt ] );
		}
	}

	// Insert the created Steiner point.
	for(i = 0; i < n; i++)
	{
		infect( abtets[ i ] );
		caveoldtetlist->newindex( (void **)&parytet );
		*parytet = abtets[ i ];
	}
	worktet = abtets[ 0 ]; // No need point location.
	ivf.iloc = (int)INSTAR;
	ivf.chkencflag = chkencflag;
	ivf.assignmeshsize = b->metric;
	if(ivf.assignmeshsize)
	{
		// Search the tet containing 'steinerpt' for size interpolation.
		locate( steinerpt, &( abtets[ 0 ] ) );
		worktet = abtets[ 0 ];
	}

	// Insert the new point into the tetrahedralization T.
	// Note that T is convex (nonconvex = 0).
	if(insertpoint( steinerpt, &worktet, NULL, NULL, &ivf ))
	{
		// The vertex has been inserted.
		st_volref_count++;
		if(steinerleft > 0) steinerleft--;
		return 1;
	}
	else
	{
		// Not inserted. 
		pointdealloc( steinerpt );
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// add_steinerpt_in_segment()    Add a Steiner point inside a segment.       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::add_steinerpt_in_segment( face* misseg, int searchlevel )
{
	triface searchtet;
	face *paryseg, candseg;
	point startpt, endpt, pc, pd;
	flipconstraints fc;
	enum interresult dir;
	REAL P[ 3 ], Q[ 3 ], tp, tq;
	REAL len, smlen = 0, split = 0, split_q = 0;
	int success;
	int i;

	startpt = sorg( *misseg );
	endpt = sdest( *misseg );

	fc.seg[ 0 ] = startpt;
	fc.seg[ 1 ] = endpt;
	fc.checkflipeligibility = 1;
	fc.collectencsegflag = 1;

	point2tetorg( startpt, searchtet );
	dir = finddirection( &searchtet, endpt );
	// Try to flip the first intersecting face/edge.
	enextesymself( searchtet ); // Go to the opposite face.

	int bak_fliplinklevel = b->fliplinklevel;
	b->fliplinklevel = searchlevel;

	if(dir == ACROSSFACE)
	{
		// A face is intersected with the segment. Try to flip it.
		success = removefacebyflips( &searchtet, &fc );
	}
	else if(dir == ACROSSEDGE)
	{
		// An edge is intersected with the segment. Try to flip it.
		success = removeedgebyflips( &searchtet, &fc );
	}

	split = 0;
	for(i = 0; i < caveencseglist->objects; i++)
	{
		paryseg = (face *)fastlookup( caveencseglist, i );
		suninfect( *paryseg );
		// Calculate the shortest edge between the two lines.
		pc = sorg( *paryseg );
		pd = sdest( *paryseg );
		tp = tq = 0;
		if(linelineint( startpt, endpt, pc, pd, P, Q, &tp, &tq ))
		{
			// Does the shortest edge lie between the two segments? 
			// Round tp and tq.
			if(( tp > 0 ) && ( tq < 1 ))
			{
				if(tp < 0.5)
				{
					if(tp < ( b->epsilon * 1e+3 )) tp = 0.0;
				}
				else
				{
					if(( 1.0 - tp ) < ( b->epsilon * 1e+3 )) tp = 1.0;
				}
			}
			if(( tp <= 0 ) || ( tp >= 1 )) continue;
			if(( tq > 0 ) && ( tq < 1 ))
			{
				if(tq < 0.5)
				{
					if(tq < ( b->epsilon * 1e+3 )) tq = 0.0;
				}
				else
				{
					if(( 1.0 - tq ) < ( b->epsilon * 1e+3 )) tq = 1.0;
				}
			}
			if(( tq <= 0 ) || ( tq >= 1 )) continue;
			// It is a valid shortest edge. Calculate its length.
			len = distance( P, Q );
			if(split == 0)
			{
				smlen = len;
				split = tp;
				split_q = tq;
				candseg = *paryseg;
			}
			else
			{
				if(len < smlen)
				{
					smlen = len;
					split = tp;
					split_q = tq;
					candseg = *paryseg;
				}
			}
		}
	}

	caveencseglist->restart( );
	b->fliplinklevel = bak_fliplinklevel;

	if(split == 0)
	{
		// Found no crossing segment. 
		return 0;
	}

	face splitsh;
	face splitseg;
	point steinerpt, *parypt;
	insertvertexflags ivf;

	if(b->addsteiner_algo == 1)
	{
		// Split the segment at the closest point to a near segment.
		makepoint( &steinerpt, FREESEGVERTEX );
		for(i = 0; i < 3; i++)
		{
			steinerpt[ i ] = startpt[ i ] + split * ( endpt[ i ] - startpt[ i ] );
		}
		// UV interpolation
		if(b->psc)
		{
			for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
			{
				REAL auv, buv, newuv;
				auv = pointgeomuv( startpt, tt );
				buv = pointgeomuv( endpt, tt );
				newuv = auv + split * ( buv - auv );
				setpointgeomuv( steinerpt, tt, newuv );
			}
		}
	}
	else
	{ // b->addsteiner_algo == 2
		for(i = 0; i < 3; i++)
		{
			P[ i ] = startpt[ i ] + split * ( endpt[ i ] - startpt[ i ] );
		}
		pc = sorg( candseg );
		pd = sdest( candseg );
		for(i = 0; i < 3; i++)
		{
			Q[ i ] = pc[ i ] + split_q * ( pd[ i ] - pc[ i ] );
		}
		makepoint( &steinerpt, FREEVOLVERTEX );
		for(i = 0; i < 3; i++)
		{
			steinerpt[ i ] = 0.5 * ( P[ i ] + Q[ i ] );
		}
		// UV interpolation: TODO!
	}

	// We need to locate the point. Start searching from 'searchtet'.
	if(split < 0.5)
	{
		point2tetorg( startpt, searchtet );
	}
	else
	{
		point2tetorg( endpt, searchtet );
	}
	if(b->addsteiner_algo == 1)
	{
		splitseg = *misseg;
		spivot( *misseg, splitsh );
	}
	else
	{
		splitsh.sh = NULL;
		splitseg.sh = NULL;
	}
	ivf.iloc = (int)OUTSIDE;
	ivf.bowywat = 1;
	ivf.lawson = 0;
	ivf.rejflag = 0;
	ivf.chkencflag = 0;
	ivf.sloc = (int)ONEDGE;
	ivf.sbowywat = 1;
	ivf.splitbdflag = 0;
	ivf.validflag = 1;
	ivf.respectbdflag = 1;
	ivf.assignmeshsize = b->metric;

	if(!insertpoint( steinerpt, &searchtet, &splitsh, &splitseg, &ivf ))
	{
		pointdealloc( steinerpt );
		return 0;
	}

	if(b->addsteiner_algo == 1)
	{
		// Save this Steiner point (for removal).
		//   Re-use the array 'subvertstack'.
		subvertstack->newindex( (void **)&parypt );
		*parypt = steinerpt;
		st_segref_count++;
	}
	else
	{ // b->addsteiner_algo == 2
		// Queue the segment for recovery.
		subsegstack->newindex( (void **)&paryseg );
		*paryseg = *misseg;
		st_volref_count++;
	}
	if(steinerleft > 0) steinerleft--;

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// addsteiner4recoversegment()    Add a Steiner point for recovering a seg.  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::addsteiner4recoversegment( face* misseg, int splitsegflag )
{
	triface *abtets, searchtet, spintet;
	face splitsh;
	face *paryseg;
	point startpt, endpt;
	point pa, pb, pd, steinerpt, *parypt;
	enum interresult dir;
	insertvertexflags ivf;
	int types[ 2 ], poss[ 4 ];
	int n, endi, success;
	int t1ver;
	int i;

	startpt = sorg( *misseg );
	if(pointtype( startpt ) == FREESEGVERTEX)
	{
		sesymself( *misseg );
		startpt = sorg( *misseg );
	}
	endpt = sdest( *misseg );

	// Try to recover the edge by adding Steiner points.
	point2tetorg( startpt, searchtet );
	dir = finddirection( &searchtet, endpt );
	enextself( searchtet );

	if(dir == ACROSSFACE)
	{
		// The segment is crossing at least 3 faces. Find the common edge of 
		//   the first 3 crossing faces.
		esymself( searchtet );
		fsym( searchtet, spintet );
		pd = oppo( spintet );
		for(i = 0; i < 3; i++)
		{
			pa = org( spintet );
			pb = dest( spintet );
			if(tri_edge_test( pa, pb, pd, startpt, endpt, NULL, 1, types, poss ))
			{
				break; // Found the edge.
			}
			enextself( spintet );
			eprevself( searchtet );
		}
		esymself( searchtet );
	}

	spintet = searchtet;
	n = 0; endi = -1;
	while(1)
	{
		// Check if the endpt appears in the star.
		if(apex( spintet ) == endpt)
		{
			endi = n; // Remember the position of endpt.
		}
		n++; // Count a tet in the star.
		fnextself( spintet );
		if(spintet.tet == searchtet.tet) break;
	}

	if(endi > 0)
	{
		// endpt is also in the edge star
		// Get all tets in the edge star.
		abtets = new triface[ n ];
		spintet = searchtet;
		for(i = 0; i < n; i++)
		{
			abtets[ i ] = spintet;
			fnextself( spintet );
		}

		success = 0;

		if(dir == ACROSSFACE)
		{
			// Find a Steiner points inside the polyhedron.
			if(add_steinerpt_in_schoenhardtpoly( abtets, endi, 0 ))
			{
				success = 1;
			}
		}
		else if(dir == ACROSSEDGE)
		{
			// PLC check.
			if(issubseg( searchtet ))
			{
				terminatetetgen( this, 2 );
			}
			if(n > 4)
			{
				// In this case, 'abtets' is separated by the plane (containing the
				//   two intersecting edges) into two parts, P1 and P2, where P1
				//   consists of 'endi' tets: abtets[0], abtets[1], ..., 
				//   abtets[endi-1], and P2 consists of 'n - endi' tets: 
				//   abtets[endi], abtets[endi+1], abtets[n-1].
				if(endi > 2)
				{ // P1
					// There are at least 3 tets in the first part.
					if(add_steinerpt_in_schoenhardtpoly( abtets, endi, 0 ))
					{
						success++;
					}
				}
				if(( n - endi ) > 2)
				{ // P2
					// There are at least 3 tets in the first part.
					if(add_steinerpt_in_schoenhardtpoly( &( abtets[ endi ] ), n - endi, 0 ))
					{
						success++;
					}
				}
			}
			else
			{
				// In this case, a 4-to-4 flip should be re-cover the edge [c,d].
				//   However, there will be invalid tets (either zero or negtive 
				//   volume). Otherwise, [c,d] should already be recovered by the 
				//   recoveredge() function.
				terminatetetgen( this, 2 );
			}
		}
		else
		{
			terminatetetgen( this, 2 );
		}

		delete[ ] abtets;

		if(success)
		{
			// Add the missing segment back to the recovering list.
			subsegstack->newindex( (void **)&paryseg );
			*paryseg = *misseg;
			return 1;
		}
	} // if (endi > 0)

	if(!splitsegflag)
	{
		return 0;
	}

	if(b->verbose > 2)
	{
		myprintf( "      Splitting segment (%d, %d)\n", pointmark( startpt ),
				pointmark( endpt ) );
	}
	steinerpt = NULL;

	if(b->addsteiner_algo > 0)
	{ // -Y/1 or -Y/2
		if(add_steinerpt_in_segment( misseg, 3 ))
		{
			return 1;
		}
		sesymself( *misseg );
		if(add_steinerpt_in_segment( misseg, 3 ))
		{
			return 1;
		}
		sesymself( *misseg );
	}




	if(steinerpt == NULL)
	{
		// Split the segment at its midpoint.
		makepoint( &steinerpt, FREESEGVERTEX );
		for(i = 0; i < 3; i++)
		{
			steinerpt[ i ] = 0.5 * ( startpt[ i ] + endpt[ i ] );
		}
		if(b->psc)
		{
			for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
			{
				REAL startuv, enduv, newuv;
				startuv = pointgeomuv( startpt, tt );
				enduv = pointgeomuv( endpt, tt );
				newuv = 0.5 * ( startuv + enduv );
				setpointgeomuv( steinerpt, tt, newuv );
			}
		}

		// We need to locate the point.
		spivot( *misseg, splitsh );
		ivf.iloc = (int)OUTSIDE;
		ivf.bowywat = 1;
		ivf.lawson = 0;
		ivf.rejflag = 0;
		ivf.chkencflag = 0;
		ivf.sloc = (int)ONEDGE;
		ivf.sbowywat = 1;
		ivf.splitbdflag = 0;
		ivf.validflag = 1;
		ivf.respectbdflag = 1;
		ivf.assignmeshsize = b->metric;
		if(!insertpoint( steinerpt, &searchtet, &splitsh, misseg, &ivf ))
		{
			terminatetetgen( this, 2 );
		}
	} // if (endi > 0)

	// Save this Steiner point (for removal).
	//   Re-use the array 'subvertstack'.
	subvertstack->newindex( (void **)&parypt );
	*parypt = steinerpt;

	st_segref_count++;
	if(steinerleft > 0) steinerleft--;

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// recoversegments()    Recover all segments.                                //
//                                                                           //
// All segments need to be recovered are in 'subsegstack'.                   //
//                                                                           //
// This routine first tries to recover each segment by only using flips. If  //
// no flip is possible, and the flag 'steinerflag' is set, it then tries to  //
// insert Steiner points near or in the segment.                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::recoversegments( arraypool *misseglist, int fullsearch,
								 int steinerflag )
{
	triface searchtet, spintet;
	face sseg, *paryseg;
	point startpt, endpt;
	int success;
	int t1ver;

	long bak_inpoly_count = st_volref_count;
	long bak_segref_count = st_segref_count;

	if(b->verbose > 1)
	{
		myprintf( "    Recover segments [%s level = %2d] #:  %ld.\n",
				( b->fliplinklevel > 0 ) ? "fixed" : "auto",
				( b->fliplinklevel > 0 ) ? b->fliplinklevel : autofliplinklevel,
				subsegstack->objects );
	}

	// Loop until 'subsegstack' is empty.
	while(subsegstack->objects > 0l)
	{
		// seglist is used as a stack.
		subsegstack->objects--;
		paryseg = (face *)fastlookup( subsegstack, subsegstack->objects );
		sseg = *paryseg;

		// Check if this segment has been recovered.
		sstpivot1( sseg, searchtet );
		if(searchtet.tet != NULL)
		{
			continue; // Not a missing segment.
		}

		startpt = sorg( sseg );
		endpt = sdest( sseg );

		if(b->verbose > 2)
		{
			myprintf( "      Recover segment (%d, %d).\n", pointmark( startpt ),
					pointmark( endpt ) );
		}

		success = 0;

		if(recoveredgebyflips( startpt, endpt, &sseg, &searchtet, 0 ))
		{
			success = 1;
		}
		else
		{
			// Try to recover it from the other direction.
			if(recoveredgebyflips( endpt, startpt, &sseg, &searchtet, 0 ))
			{
				success = 1;
			}
		}

		if(!success && fullsearch)
		{
			if(recoveredgebyflips( startpt, endpt, &sseg, &searchtet, fullsearch ))
			{
				success = 1;
			}
		}

		if(success)
		{
			// Segment is recovered. Insert it.
			// Let the segment remember an adjacent tet.
			sstbond1( sseg, searchtet );
			// Bond the segment to all tets containing it.
			spintet = searchtet;
			do
			{
				tssbond1( spintet, sseg );
				fnextself( spintet );
			}
			while(spintet.tet != searchtet.tet);
		}
		else
		{
			if(steinerflag > 0)
			{
				// Try to recover the segment but do not split it.
				if(addsteiner4recoversegment( &sseg, 0 ))
				{
					success = 1;
				}
				if(!success && ( steinerflag > 1 ))
				{
					// Split the segment.
					addsteiner4recoversegment( &sseg, 1 );
					success = 1;
				}
			}
			if(!success)
			{
				if(misseglist != NULL)
				{
					// Save this segment.
					misseglist->newindex( (void **)&paryseg );
					*paryseg = sseg;
				}
			}
		}

	} // while (subsegstack->objects > 0l)

	if(steinerflag)
	{
		if(b->verbose > 1)
		{
			// Report the number of added Steiner points.
			if(st_volref_count > bak_inpoly_count)
			{
				myprintf( "    Add %ld Steiner points in volume.\n",
						st_volref_count - bak_inpoly_count );
			}
			if(st_segref_count > bak_segref_count)
			{
				myprintf( "    Add %ld Steiner points in segments.\n",
						st_segref_count - bak_segref_count );
			}
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// recoverfacebyflips()    Recover a face by flips.                          //
//                                                                           //
// 'pa', 'pb', and 'pc' are the three vertices of this face.  This routine   //
// tries to recover it in the tetrahedral mesh. It is assumed that the three //
// edges, i.e., pa->pb, pb->pc, and pc->pa all exist.                        //
//                                                                           //
// If the face is recovered, it is returned by 'searchtet'.                  //
//                                                                           //
// If 'searchsh' is not NULL, it is a subface to be recovered.  Its vertices //
// must be pa, pb, and pc.  It is mainly used to check self-intersections.   //
// Another use of this subface is to split it when a Steiner point is found  //
// inside this subface.                                                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::recoverfacebyflips( point pa, point pb, point pc,
									face *searchsh, triface* searchtet )
{
	triface spintet, flipedge;
	point pd, pe;
	flipconstraints fc;
	int types[ 2 ], poss[ 4 ], intflag;
	int success;
	int t1ver;
	int i, j;


	fc.fac[ 0 ] = pa;
	fc.fac[ 1 ] = pb;
	fc.fac[ 2 ] = pc;
	fc.checkflipeligibility = 1;
	success = 0;

	for(i = 0; i < 3 && !success; i++)
	{
		while(1)
		{
			// Get a tet containing the edge [a,b].
			point2tetorg( fc.fac[ i ], *searchtet );
			finddirection( searchtet, fc.fac[ ( i + 1 ) % 3 ] );
			// Search the face [a,b,c]
			spintet = *searchtet;
			while(1)
			{
				if(apex( spintet ) == fc.fac[ ( i + 2 ) % 3 ])
				{
					// Found the face.
					*searchtet = spintet;
					// Return the face [a,b,c].
					for(j = i; j > 0; j--)
					{
						eprevself( *searchtet );
					}
					success = 1;
					break;
				}
				fnextself( spintet );
				if(spintet.tet == searchtet->tet) break;
			} // while (1)
			if(success) break;
			// The face is missing. Try to recover it.
			flipedge.tet = NULL;
			// Find a crossing edge of this face.
			spintet = *searchtet;
			while(1)
			{
				pd = apex( spintet );
				pe = oppo( spintet );
				if(( pd != dummypoint ) && ( pe != dummypoint ))
				{
					// Check if [d,e] intersects [a,b,c]
					intflag = tri_edge_test( pa, pb, pc, pd, pe, NULL, 1, types, poss );
					if(intflag > 0)
					{
						// By the assumption that all edges of the face exist, they can
						//   only intersect at a single point.
						if(intflag == 2)
						{
							// Go to the edge [d,e].
							edestoppo( spintet, flipedge ); // [d,e,a,b]
							if(searchsh != NULL)
							{
								// Check the intersection type.
								if(( types[ 0 ] == (int)ACROSSFACE ) ||
									( types[ 0 ] == (int)ACROSSEDGE ))
								{
									// Check if [e,d] is a segment.
									if(issubseg( flipedge ))
									{
										return report_selfint_face( pa, pb, pc, searchsh, &flipedge,
																	intflag, types, poss );
									}
									else
									{
										// Check if [e,d] is an edge of a subface.
										triface chkface = flipedge;
										while(1)
										{
											if(issubface( chkface )) break;
											fsymself( chkface );
											if(chkface.tet == flipedge.tet) break;
										}
										if(issubface( chkface ))
										{
											// Two subfaces are intersecting.
											return report_selfint_face( pa, pb, pc, searchsh, &chkface,
																		intflag, types, poss );
										}
									}
								}
								else if(types[ 0 ] == TOUCHFACE)
								{
									// This is possible when a Steiner point was added on it.
									point touchpt, *parypt;
									if(poss[ 1 ] == 0)
									{
										touchpt = pd; // pd is a coplanar vertex.
									}
									else
									{
										touchpt = pe; // pe is a coplanar vertex.
									}
									if(pointtype( touchpt ) == FREEVOLVERTEX)
									{
										// A volume Steiner point was added in this subface.
										// Split this subface by this point.
										face checksh, *parysh;
										int siloc = (int)ONFACE;
										int sbowat = 0; // Only split this subface. A 1-to-3 flip.
										setpointtype( touchpt, FREEFACETVERTEX );
										sinsertvertex( touchpt, searchsh, NULL, siloc, sbowat, 0 );
										st_volref_count--;
										st_facref_count++;
										// Queue this vertex for removal.
										subvertstack->newindex( (void **)&parypt );
										*parypt = touchpt;
										// Queue new subfaces for recovery.
										// Put all new subfaces into stack for recovery.
										for(i = 0; i < caveshbdlist->objects; i++)
										{
											// Get an old subface at edge [a, b].
											parysh = (face *)fastlookup( caveshbdlist, i );
											spivot( *parysh, checksh ); // The new subface [a, b, p].
											// Do not recover a deleted new face (degenerated).
											if(checksh.sh[ 3 ] != NULL)
											{
												subfacstack->newindex( (void **)&parysh );
												*parysh = checksh;
											}
										}
										// Delete the old subfaces in sC(p).
										for(i = 0; i < caveshlist->objects; i++)
										{
											parysh = (face *)fastlookup( caveshlist, i );
											shellfacedealloc( subfaces, parysh->sh );
										}
										// Clear working lists.
										caveshlist->restart( );
										caveshbdlist->restart( );
										cavesegshlist->restart( );
										// We can return this function.
										searchsh->sh = NULL; // It has been split.
										return 1;
									}
									else
									{
										// Other cases may be due to a bug or a PLC error.
										return report_selfint_face( pa, pb, pc, searchsh, &flipedge,
																	intflag, types, poss );
									}
								}
								else
								{
									// The other intersection types: ACROSSVERT, TOUCHEDGE, 
									// SHAREVERTEX should not be possible or due to a PLC error.
									return report_selfint_face( pa, pb, pc, searchsh, &flipedge,
																intflag, types, poss );
								}
							} // if (searchsh != NULL)
						}
						else
						{ // intflag == 4. Coplanar case.
							terminatetetgen( this, 2 );
						}
						break;
					} // if (intflag > 0)
				}
				fnextself( spintet );
				if(spintet.tet == searchtet->tet)
				{
					terminatetetgen( this, 2 );
				}
			} // while (1)
			// Try to flip the edge [d,e].
			if(removeedgebyflips( &flipedge, &fc ) == 2)
			{
				// A crossing edge is removed.
				continue;
			}
			break;
		} // while (1)
	} // i

	return success;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// recoversubfaces()    Recover all subfaces.                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::recoversubfaces( arraypool *misshlist, int steinerflag )
{
	triface searchtet, neightet, spintet;
	face searchsh, neighsh, neineish, *parysh;
	face bdsegs[ 3 ];
	point startpt, endpt, apexpt, *parypt;
	point steinerpt;
	insertvertexflags ivf;
	int success;
	int t1ver;
	int i, j;

	if(b->verbose > 1)
	{
		myprintf( "    Recover subfaces [%s level = %2d] #:  %ld.\n",
				( b->fliplinklevel > 0 ) ? "fixed" : "auto",
				( b->fliplinklevel > 0 ) ? b->fliplinklevel : autofliplinklevel,
				subfacstack->objects );
	}

	// Loop until 'subfacstack' is empty.
	while(subfacstack->objects > 0l)
	{

		subfacstack->objects--;
		parysh = (face *)fastlookup( subfacstack, subfacstack->objects );
		searchsh = *parysh;

		if(searchsh.sh[ 3 ] == NULL) continue; // Skip a dead subface.

		stpivot( searchsh, neightet );
		if(neightet.tet != NULL) continue; // Skip a recovered subface.


		if(b->verbose > 2)
		{
			myprintf( "      Recover subface (%d, %d, %d).\n", pointmark( sorg( searchsh ) ),
					pointmark( sdest( searchsh ) ), pointmark( sapex( searchsh ) ) );
		}

		// The three edges of the face need to be existed first.
		for(i = 0; i < 3; i++)
		{
			sspivot( searchsh, bdsegs[ i ] );
			if(bdsegs[ i ].sh != NULL)
			{
				// The segment must exist.
				sstpivot1( bdsegs[ i ], searchtet );
				if(searchtet.tet == NULL)
				{
					terminatetetgen( this, 2 );
				}
			}
			else
			{
				// This edge is not a segment (due to a Steiner point).
				// Check whether it exists or not.
				success = 0;
				startpt = sorg( searchsh );
				endpt = sdest( searchsh );
				point2tetorg( startpt, searchtet );
				finddirection( &searchtet, endpt );
				if(dest( searchtet ) == endpt)
				{
					success = 1;
				}
				else
				{
					// The edge is missing. Try to recover it.
					if(recoveredgebyflips( startpt, endpt, &searchsh, &searchtet, 0 ))
					{
						success = 1;
					}
					else
					{
						if(recoveredgebyflips( endpt, startpt, &searchsh, &searchtet, 0 ))
						{
							success = 1;
						}
					}
				}
				if(success)
				{
					// Insert a temporary segment to protect this edge.
					makeshellface( subsegs, &( bdsegs[ i ] ) );
					setshvertices( bdsegs[ i ], startpt, endpt, NULL );
					smarktest2( bdsegs[ i ] ); // It's a temporary segment.
					// Insert this segment into surface mesh.
					ssbond( searchsh, bdsegs[ i ] );
					spivot( searchsh, neighsh );
					if(neighsh.sh != NULL)
					{
						ssbond( neighsh, bdsegs[ i ] );
					}
					// Insert this segment into tetrahedralization.
					sstbond1( bdsegs[ i ], searchtet );
					// Bond the segment to all tets containing it.
					spintet = searchtet;
					do
					{
						tssbond1( spintet, bdsegs[ i ] );
						fnextself( spintet );
					}
					while(spintet.tet != searchtet.tet);
				}
				else
				{
					// An edge of this subface is missing. Can't recover this subface.
					// Delete any temporary segment that has been created.
					for(j = ( i - 1 ); j >= 0; j--)
					{
						if(smarktest2ed( bdsegs[ j ] ))
						{
							spivot( bdsegs[ j ], neineish );
							ssdissolve( neineish );
							spivot( neineish, neighsh );
							if(neighsh.sh != NULL)
							{
								ssdissolve( neighsh );
							}
							sstpivot1( bdsegs[ j ], searchtet );
							spintet = searchtet;
							while(1)
							{
								tssdissolve1( spintet );
								fnextself( spintet );
								if(spintet.tet == searchtet.tet) break;
							}
							shellfacedealloc( subsegs, bdsegs[ j ].sh );
						}
					} // j
					if(steinerflag)
					{
						// Add a Steiner point at the midpoint of this edge.
						if(b->verbose > 2)
						{
							myprintf( "      Add a Steiner point in subedge (%d, %d).\n",
									pointmark( startpt ), pointmark( endpt ) );
						}
						makepoint( &steinerpt, FREEFACETVERTEX );
						for(j = 0; j < 3; j++)
						{
							steinerpt[ j ] = 0.5 * ( startpt[ j ] + endpt[ j ] );
						}
						if(b->psc)
						{
							// UV
							for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
							{
								REAL startuv, enduv, newuv;
								startuv = pointgeomuv( startpt, tt );
								enduv = pointgeomuv( endpt, tt );
								newuv = 0.5 * ( startuv + enduv );
								setpointgeomuv( steinerpt, tt, newuv );
							}
						}

						point2tetorg( startpt, searchtet ); // Start from 'searchtet'.
						ivf.iloc = (int)OUTSIDE; // Need point location.
						ivf.bowywat = 1;
						ivf.lawson = 0;
						ivf.rejflag = 0;
						ivf.chkencflag = 0;
						ivf.sloc = (int)ONEDGE;
						ivf.sbowywat = 1; // Allow flips in facet.
						ivf.splitbdflag = 0;
						ivf.validflag = 1;
						ivf.respectbdflag = 1;
						ivf.assignmeshsize = b->metric;
						if(!insertpoint( steinerpt, &searchtet, &searchsh, NULL, &ivf ))
						{
							terminatetetgen( this, 2 );
						}
						// Save this Steiner point (for removal).
						//   Re-use the array 'subvertstack'.
						subvertstack->newindex( (void **)&parypt );
						*parypt = steinerpt;

						st_facref_count++;
						if(steinerleft > 0) steinerleft--;
					} // if (steinerflag)
					break;
				}
			}
			senextself( searchsh );
		} // i

		if(i == 3)
		{
			// Recover the subface.
			startpt = sorg( searchsh );
			endpt = sdest( searchsh );
			apexpt = sapex( searchsh );

			success = recoverfacebyflips( startpt, endpt, apexpt, &searchsh, &searchtet );

			// Delete any temporary segment that has been created.
			for(j = 0; j < 3; j++)
			{
				if(smarktest2ed( bdsegs[ j ] ))
				{
					spivot( bdsegs[ j ], neineish );
					ssdissolve( neineish );
					spivot( neineish, neighsh );
					if(neighsh.sh != NULL)
					{
						ssdissolve( neighsh );
					}
					sstpivot1( bdsegs[ j ], neightet );
					spintet = neightet;
					while(1)
					{
						tssdissolve1( spintet );
						fnextself( spintet );
						if(spintet.tet == neightet.tet) break;
					}
					shellfacedealloc( subsegs, bdsegs[ j ].sh );
				}
			} // j

			if(success)
			{
				if(searchsh.sh != NULL)
				{
					// Face is recovered. Insert it.
					tsbond( searchtet, searchsh );
					fsymself( searchtet );
					sesymself( searchsh );
					tsbond( searchtet, searchsh );
				}
			}
			else
			{
				if(steinerflag)
				{
					// Add a Steiner point at the barycenter of this subface.
					if(b->verbose > 2)
					{
						myprintf( "      Add a Steiner point in subface (%d, %d, %d).\n",
								pointmark( startpt ), pointmark( endpt ), pointmark( apexpt ) );
					}
					makepoint( &steinerpt, FREEFACETVERTEX );
					for(j = 0; j < 3; j++)
					{
						steinerpt[ j ] = ( startpt[ j ] + endpt[ j ] + apexpt[ j ] ) / 3.0;
					}
					if(b->psc)
					{
						// UV
						for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
						{
							REAL startuv, enduv, apexuv, newuv;
							startuv = pointgeomuv( startpt, tt );
							enduv = pointgeomuv( endpt, tt );
							apexuv = pointgeomuv( apexpt, tt );
							newuv = ( startuv + enduv + apexuv ) / 3.0;
							setpointgeomuv( steinerpt, tt, newuv );
						}
					}

					point2tetorg( startpt, searchtet ); // Start from 'searchtet'.
					ivf.iloc = (int)OUTSIDE; // Need point location.
					ivf.bowywat = 1;
					ivf.lawson = 0;
					ivf.rejflag = 0;
					ivf.chkencflag = 0;
					ivf.sloc = (int)ONFACE;
					ivf.sbowywat = 1; // Allow flips in facet.
					ivf.splitbdflag = 0;
					ivf.validflag = 1;
					ivf.respectbdflag = 1;
					ivf.assignmeshsize = b->metric;
					if(!insertpoint( steinerpt, &searchtet, &searchsh, NULL, &ivf ))
					{
						terminatetetgen( this, 2 );
					}
					// Save this Steiner point (for removal).
					//   Re-use the array 'subvertstack'.
					subvertstack->newindex( (void **)&parypt );
					*parypt = steinerpt;

					st_facref_count++;
					if(steinerleft > 0) steinerleft--;
				} // if (steinerflag)
			}
		}
		else
		{
			success = 0;
		}

		if(!success)
		{
			if(misshlist != NULL)
			{
				// Save this subface.
				misshlist->newindex( (void **)&parysh );
				*parysh = searchsh;
			}
		}

	} // while (subfacstack->objects > 0l)

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// getvertexstar()    Return the star of a vertex.                           //
//                                                                           //
// If the flag 'fullstar' is set, return the complete star of this vertex.   //
// Otherwise, only a part of the star which is bounded by facets is returned.// 
//                                                                           //
// 'tetlist' returns the list of tets in the star of the vertex 'searchpt'.  //
// Every tet in 'tetlist' is at the face opposing to 'searchpt'.             //
//                                                                           //
// 'vertlist' returns the list of vertices in the star (exclude 'searchpt'). //
//                                                                           //
// 'shlist' returns the list of subfaces in the star. Each subface must face //
// to the interior of this star.                                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::getvertexstar( int fullstar, point searchpt, arraypool* tetlist,
							   arraypool* vertlist, arraypool* shlist )
{
	triface searchtet, neightet, *parytet;
	face checksh, *parysh;
	point pt, *parypt;
	int collectflag;
	int t1ver;
	int i, j;

	point2tetorg( searchpt, searchtet );

	// Go to the opposite face (the link face) of the vertex.
	enextesymself( searchtet );
	//assert(oppo(searchtet) == searchpt);
	infect( searchtet ); // Collect this tet (link face).
	tetlist->newindex( (void **)&parytet );
	*parytet = searchtet;
	if(vertlist != NULL)
	{
		// Collect three (link) vertices.
		j = ( searchtet.ver & 3 ); // The current vertex index.
		for(i = 1; i < 4; i++)
		{
			pt = (point)searchtet.tet[ 4 + ( ( j + i ) % 4 ) ];
			pinfect( pt );
			vertlist->newindex( (void **)&parypt );
			*parypt = pt;
		}
	}

	collectflag = 1;
	esym( searchtet, neightet );
	if(issubface( neightet ))
	{
		if(shlist != NULL)
		{
			tspivot( neightet, checksh );
			if(!sinfected( checksh ))
			{
				// Collect this subface (link edge).
				sinfected( checksh );
				shlist->newindex( (void **)&parysh );
				*parysh = checksh;
			}
		}
		if(!fullstar)
		{
			collectflag = 0;
		}
	}
	if(collectflag)
	{
		fsymself( neightet ); // Goto the adj tet of this face.
		esymself( neightet ); // Goto the oppo face of this vertex.
		// assert(oppo(neightet) == searchpt);
		infect( neightet ); // Collect this tet (link face).
		tetlist->newindex( (void **)&parytet );
		*parytet = neightet;
		if(vertlist != NULL)
		{
			// Collect its apex.
			pt = apex( neightet );
			pinfect( pt );
			vertlist->newindex( (void **)&parypt );
			*parypt = pt;
		}
	} // if (collectflag)

	// Continue to collect all tets in the star.
	for(i = 0; i < tetlist->objects; i++)
	{
		searchtet = *(triface *)fastlookup( tetlist, i );
		// Note that 'searchtet' is a face opposite to 'searchpt', and the neighbor
		//   tet at the current edge is already collected.
		// Check the neighbors at the other two edges of this face.
		for(j = 0; j < 2; j++)
		{
			collectflag = 1;
			enextself( searchtet );
			esym( searchtet, neightet );
			if(issubface( neightet ))
			{
				if(shlist != NULL)
				{
					tspivot( neightet, checksh );
					if(!sinfected( checksh ))
					{
						// Collect this subface (link edge).
						sinfected( checksh );
						shlist->newindex( (void **)&parysh );
						*parysh = checksh;
					}
				}
				if(!fullstar)
				{
					collectflag = 0;
				}
			}
			if(collectflag)
			{
				fsymself( neightet );
				if(!infected( neightet ))
				{
					esymself( neightet ); // Go to the face opposite to 'searchpt'.
					infect( neightet );
					tetlist->newindex( (void **)&parytet );
					*parytet = neightet;
					if(vertlist != NULL)
					{
						// Check if a vertex is collected.
						pt = apex( neightet );
						if(!pinfected( pt ))
						{
							pinfect( pt );
							vertlist->newindex( (void **)&parypt );
							*parypt = pt;
						}
					}
				} // if (!infected(neightet))
			} // if (collectflag)
		} // j
	} // i


	// Uninfect the list of tets and vertices.
	for(i = 0; i < tetlist->objects; i++)
	{
		parytet = (triface *)fastlookup( tetlist, i );
		uninfect( *parytet );
	}

	if(vertlist != NULL)
	{
		for(i = 0; i < vertlist->objects; i++)
		{
			parypt = (point *)fastlookup( vertlist, i );
			puninfect( *parypt );
		}
	}

	if(shlist != NULL)
	{
		for(i = 0; i < shlist->objects; i++)
		{
			parysh = (face *)fastlookup( shlist, i );
			suninfect( *parysh );
		}
	}

	return (int)tetlist->objects;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// getedge()    Get a tetrahedron having the two endpoints.                  //
//                                                                           //
// The method here is to search the second vertex in the link faces of the   //
// first vertex. The global array 'cavetetlist' is re-used for searching.    //
//                                                                           //
// This function is used for the case when the mesh is non-convex. Otherwise,//
// the function finddirection() should be faster than this.                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::getedge( point e1, point e2, triface *tedge )
{
	triface searchtet, neightet, *parytet;
	point pt;
	int done;
	int i, j;

	if(b->verbose > 2)
	{
		myprintf( "      Get edge from %d to %d.\n", pointmark( e1 ), pointmark( e2 ) );
	}

	// Quickly check if 'tedge' is just this edge.
	if(!isdeadtet( *tedge ))
	{
		if(org( *tedge ) == e1)
		{
			if(dest( *tedge ) == e2)
			{
				return 1;
			}
		}
		else if(org( *tedge ) == e2)
		{
			if(dest( *tedge ) == e1)
			{
				esymself( *tedge );
				return 1;
			}
		}
	}

	// Search for the edge [e1, e2].
	point2tetorg( e1, *tedge );
	finddirection( tedge, e2 );
	if(dest( *tedge ) == e2)
	{
		return 1;
	}
	else
	{
		// Search for the edge [e2, e1].
		point2tetorg( e2, *tedge );
		finddirection( tedge, e1 );
		if(dest( *tedge ) == e1)
		{
			esymself( *tedge );
			return 1;
		}
	}


	// Go to the link face of e1.
	point2tetorg( e1, searchtet );
	enextesymself( searchtet );
	arraypool *tetlist = cavebdrylist;

	// Search e2.
	for(i = 0; i < 3; i++)
	{
		pt = apex( searchtet );
		if(pt == e2)
		{
			// Found. 'searchtet' is [#,#,e2,e1].
			eorgoppo( searchtet, *tedge ); // [e1,e2,#,#].
			return 1;
		}
		enextself( searchtet );
	}

	// Get the adjacent link face at 'searchtet'.
	fnext( searchtet, neightet );
	esymself( neightet );
	// assert(oppo(neightet) == e1);
	pt = apex( neightet );
	if(pt == e2)
	{
		// Found. 'neightet' is [#,#,e2,e1].
		eorgoppo( neightet, *tedge ); // [e1,e2,#,#].
		return 1;
	}

	// Continue searching in the link face of e1.
	infect( searchtet );
	tetlist->newindex( (void **)&parytet );
	*parytet = searchtet;
	infect( neightet );
	tetlist->newindex( (void **)&parytet );
	*parytet = neightet;

	done = 0;

	for(i = 0; ( i < tetlist->objects ) && !done; i++)
	{
		parytet = (triface *)fastlookup( tetlist, i );
		searchtet = *parytet;
		for(j = 0; ( j < 2 ) && !done; j++)
		{
			enextself( searchtet );
			fnext( searchtet, neightet );
			if(!infected( neightet ))
			{
				esymself( neightet );
				pt = apex( neightet );
				if(pt == e2)
				{
					// Found. 'neightet' is [#,#,e2,e1].
					eorgoppo( neightet, *tedge );
					done = 1;
				}
				else
				{
					infect( neightet );
					tetlist->newindex( (void **)&parytet );
					*parytet = neightet;
				}
			}
		} // j
	} // i 

	// Uninfect the list of visited tets.
	for(i = 0; i < tetlist->objects; i++)
	{
		parytet = (triface *)fastlookup( tetlist, i );
		uninfect( *parytet );
	}
	tetlist->restart( );

	return done;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// reduceedgesatvertex()    Reduce the number of edges at a given vertex.    //
//                                                                           //
// 'endptlist' contains the endpoints of edges connecting at the vertex.     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::reduceedgesatvertex( point startpt, arraypool* endptlist )
{
	triface searchtet;
	point *pendpt, *parypt;
	enum interresult dir;
	flipconstraints fc;
	int reduceflag;
	int count;
	int n, i, j;


	fc.remvert = startpt;
	fc.checkflipeligibility = 1;

	while(1)
	{

		count = 0;

		for(i = 0; i < endptlist->objects; i++)
		{
			pendpt = (point *)fastlookup( endptlist, i );
			if(*pendpt == dummypoint)
			{
				continue; // Do not reduce a virtual edge.
			}
			reduceflag = 0;
			// Find the edge.
			if(nonconvex)
			{
				if(getedge( startpt, *pendpt, &searchtet ))
				{
					dir = ACROSSVERT;
				}
				else
				{
					// The edge does not exist (was flipped).
					dir = INTERSECT;
				}
			}
			else
			{
				point2tetorg( startpt, searchtet );
				dir = finddirection( &searchtet, *pendpt );
			}
			if(dir == ACROSSVERT)
			{
				if(dest( searchtet ) == *pendpt)
				{
					// Do not flip a segment.
					if(!issubseg( searchtet ))
					{
						n = removeedgebyflips( &searchtet, &fc );
						if(n == 2)
						{
							reduceflag = 1;
						}
					}
				}
			}
			else
			{
				// The edge has been flipped.
				reduceflag = 1;
			}
			if(reduceflag)
			{
				count++;
				// Move the last vertex into this slot.
				j = endptlist->objects - 1;
				parypt = (point *)fastlookup( endptlist, j );
				*pendpt = *parypt;
				endptlist->objects--;
				i--;
			}
		} // i

		if(count == 0)
		{
			// No edge is reduced.
			break;
		}

	} // while (1)

	return (int)endptlist->objects;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// removevertexbyflips()    Remove a vertex by flips.                        //
//                                                                           //
// This routine attempts to remove the given vertex 'rempt' (p) from the     //
// tetrahedralization (T) by a sequence of flips.                            //
//                                                                           //
// The algorithm used here is a simple edge reduce method. Suppose there are //
// n edges connected at p. We try to reduce the number of edges by flipping  //
// any edge (not a segment) that is connecting at p.                         //
//                                                                           //
// Unless T is a Delaunay tetrahedralization, there is no guarantee that 'p' //
// can be successfully removed.                                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::removevertexbyflips( point steinerpt )
{
	triface *fliptets = NULL, wrktets[ 4 ];
	triface searchtet, spintet, neightet;
	face parentsh, spinsh, checksh;
	face leftseg, rightseg, checkseg;
	point lpt = NULL, rpt = NULL, apexpt; //, *parypt;
	flipconstraints fc;
	enum verttype vt;
	enum locateresult loc;
	int valence, removeflag;
	int slawson;
	int t1ver;
	int n, i;

	vt = pointtype( steinerpt );

	if(vt == FREESEGVERTEX)
	{
		sdecode( point2sh( steinerpt ), leftseg );
		leftseg.shver = 0;
		if(sdest( leftseg ) == steinerpt)
		{
			senext( leftseg, rightseg );
			spivotself( rightseg );
			rightseg.shver = 0;
		}
		else
		{
			rightseg = leftseg;
			senext2( rightseg, leftseg );
			spivotself( leftseg );
			leftseg.shver = 0;
		}
		lpt = sorg( leftseg );
		rpt = sdest( rightseg );
		if(b->verbose > 2)
		{
			myprintf( "      Removing Steiner point %d in segment (%d, %d).\n",
					pointmark( steinerpt ), pointmark( lpt ), pointmark( rpt ) );

		}
	}
	else if(vt == FREEFACETVERTEX)
	{
		if(b->verbose > 2)
		{
			myprintf( "      Removing Steiner point %d in facet.\n",
					pointmark( steinerpt ) );
		}
	}
	else if(vt == FREEVOLVERTEX)
	{
		if(b->verbose > 2)
		{
			myprintf( "      Removing Steiner point %d in volume.\n",
					pointmark( steinerpt ) );
		}
	}
	else if(vt == VOLVERTEX)
	{
		if(b->verbose > 2)
		{
			myprintf( "      Removing a point %d in volume.\n",
					pointmark( steinerpt ) );
		}
	}
	else
	{
		// It is not a Steiner point.
		return 0;
	}

	// Try to reduce the number of edges at 'p' by flips.
	getvertexstar( 1, steinerpt, cavetetlist, cavetetvertlist, NULL );
	cavetetlist->restart( ); // This list may be re-used.
	if(cavetetvertlist->objects > 3l)
	{
		valence = reduceedgesatvertex( steinerpt, cavetetvertlist );
	}
	else
	{
		valence = cavetetvertlist->objects;
	}
	cavetetvertlist->restart( );

	removeflag = 0;

	if(valence == 4)
	{
		// Only 4 vertices (4 tets) left! 'p' is inside the convex hull of the 4
		//   vertices. This case is due to that 'p' is not exactly on the segment.
		point2tetorg( steinerpt, searchtet );
		loc = INTETRAHEDRON;
		removeflag = 1;
	}
	else if(valence == 5)
	{
		// There are 5 edges.
		if(vt == FREESEGVERTEX)
		{
			sstpivot1( leftseg, searchtet );
			if(org( searchtet ) != steinerpt)
			{
				esymself( searchtet );
			}
			i = 0; // Count the numbe of tet at the edge [p,lpt].
			neightet.tet = NULL; // Init the face.
			spintet = searchtet;
			while(1)
			{
				i++;
				if(apex( spintet ) == rpt)
				{
					// Remember the face containing the edge [lpt, rpt].
					neightet = spintet;
				}
				fnextself( spintet );
				if(spintet.tet == searchtet.tet) break;
			}
			if(i == 3)
			{
				// This case has been checked below.
			}
			else if(i == 4)
			{
				// There are 4 tets sharing at [p,lpt]. There must be 4 tets sharing
				//   at [p,rpt].  There must be a face [p, lpt, rpt].  
				if(apex( neightet ) == rpt)
				{
					// The edge (segment) has been already recovered!  
					// Check if a 6-to-2 flip is possible (to remove 'p').
					// Let 'searchtet' be [p,d,a,b]
					esym( neightet, searchtet );
					enextself( searchtet );
					// Check if there are exactly three tets at edge [p,d].
					wrktets[ 0 ] = searchtet; // [p,d,a,b]
					for(i = 0; i < 2; i++)
					{
						fnext( wrktets[ i ], wrktets[ i + 1 ] ); // [p,d,b,c], [p,d,c,a]
					}
					if(apex( wrktets[ 0 ] ) == oppo( wrktets[ 2 ] ))
					{
						loc = ONFACE;
						removeflag = 1;
					}
				}
			}
		}
		else if(vt == FREEFACETVERTEX)
		{
			// It is possible to do a 6-to-2 flip to remove the vertex.
			point2tetorg( steinerpt, searchtet );
			// Get the three faces of 'searchtet' which share at p.
			//    All faces has p as origin.
			wrktets[ 0 ] = searchtet;
			wrktets[ 1 ] = searchtet;
			esymself( wrktets[ 1 ] );
			enextself( wrktets[ 1 ] );
			wrktets[ 2 ] = searchtet;
			eprevself( wrktets[ 2 ] );
			esymself( wrktets[ 2 ] );
			// All internal edges of the six tets have valance either 3 or 4.
			// Get one edge which has valance 3.
			searchtet.tet = NULL;
			for(i = 0; i < 3; i++)
			{
				spintet = wrktets[ i ];
				valence = 0;
				while(1)
				{
					valence++;
					fnextself( spintet );
					if(spintet.tet == wrktets[ i ].tet) break;
				}
				if(valence == 3)
				{
					// Found the edge.
					searchtet = wrktets[ i ];
					break;
				}
			}
			// Note, we do not detach the three subfaces at p.
			// They will be removed within a 4-to-1 flip.
			loc = ONFACE;
			removeflag = 1;
		}
		//removeflag = 1;
	}

	if(!removeflag)
	{
		if(vt == FREESEGVERTEX)
		{
			// Check is it possible to recover the edge [lpt,rpt].
			// The condition to check is:  Whether each tet containing 'leftseg' is
			//   adjacent to a tet containing 'rightseg'.
			sstpivot1( leftseg, searchtet );
			if(org( searchtet ) != steinerpt)
			{
				esymself( searchtet );
			}
			spintet = searchtet;
			while(1)
			{
				// Go to the bottom face of this tet.
				eprev( spintet, neightet );
				esymself( neightet );  // [steinerpt, p1, p2, lpt]
				// Get the adjacent tet.
				fsymself( neightet );  // [p1, steinerpt, p2, rpt]
				if(oppo( neightet ) != rpt)
				{
					// Found a non-matching adjacent tet.
					break;
				}
				fnextself( spintet );
				if(spintet.tet == searchtet.tet)
				{
					// 'searchtet' is [p,d,p1,p2].
					loc = ONEDGE;
					removeflag = 1;
					break;
				}
			}
		} // if (vt == FREESEGVERTEX)
	}

	if(!removeflag)
	{
		if(vt == FREESEGVERTEX)
		{
			// Check if the edge [lpt, rpt] exists.
			if(getedge( lpt, rpt, &searchtet ))
			{
				// We have recovered this edge. Shift the vertex into the volume.
				// We can recover this edge if the subfaces are not recovered yet.
				if(!checksubfaceflag)
				{
					// Remove the vertex from the surface mesh.
					//   This will re-create the segment [lpt, rpt] and re-triangulate
					//   all the facets at the segment.
					// Detach the subsegments from their surrounding tets.
					for(i = 0; i < 2; i++)
					{
						checkseg = ( i == 0 ) ? leftseg : rightseg;
						sstpivot1( checkseg, neightet );
						spintet = neightet;
						while(1)
						{
							tssdissolve1( spintet );
							fnextself( spintet );
							if(spintet.tet == neightet.tet) break;
						}
						sstdissolve1( checkseg );
					} // i
					slawson = 1; // Do lawson flip after removal.
					spivot( rightseg, parentsh ); // 'rightseg' has p as its origin.
					sremovevertex( steinerpt, &parentsh, &rightseg, slawson );
					// Clear the list for new subfaces.
					caveshbdlist->restart( );
					// Insert the new segment.
					sstbond1( rightseg, searchtet );
					spintet = searchtet;
					while(1)
					{
						tssbond1( spintet, rightseg );
						fnextself( spintet );
						if(spintet.tet == searchtet.tet) break;
					}
					// The Steiner point has been shifted into the volume.
					setpointtype( steinerpt, FREEVOLVERTEX );
					st_segref_count--;
					st_volref_count++;
					return 1;
				} // if (!checksubfaceflag)
			} // if (getedge(...))
		} // if (vt == FREESEGVERTEX)
	} // if (!removeflag)

	if(!removeflag)
	{
		return 0;
	}

	if(vt == FREESEGVERTEX)
	{
		// Detach the subsegments from their surronding tets.
		for(i = 0; i < 2; i++)
		{
			checkseg = ( i == 0 ) ? leftseg : rightseg;
			sstpivot1( checkseg, neightet );
			spintet = neightet;
			while(1)
			{
				tssdissolve1( spintet );
				fnextself( spintet );
				if(spintet.tet == neightet.tet) break;
			}
			sstdissolve1( checkseg );
		} // i
		if(checksubfaceflag)
		{
			// Detach the subfaces at the subsegments from their attached tets.
			for(i = 0; i < 2; i++)
			{
				checkseg = ( i == 0 ) ? leftseg : rightseg;
				spivot( checkseg, parentsh );
				if(parentsh.sh != NULL)
				{
					spinsh = parentsh;
					while(1)
					{
						stpivot( spinsh, neightet );
						if(neightet.tet != NULL)
						{
							tsdissolve( neightet );
						}
						sesymself( spinsh );
						stpivot( spinsh, neightet );
						if(neightet.tet != NULL)
						{
							tsdissolve( neightet );
						}
						stdissolve( spinsh );
						spivotself( spinsh ); // Go to the next subface.
						if(spinsh.sh == parentsh.sh) break;
					}
				}
			} // i
		} // if (checksubfaceflag)
	}

	if(loc == INTETRAHEDRON)
	{
		// Collect the four tets containing 'p'.
		fliptets = new triface[ 4 ];
		fliptets[ 0 ] = searchtet; // [p,d,a,b]
		for(i = 0; i < 2; i++)
		{
			fnext( fliptets[ i ], fliptets[ i + 1 ] ); // [p,d,b,c], [p,d,c,a]
		}
		eprev( fliptets[ 0 ], fliptets[ 3 ] );
		fnextself( fliptets[ 3 ] ); // it is [a,p,b,c]
		eprevself( fliptets[ 3 ] );
		esymself( fliptets[ 3 ] ); // [a,b,c,p].
		// Remove p by a 4-to-1 flip.
		//flip41(fliptets, 1, 0, 0);
		flip41( fliptets, 1, &fc );
		//recenttet = fliptets[0];
	}
	else if(loc == ONFACE)
	{
		// Let the original two tets be [a,b,c,d] and [b,a,c,e]. And p is in
		//   face [a,b,c].  Let 'searchtet' be the tet [p,d,a,b].
		// Collect the six tets containing 'p'.
		fliptets = new triface[ 6 ];
		fliptets[ 0 ] = searchtet; // [p,d,a,b]
		for(i = 0; i < 2; i++)
		{
			fnext( fliptets[ i ], fliptets[ i + 1 ] ); // [p,d,b,c], [p,d,c,a]
		}
		eprev( fliptets[ 0 ], fliptets[ 3 ] );
		fnextself( fliptets[ 3 ] ); // [a,p,b,e]
		esymself( fliptets[ 3 ] );  // [p,a,e,b]
		eprevself( fliptets[ 3 ] ); // [e,p,a,b]
		for(i = 3; i < 5; i++)
		{
			fnext( fliptets[ i ], fliptets[ i + 1 ] ); // [e,p,b,c], [e,p,c,a]
		}
		if(vt == FREEFACETVERTEX)
		{
			// We need to determine the location of three subfaces at p.
			valence = 0; // Re-use it.
			// Check if subfaces are all located in the lower three tets.
			//   i.e., [e,p,a,b], [e,p,b,c], and [e,p,c,a].
			for(i = 3; i < 6; i++)
			{
				if(issubface( fliptets[ i ] )) valence++;
			}
			if(valence > 0)
			{
				// We must do 3-to-2 flip in the upper part. We simply re-arrange
				//   the six tets.
				for(i = 0; i < 3; i++)
				{
					esym( fliptets[ i + 3 ], wrktets[ i ] );
					esym( fliptets[ i ], fliptets[ i + 3 ] );
					fliptets[ i ] = wrktets[ i ];
				}
				// Swap the last two pairs, i.e., [1]<->[[2], and [4]<->[5]
				wrktets[ 1 ] = fliptets[ 1 ];
				fliptets[ 1 ] = fliptets[ 2 ];
				fliptets[ 2 ] = wrktets[ 1 ];
				wrktets[ 1 ] = fliptets[ 4 ];
				fliptets[ 4 ] = fliptets[ 5 ];
				fliptets[ 5 ] = wrktets[ 1 ];
			}
		}
		// Remove p by a 6-to-2 flip, which is a combination of two flips:
		//   a 3-to-2 (deletes the edge [e,p]), and
		//   a 4-to-1 (deletes the vertex p).
		// First do a 3-to-2 flip on [e,p,a,b],[e,p,b,c],[e,p,c,a]. It creates
		//   two new tets: [a,b,c,p] and [b,a,c,e].  The new tet [a,b,c,p] is
		//   degenerate (has zero volume). It will be deleted in the followed
		//   4-to-1 flip.
		//flip32(&(fliptets[3]), 1, 0, 0);
		flip32( &( fliptets[ 3 ] ), 1, &fc );
		// Second do a 4-to-1 flip on [p,d,a,b],[p,d,b,c],[p,d,c,a],[a,b,c,p].
		//   This creates a new tet [a,b,c,d].
		//flip41(fliptets, 1, 0, 0);
		flip41( fliptets, 1, &fc );
		//recenttet = fliptets[0];
	}
	else if(loc == ONEDGE)
	{
		// Let the original edge be [e,d] and p is in [e,d]. Assume there are n
		//   tets sharing at edge [e,d] originally.  We number the link vertices
		//   of [e,d]: p_0, p_1, ..., p_n-1. 'searchtet' is [p,d,p_0,p_1].
		// Count the number of tets at edge [e,p] and [p,d] (this is n).
		n = 0;
		spintet = searchtet;
		while(1)
		{
			n++;
			fnextself( spintet );
			if(spintet.tet == searchtet.tet) break;
		}
		// Collect the 2n tets containing 'p'.
		fliptets = new triface[ 2 * n ];
		fliptets[ 0 ] = searchtet; // [p,b,p_0,p_1] 
		for(i = 0; i < ( n - 1 ); i++)
		{
			fnext( fliptets[ i ], fliptets[ i + 1 ] ); // [p,d,p_i,p_i+1].
		}
		eprev( fliptets[ 0 ], fliptets[ n ] );
		fnextself( fliptets[ n ] ); // [p_0,p,p_1,e]
		esymself( fliptets[ n ] );  // [p,p_0,e,p_1]
		eprevself( fliptets[ n ] ); // [e,p,p_0,p_1]
		for(i = n; i < ( 2 * n - 1 ); i++)
		{
			fnext( fliptets[ i ], fliptets[ i + 1 ] ); // [e,p,p_i,p_i+1].
		}
		// Remove p by a 2n-to-n flip, it is a sequence of n flips:
		// - Do a 2-to-3 flip on 
		//     [p_0,p_1,p,d] and 
		//     [p,p_1,p_0,e].
		//   This produces: 
		//     [e,d,p_0,p_1], 
		//     [e,d,p_1,p] (degenerated), and 
		//     [e,d,p,p_0] (degenerated).
		wrktets[ 0 ] = fliptets[ 0 ]; // [p,d,p_0,p_1]
		eprevself( wrktets[ 0 ] );    // [p_0,p,d,p_1]
		esymself( wrktets[ 0 ] );     // [p,p_0,p_1,d]
		enextself( wrktets[ 0 ] );    // [p_0,p_1,p,d] [0]
		wrktets[ 1 ] = fliptets[ n ]; // [e,p,p_0,p_1]
		enextself( wrktets[ 1 ] );    // [p,p_0,e,p_1]
		esymself( wrktets[ 1 ] );     // [p_0,p,p_1,e]
		eprevself( wrktets[ 1 ] );    // [p_1,p_0,p,e] [1]
		//flip23(wrktets, 1, 0, 0);
		flip23( wrktets, 1, &fc );
		// Save the new tet [e,d,p,p_0] (degenerated).
		fliptets[ n ] = wrktets[ 2 ];
		// Save the new tet [e,d,p_0,p_1].
		fliptets[ 0 ] = wrktets[ 0 ];
		// - Repeat from i = 1 to n-2: (n - 2) flips
		//   - Do a 3-to-2 flip on 
		//       [p,p_i,d,e], 
		//       [p,p_i,e,p_i+1], and 
		//       [p,p_i,p_i+1,d]. 
		//     This produces: 
		//       [d,e,p_i+1,p_i], and
		//       [e,d,p_i+1,p] (degenerated).
		for(i = 1; i < ( n - 1 ); i++)
		{
			wrktets[ 0 ] = wrktets[ 1 ]; // [e,d,p_i,p] (degenerated).
			enextself( wrktets[ 0 ] );   // [d,p_i,e,p] (...)
			esymself( wrktets[ 0 ] );    // [p_i,d,p,e] (...) 
			eprevself( wrktets[ 0 ] );   // [p,p_i,d,e] (degenerated) [0].
			wrktets[ 1 ] = fliptets[ n + i ];  // [e,p,p_i,p_i+1]
			enextself( wrktets[ 1 ] );       // [p,p_i,e,p_i+1] [1]
			wrktets[ 2 ] = fliptets[ i ]; // [p,d,p_i,p_i+1]
			eprevself( wrktets[ 2 ] );    // [p_i,p,d,p_i+1]
			esymself( wrktets[ 2 ] );     // [p,p_i,p_i+1,d] [2]
			//flip32(wrktets, 1, 0, 0);
			flip32( wrktets, 1, &fc );
			// Save the new tet [e,d,p_i,p_i+1].         // FOR DEBUG ONLY
			fliptets[ i ] = wrktets[ 0 ]; // [d,e,p_i+1,p_i] // FOR DEBUG ONLY
			esymself( fliptets[ i ] );    // [e,d,p_i,p_i+1] // FOR DEBUG ONLY
		}
		// - Do a 4-to-1 flip on 
		//     [p,p_0,e,d],     [d,e,p_0,p],
		//     [p,p_0,d,p_n-1], [e,p_n-1,p_0,p], 
		//     [p,p_0,p_n-1,e], [p_0,p_n-1,d,p], and
		//     [e,d,p_n-1,p]. 
		//   This produces 
		//     [e,d,p_n-1,p_0] and 
		//     deletes p.
		wrktets[ 3 ] = wrktets[ 1 ];  // [e,d,p_n-1,p] (degenerated) [3]
		wrktets[ 0 ] = fliptets[ n ]; // [e,d,p,p_0] (degenerated)
		eprevself( wrktets[ 0 ] );    // [p,e,d,p_0] (...)
		esymself( wrktets[ 0 ] );     // [e,p,p_0,d] (...)
		enextself( wrktets[ 0 ] );    // [p,p_0,e,d] (degenerated) [0]
		wrktets[ 1 ] = fliptets[ n - 1 ];   // [p,d,p_n-1,p_0]
		esymself( wrktets[ 1 ] );         // [d,p,p_0,p_n-1]
		enextself( wrktets[ 1 ] );        // [p,p_0,d,p_n-1] [1]
		wrktets[ 2 ] = fliptets[ 2 * n - 1 ]; // [e,p,p_n-1,p_0]
		enextself( wrktets[ 2 ] );        // [p_p_n-1,e,p_0]
		esymself( wrktets[ 2 ] );         // [p_n-1,p,p_0,e]
		enextself( wrktets[ 2 ] );        // [p,p_0,p_n-1,e] [2]
		//flip41(wrktets, 1, 0, 0);
		flip41( wrktets, 1, &fc );
		// Save the new tet [e,d,p_n-1,p_0]             // FOR DEBUG ONLY
		fliptets[ n - 1 ] = wrktets[ 0 ];  // [e,d,p_n-1,p_0] // FOR DEBUG ONLY
		//recenttet = fliptets[0];
	}

	delete[ ] fliptets;

	if(vt == FREESEGVERTEX)
	{
		// Remove the vertex from the surface mesh.
		//   This will re-create the segment [lpt, rpt] and re-triangulate
		//   all the facets at the segment.
		// Only do lawson flip when subfaces are not recovery yet.
		slawson = ( checksubfaceflag ? 0 : 1 );
		spivot( rightseg, parentsh ); // 'rightseg' has p as its origin.
		sremovevertex( steinerpt, &parentsh, &rightseg, slawson );

		// The original segment is returned in 'rightseg'. 
		rightseg.shver = 0;
		// Insert the new segment.
		point2tetorg( lpt, searchtet );
		finddirection( &searchtet, rpt );
		sstbond1( rightseg, searchtet );
		spintet = searchtet;
		while(1)
		{
			tssbond1( spintet, rightseg );
			fnextself( spintet );
			if(spintet.tet == searchtet.tet) break;
		}

		if(checksubfaceflag)
		{
			// Insert subfaces at segment [lpt,rpt] into the tetrahedralization.
			spivot( rightseg, parentsh );
			if(parentsh.sh != NULL)
			{
				spinsh = parentsh;
				while(1)
				{
					if(sorg( spinsh ) != lpt)
					{
						sesymself( spinsh );
					}
					apexpt = sapex( spinsh );
					// Find the adjacent tet of [lpt,rpt,apexpt];
					spintet = searchtet;
					while(1)
					{
						if(apex( spintet ) == apexpt)
						{
							tsbond( spintet, spinsh );
							sesymself( spinsh ); // Get to another side of this face.
							fsym( spintet, neightet );
							tsbond( neightet, spinsh );
							sesymself( spinsh ); // Get back to the original side.
							break;
						}
						fnextself( spintet );
					}
					spivotself( spinsh );
					if(spinsh.sh == parentsh.sh) break;
				}
			}
		} // if (checksubfaceflag)

		// Clear the set of new subfaces.
		caveshbdlist->restart( );
	} // if (vt == FREESEGVERTEX)

	// The point has been removed.
	if(pointtype( steinerpt ) != UNUSEDVERTEX)
	{
		setpointtype( steinerpt, UNUSEDVERTEX );
		unuverts++;
	}
	if(vt != VOLVERTEX)
	{
		// Update the correspinding counters.
		if(vt == FREESEGVERTEX)
		{
			st_segref_count--;
		}
		else if(vt == FREEFACETVERTEX)
		{
			st_facref_count--;
		}
		else if(vt == FREEVOLVERTEX)
		{
			st_volref_count--;
		}
		if(steinerleft > 0) steinerleft++;
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// suppressbdrysteinerpoint()    Suppress a boundary Steiner point           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::suppressbdrysteinerpoint( point steinerpt )
{
	face parentsh, spinsh, *parysh;
	face leftseg, rightseg;
	point lpt = NULL, rpt = NULL;
	int i;

	verttype vt = pointtype( steinerpt );

	if(vt == FREESEGVERTEX)
	{
		sdecode( point2sh( steinerpt ), leftseg );
		leftseg.shver = 0;
		if(sdest( leftseg ) == steinerpt)
		{
			senext( leftseg, rightseg );
			spivotself( rightseg );
			rightseg.shver = 0;
		}
		else
		{
			rightseg = leftseg;
			senext2( rightseg, leftseg );
			spivotself( leftseg );
			leftseg.shver = 0;
		}
		lpt = sorg( leftseg );
		rpt = sdest( rightseg );
		if(b->verbose > 2)
		{
			myprintf( "      Suppressing Steiner point %d in segment (%d, %d).\n",
					pointmark( steinerpt ), pointmark( lpt ), pointmark( rpt ) );
		}
		// Get all subfaces at the left segment [lpt, steinerpt].
		spivot( leftseg, parentsh );
		if(parentsh.sh != NULL)
		{
			// It is not a dangling segment.
			spinsh = parentsh;
			while(1)
			{
				cavesegshlist->newindex( (void **)&parysh );
				*parysh = spinsh;
				// Orient the face consistently. 
				if(sorg( *parysh ) != sorg( parentsh )) sesymself( *parysh );
				spivotself( spinsh );
				if(spinsh.sh == NULL) break;
				if(spinsh.sh == parentsh.sh) break;
			}
		}
		if(cavesegshlist->objects < 2)
		{
			// It is a single segment. Not handle it yet.
			cavesegshlist->restart( );
			return 0;
		}
	}
	else if(vt == FREEFACETVERTEX)
	{
		if(b->verbose > 2)
		{
			myprintf( "      Suppressing Steiner point %d from facet.\n",
					pointmark( steinerpt ) );
		}
		sdecode( point2sh( steinerpt ), parentsh );
		// A facet Steiner point. There are exactly two sectors.
		for(i = 0; i < 2; i++)
		{
			cavesegshlist->newindex( (void **)&parysh );
			*parysh = parentsh;
			sesymself( parentsh );
		}
	}
	else
	{
		return 0;
	}

	triface searchtet, neightet, *parytet;
	point pa, pb, pc, pd;
	REAL v1[ 3 ], v2[ 3 ], len, u;

	REAL startpt[ 3 ] = { 0, }, samplept[ 3 ] = { 0, }, candpt[ 3 ] = { 0, };
	REAL ori, minvol, smallvol;
	int samplesize;
	int it, j, k;

	int n = (int)cavesegshlist->objects;
	point *newsteiners = new point[ n ];
	for(i = 0; i < n; i++) newsteiners[ i ] = NULL;

	// Search for each sector an interior vertex. 
	for(i = 0; i < cavesegshlist->objects; i++)
	{
		parysh = (face *)fastlookup( cavesegshlist, i );
		stpivot( *parysh, searchtet );
		// Skip it if it is outside.
		if(ishulltet( searchtet )) continue;
		// Get the "half-ball". Tets in 'cavetetlist' all contain 'steinerpt' as
		//   opposite.  Subfaces in 'caveshlist' all contain 'steinerpt' as apex.
		//   Moreover, subfaces are oriented towards the interior of the ball.
		setpoint2tet( steinerpt, encode( searchtet ) );
		getvertexstar( 0, steinerpt, cavetetlist, NULL, caveshlist );
		// Calculate the searching vector.
		pa = sorg( *parysh );
		pb = sdest( *parysh );
		pc = sapex( *parysh );
		facenormal( pa, pb, pc, v1, 1, NULL );
		len = sqrt( dot( v1, v1 ) );
		v1[ 0 ] /= len;
		v1[ 1 ] /= len;
		v1[ 2 ] /= len;
		if(vt == FREESEGVERTEX)
		{
			parysh = (face *)fastlookup( cavesegshlist, ( i + 1 ) % n );
			pd = sapex( *parysh );
			facenormal( pb, pa, pd, v2, 1, NULL );
			len = sqrt( dot( v2, v2 ) );
			v2[ 0 ] /= len;
			v2[ 1 ] /= len;
			v2[ 2 ] /= len;
			// Average the two vectors.
			v1[ 0 ] = 0.5 * ( v1[ 0 ] + v2[ 0 ] );
			v1[ 1 ] = 0.5 * ( v1[ 1 ] + v2[ 1 ] );
			v1[ 2 ] = 0.5 * ( v1[ 2 ] + v2[ 2 ] );
		}
		// Search the intersection of the ray starting from 'steinerpt' to
		//   the search direction 'v1' and the shell of the half-ball.
		// - Construct an endpoint.
		len = distance( pa, pb );
		v2[ 0 ] = steinerpt[ 0 ] + len * v1[ 0 ];
		v2[ 1 ] = steinerpt[ 1 ] + len * v1[ 1 ];
		v2[ 2 ] = steinerpt[ 2 ] + len * v1[ 2 ];
		for(j = 0; j < cavetetlist->objects; j++)
		{
			parytet = (triface *)fastlookup( cavetetlist, j );
			pa = org( *parytet );
			pb = dest( *parytet );
			pc = apex( *parytet );
			// Test if the ray startpt->v2 lies in the cone: where 'steinerpt'
			//   is the apex, and three sides are defined by the triangle 
			//   [pa, pb, pc].
			ori = orient3d( steinerpt, pa, pb, v2 );
			if(ori >= 0)
			{
				ori = orient3d( steinerpt, pb, pc, v2 );
				if(ori >= 0)
				{
					ori = orient3d( steinerpt, pc, pa, v2 );
					if(ori >= 0)
					{
						// Found! Calculate the intersection.
						planelineint( pa, pb, pc, steinerpt, v2, startpt, &u );
						break;
					}
				}
			}
		} // j
		// Close the ball by adding the subfaces.
		for(j = 0; j < caveshlist->objects; j++)
		{
			parysh = (face *)fastlookup( caveshlist, j );
			stpivot( *parysh, neightet );
			cavetetlist->newindex( (void **)&parytet );
			*parytet = neightet;
		}
		// Search a best point inside the segment [startpt, steinerpt].
		it = 0;
		samplesize = 100;
		v1[ 0 ] = steinerpt[ 0 ] - startpt[ 0 ];
		v1[ 1 ] = steinerpt[ 1 ] - startpt[ 1 ];
		v1[ 2 ] = steinerpt[ 2 ] - startpt[ 2 ];
		minvol = -1.0;
		while(it < 3)
		{
			for(j = 1; j < samplesize - 1; j++)
			{
				samplept[ 0 ] = startpt[ 0 ] + ( (REAL)j / (REAL)samplesize ) * v1[ 0 ];
				samplept[ 1 ] = startpt[ 1 ] + ( (REAL)j / (REAL)samplesize ) * v1[ 1 ];
				samplept[ 2 ] = startpt[ 2 ] + ( (REAL)j / (REAL)samplesize ) * v1[ 2 ];
				// Find the minimum volume for 'samplept'.
				smallvol = -1;
				for(k = 0; k < cavetetlist->objects; k++)
				{
					parytet = (triface *)fastlookup( cavetetlist, k );
					pa = org( *parytet );
					pb = dest( *parytet );
					pc = apex( *parytet );
					ori = orient3d( pb, pa, pc, samplept );
					if(ori <= 0)
					{
						break; // An invalid tet.
					}
					if(smallvol == -1)
					{
						smallvol = ori;
					}
					else
					{
						if(ori < smallvol) smallvol = ori;
					}
				} // k
				if(k == cavetetlist->objects)
				{
					// Found a valid point. Remember it.
					if(minvol == -1.0)
					{
						candpt[ 0 ] = samplept[ 0 ];
						candpt[ 1 ] = samplept[ 1 ];
						candpt[ 2 ] = samplept[ 2 ];
						minvol = smallvol;
					}
					else
					{
						if(minvol < smallvol)
						{
							// It is a better location. Remember it.
							candpt[ 0 ] = samplept[ 0 ];
							candpt[ 1 ] = samplept[ 1 ];
							candpt[ 2 ] = samplept[ 2 ];
							minvol = smallvol;
						}
						else
						{
							// No improvement of smallest volume. 
							// Since we are searching along the line [startpt, steinerpy],
							// The smallest volume can only be decreased later.
							break;
						}
					}
				}
			} // j
			if(minvol > 0) break;
			samplesize *= 10;
			it++;
		} // while (it < 3)
		if(minvol == -1.0)
		{
			// Failed to find a valid point.
			cavetetlist->restart( );
			caveshlist->restart( );
			break;
		}
		// Create a new Steiner point inside this section.
		makepoint( &( newsteiners[ i ] ), FREEVOLVERTEX );
		newsteiners[ i ][ 0 ] = candpt[ 0 ];
		newsteiners[ i ][ 1 ] = candpt[ 1 ];
		newsteiners[ i ][ 2 ] = candpt[ 2 ];
		cavetetlist->restart( );
		caveshlist->restart( );
		// UV: TODO
	} // i

	if(i < cavesegshlist->objects)
	{
		// Failed to suppress the vertex.
		for(; i > 0; i--)
		{
			if(newsteiners[ i - 1 ] != NULL)
			{
				pointdealloc( newsteiners[ i - 1 ] );
			}
		}
		delete[ ] newsteiners;
		cavesegshlist->restart( );
		return 0;
	}

	// Remove p from the segment or the facet.
	triface newtet, newface, spintet;
	face newsh, neighsh;
	face *splitseg, checkseg;
	int slawson = 0; // Do not do flip afterword.
	int t1ver;

	if(vt == FREESEGVERTEX)
	{
		// Detach 'leftseg' and 'rightseg' from their adjacent tets.
		//   These two subsegments will be deleted. 
		sstpivot1( leftseg, neightet );
		spintet = neightet;
		while(1)
		{
			tssdissolve1( spintet );
			fnextself( spintet );
			if(spintet.tet == neightet.tet) break;
		}
		sstpivot1( rightseg, neightet );
		spintet = neightet;
		while(1)
		{
			tssdissolve1( spintet );
			fnextself( spintet );
			if(spintet.tet == neightet.tet) break;
		}
	}

	// Loop through all sectors bounded by facets at this segment.
	//   Within each sector, create a new Steiner point 'np', and replace 'p'
	//   by 'np' for all tets in this sector.
	for(i = 0; i < cavesegshlist->objects; i++)
	{
		parysh = (face *)fastlookup( cavesegshlist, i );
		// 'parysh' is the face [lpt, steinerpt, #].
		stpivot( *parysh, neightet );
		// Get all tets in this sector.
		setpoint2tet( steinerpt, encode( neightet ) );
		getvertexstar( 0, steinerpt, cavetetlist, NULL, caveshlist );
		if(!ishulltet( neightet ))
		{
			// Within each tet in the ball, replace 'p' by 'np'.
			for(j = 0; j < cavetetlist->objects; j++)
			{
				parytet = (triface *)fastlookup( cavetetlist, j );
				setoppo( *parytet, newsteiners[ i ] );
			} // j
			// Point to a parent tet.
			parytet = (triface *)fastlookup( cavetetlist, 0 );
			setpoint2tet( newsteiners[ i ], (tetrahedron)( parytet->tet ) );
			st_volref_count++;
			if(steinerleft > 0) steinerleft--;
		}
		// Disconnect the set of boundary faces. They're temporarily open faces.
		//   They will be connected to the new tets after 'p' is removed.
		for(j = 0; j < caveshlist->objects; j++)
		{
			// Get a boundary face.
			parysh = (face *)fastlookup( caveshlist, j );
			stpivot( *parysh, neightet );
			//assert(apex(neightet) == newpt);
			// Clear the connection at this face.
			dissolve( neightet );
			tsdissolve( neightet );
		}
		// Clear the working lists.
		cavetetlist->restart( );
		caveshlist->restart( );
	} // i
	cavesegshlist->restart( );

	if(vt == FREESEGVERTEX)
	{
		spivot( rightseg, parentsh ); // 'rightseg' has p as its origin.
		splitseg = &rightseg;
	}
	else
	{
		if(sdest( parentsh ) == steinerpt)
		{
			senextself( parentsh );
		}
		else if(sapex( parentsh ) == steinerpt)
		{
			senext2self( parentsh );
		}
		splitseg = NULL;
	}
	sremovevertex( steinerpt, &parentsh, splitseg, slawson );

	if(vt == FREESEGVERTEX)
	{
		// The original segment is returned in 'rightseg'. 
		rightseg.shver = 0;
	}

	// For each new subface, create two new tets at each side of it.
	//   Both of the two new tets have its opposite be dummypoint. 
	for(i = 0; i < caveshbdlist->objects; i++)
	{
		parysh = (face *)fastlookup( caveshbdlist, i );
		sinfect( *parysh ); // Mark it for connecting new tets.
		newsh = *parysh;
		pa = sorg( newsh );
		pb = sdest( newsh );
		pc = sapex( newsh );
		maketetrahedron( &newtet );
		maketetrahedron( &neightet );
		setvertices( newtet, pa, pb, pc, dummypoint );
		setvertices( neightet, pb, pa, pc, dummypoint );
		bond( newtet, neightet );
		tsbond( newtet, newsh );
		sesymself( newsh );
		tsbond( neightet, newsh );
	}
	// Temporarily increase the hullsize.
	hullsize += ( caveshbdlist->objects * 2l );

	if(vt == FREESEGVERTEX)
	{
		// Connecting new tets at the recovered segment.
		spivot( rightseg, parentsh );
		spinsh = parentsh;
		while(1)
		{
			if(sorg( spinsh ) != lpt) sesymself( spinsh );
			// Get the new tet at this subface.
			stpivot( spinsh, newtet );
			tssbond1( newtet, rightseg );
			// Go to the other face at this segment.
			spivot( spinsh, neighsh );
			if(sorg( neighsh ) != lpt) sesymself( neighsh );
			sesymself( neighsh );
			stpivot( neighsh, neightet );
			tssbond1( neightet, rightseg );
			sstbond1( rightseg, neightet );
			// Connecting two adjacent tets at this segment.
			esymself( newtet );
			esymself( neightet );
			// Connect the two tets (at rightseg) together.
			bond( newtet, neightet );
			// Go to the next subface.
			spivotself( spinsh );
			if(spinsh.sh == parentsh.sh) break;
		}
	}

	// Connecting new tets at new subfaces together.
	for(i = 0; i < caveshbdlist->objects; i++)
	{
		parysh = (face *)fastlookup( caveshbdlist, i );
		newsh = *parysh;
		//assert(sinfected(newsh));
		// Each new subface contains two new tets.
		for(k = 0; k < 2; k++)
		{
			stpivot( newsh, newtet );
			for(j = 0; j < 3; j++)
			{
				// Check if this side is open.
				esym( newtet, newface );
				if(newface.tet[ newface.ver & 3 ] == NULL)
				{
					// An open face. Connect it to its adjacent tet.
					sspivot( newsh, checkseg );
					if(checkseg.sh != NULL)
					{
						// A segment. It must not be the recovered segment.
						tssbond1( newtet, checkseg );
						sstbond1( checkseg, newtet );
					}
					spivot( newsh, neighsh );
					if(neighsh.sh != NULL)
					{
						// The adjacent subface exists. It's not a dangling segment.
						if(sorg( neighsh ) != sdest( newsh )) sesymself( neighsh );
						stpivot( neighsh, neightet );
						if(sinfected( neighsh ))
						{
							esymself( neightet );
						}
						else
						{
							// Search for an open face at this edge.
							spintet = neightet;
							while(1)
							{
								esym( spintet, searchtet );
								fsym( searchtet, spintet );
								if(spintet.tet == NULL) break;
							}
							// Found an open face at 'searchtet'.
							neightet = searchtet;
						}
					}
					else
					{
						// The edge (at 'newsh') is a dangling segment.
						// Get an adjacent tet at this segment.
						sstpivot1( checkseg, neightet );
						if(org( neightet ) != sdest( newsh )) esymself( neightet );
						// Search for an open face at this edge.
						spintet = neightet;
						while(1)
						{
							esym( spintet, searchtet );
							fsym( searchtet, spintet );
							if(spintet.tet == NULL) break;
						}
						// Found an open face at 'searchtet'.
						neightet = searchtet;
					}
					pc = apex( newface );
					if(apex( neightet ) == steinerpt)
					{
						// Exterior case. The 'neightet' is a hull tet which contain
						//   'steinerpt'. It will be deleted after 'steinerpt' is removed. 
						caveoldtetlist->newindex( (void **)&parytet );
						*parytet = neightet;
						// Connect newface to the adjacent hull tet of 'neightet', which
						//   has the same edge as 'newface', and does not has 'steinerpt'.
						fnextself( neightet );
					}
					else
					{
						if(pc == dummypoint)
						{
							if(apex( neightet ) != dummypoint)
							{
								setapex( newface, apex( neightet ) );
								// A hull tet has turned into an interior tet.
								hullsize--; // Must update the hullsize.
							}
						}
					}
					bond( newface, neightet );
				} // if (newface.tet[newface.ver & 3] == NULL)
				enextself( newtet );
				senextself( newsh );
			} // j
			sesymself( newsh );
		} // k
	} // i

	// Unmark all new subfaces.
	for(i = 0; i < caveshbdlist->objects; i++)
	{
		parysh = (face *)fastlookup( caveshbdlist, i );
		suninfect( *parysh );
	}
	caveshbdlist->restart( );

	if(caveoldtetlist->objects > 0l)
	{
		// Delete hull tets which contain 'steinerpt'.
		for(i = 0; i < caveoldtetlist->objects; i++)
		{
			parytet = (triface *)fastlookup( caveoldtetlist, i );
			tetrahedrondealloc( parytet->tet );
		}
		// Must update the hullsize.
		hullsize -= caveoldtetlist->objects;
		caveoldtetlist->restart( );
	}

	setpointtype( steinerpt, UNUSEDVERTEX );
	unuverts++;
	if(vt == FREESEGVERTEX)
	{
		st_segref_count--;
	}
	else
	{ // vt == FREEFACETVERTEX
		st_facref_count--;
	}
	if(steinerleft > 0) steinerleft++;  // We've removed a Steiner points.


	point *parypt;
	int steinercount = 0;

	int bak_fliplinklevel = b->fliplinklevel;
	b->fliplinklevel = 100000; // Unlimited flip level.

	// Try to remove newly added Steiner points.
	for(i = 0; i < n; i++)
	{
		if(newsteiners[ i ] != NULL)
		{
			if(!removevertexbyflips( newsteiners[ i ] ))
			{
				if(b->supsteiner_level > 0)
				{ // Not -Y/0
					// Save it in subvertstack for removal.
					subvertstack->newindex( (void **)&parypt );
					*parypt = newsteiners[ i ];
				}
				steinercount++;
			}
		}
	}

	b->fliplinklevel = bak_fliplinklevel;

	if(steinercount > 0)
	{
		if(b->verbose > 2)
		{
			myprintf( "      Added %d interior Steiner points.\n", steinercount );
		}
	}

	delete[ ] newsteiners;

	return 1;
}


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// suppresssteinerpoints()    Suppress Steiner points.                       //
//                                                                           //
// All Steiner points have been saved in 'subvertstack' in the routines      //
// carveholes() and suppresssteinerpoint().                                  //
// Each Steiner point is either removed or shifted into the interior.        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::suppresssteinerpoints( )
{

	if(!b->quiet)
	{
		myprintf( "Suppressing Steiner points ...\n" );
	}

	point rempt, *parypt;

	int bak_fliplinklevel = b->fliplinklevel;
	b->fliplinklevel = 100000; // Unlimited flip level.
	int suppcount = 0, remcount = 0;
	int i;

	// Try to suppress boundary Steiner points.
	for(i = 0; i < subvertstack->objects; i++)
	{
		parypt = (point *)fastlookup( subvertstack, i );
		rempt = *parypt;
		if(pointtype( rempt ) != UNUSEDVERTEX)
		{
			if(( pointtype( rempt ) == FREESEGVERTEX ) ||
				( pointtype( rempt ) == FREEFACETVERTEX ))
			{
				if(suppressbdrysteinerpoint( rempt ))
				{
					suppcount++;
				}
			}
		}
	} // i

	if(suppcount > 0)
	{
		if(b->verbose)
		{
			myprintf( "  Suppressed %d boundary Steiner points.\n", suppcount );
		}
	}

	if(b->supsteiner_level > 0)
	{ // -Y/1
		for(i = 0; i < subvertstack->objects; i++)
		{
			parypt = (point *)fastlookup( subvertstack, i );
			rempt = *parypt;
			if(pointtype( rempt ) != UNUSEDVERTEX)
			{
				if(pointtype( rempt ) == FREEVOLVERTEX)
				{
					if(removevertexbyflips( rempt ))
					{
						remcount++;
					}
				}
			}
		}
	}

	if(remcount > 0)
	{
		if(b->verbose)
		{
			myprintf( "  Removed %d interior Steiner points.\n", remcount );
		}
	}

	b->fliplinklevel = bak_fliplinklevel;

	if(b->supsteiner_level > 1)
	{ // -Y/2
		// Smooth interior Steiner points.
		optparameters opm;
		triface *parytet;
		point *ppt;
		REAL ori;
		int smtcount, count, ivcount;
		int nt, j;

		// Point smooth options.
		opm.max_min_volume = 1;
		opm.numofsearchdirs = 20;
		opm.searchstep = 0.001;
		opm.maxiter = 30; // Limit the maximum iterations.

		smtcount = 0;

		do
		{

			nt = 0;

			while(1)
			{
				count = 0;
				ivcount = 0; // Clear the inverted count.

				for(i = 0; i < subvertstack->objects; i++)
				{
					parypt = (point *)fastlookup( subvertstack, i );
					rempt = *parypt;
					if(pointtype( rempt ) == FREEVOLVERTEX)
					{
						getvertexstar( 1, rempt, cavetetlist, NULL, NULL );
						// Calculate the initial smallest volume (maybe zero or negative).
						for(j = 0; j < cavetetlist->objects; j++)
						{
							parytet = (triface *)fastlookup( cavetetlist, j );
							ppt = (point *)&( parytet->tet[ 4 ] );
							ori = orient3dfast( ppt[ 1 ], ppt[ 0 ], ppt[ 2 ], ppt[ 3 ] );
							if(j == 0)
							{
								opm.initval = ori;
							}
							else
							{
								if(opm.initval > ori) opm.initval = ori;
							}
						}
						if(smoothpoint( rempt, cavetetlist, 1, &opm ))
						{
							count++;
						}
						if(opm.imprval <= 0.0)
						{
							ivcount++; // The mesh contains inverted elements.
						}
						cavetetlist->restart( );
					}
				} // i

				smtcount += count;

				if(count == 0)
				{
					// No point has been smoothed.
					break;
				}

				nt++;
				if(nt > 2)
				{
					break; // Already three iterations.
				}
			} // while

			if(ivcount > 0)
			{
				// There are inverted elements!
				if(opm.maxiter > 0)
				{
					// Set unlimited smoothing steps. Try again.
					opm.numofsearchdirs = 30;
					opm.searchstep = 0.0001;
					opm.maxiter = -1;
					continue;
				}
			}

			break;
		}
		while(1); // Additional loop for (ivcount > 0)

		if(ivcount > 0)
		{
			myprintf( "BUG Report!  The mesh contain inverted elements.\n" );
		}

		if(b->verbose)
		{
			if(smtcount > 0)
			{
				myprintf( "  Smoothed %d Steiner points.\n", smtcount );
			}
		}
	} // -Y2

	subvertstack->restart( );

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// recoverboundary()    Recover segments and facets.                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::recoverboundary( clock_t& tv )
{
	arraypool *misseglist, *misshlist;
	arraypool *bdrysteinerptlist;
	face searchsh, *parysh;
	face searchseg, *paryseg;
	point rempt, *parypt;
	long ms; // The number of missing segments/subfaces.
	int nit; // The number of iterations.
	int s, i;

	// Counters.
	long bak_segref_count, bak_facref_count, bak_volref_count;

	if(!b->quiet)
	{
		myprintf( "Recovering boundaries...\n" );
	}


	if(b->verbose)
	{
		myprintf( "  Recovering segments.\n" );
	}

	// Segments will be introduced.
	checksubsegflag = 1;

	misseglist = new arraypool( sizeof( face ), 8 );
	bdrysteinerptlist = new arraypool( sizeof( point ), 8 );

	// In random order.
	subsegs->traversalinit( );
	for(i = 0; i < subsegs->items; i++)
	{
		s = randomnation( i + 1 );
		// Move the s-th seg to the i-th.
		subsegstack->newindex( (void **)&paryseg );
		*paryseg = *(face *)fastlookup( subsegstack, s );
		// Put i-th seg to be the s-th.
		searchseg.sh = shellfacetraverse( subsegs );
		paryseg = (face *)fastlookup( subsegstack, s );
		*paryseg = searchseg;
	}

	// The init number of missing segments.
	ms = subsegs->items;
	nit = 0;
	if(b->fliplinklevel < 0)
	{
		autofliplinklevel = 1; // Init value.
	}

	// First, trying to recover segments by only doing flips.
	while(1)
	{
		recoversegments( misseglist, 0, 0 );

		if(misseglist->objects > 0)
		{
			if(b->fliplinklevel >= 0)
			{
				break;
			}
			else
			{
				if(misseglist->objects >= ms)
				{
					nit++;
					if(nit >= 3)
					{
						//break;
						// Do the last round with unbounded flip link level.
						b->fliplinklevel = 100000;
					}
				}
				else
				{
					ms = misseglist->objects;
					if(nit > 0)
					{
						nit--;
					}
				}
				for(i = 0; i < misseglist->objects; i++)
				{
					subsegstack->newindex( (void **)&paryseg );
					*paryseg = *(face *)fastlookup( misseglist, i );
				}
				misseglist->restart( );
				autofliplinklevel += b->fliplinklevelinc;
			}
		}
		else
		{
			// All segments are recovered.
			break;
		}
	} // while (1)

	if(b->verbose)
	{
		myprintf( "  %ld (%ld) segments are recovered (missing).\n",
				subsegs->items - misseglist->objects, misseglist->objects );
	}

	if(misseglist->objects > 0)
	{
		// Second, trying to recover segments by doing more flips (fullsearch).
		while(misseglist->objects > 0)
		{
			ms = misseglist->objects;
			for(i = 0; i < misseglist->objects; i++)
			{
				subsegstack->newindex( (void **)&paryseg );
				*paryseg = *(face *)fastlookup( misseglist, i );
			}
			misseglist->restart( );

			recoversegments( misseglist, 1, 0 );

			if(misseglist->objects < ms)
			{
				// The number of missing segments is reduced.
				continue;
			}
			else
			{
				break;
			}
		}
		if(b->verbose)
		{
			myprintf( "  %ld (%ld) segments are recovered (missing).\n",
					subsegs->items - misseglist->objects, misseglist->objects );
		}
	}

	if(misseglist->objects > 0)
	{
		// Third, trying to recover segments by doing more flips (fullsearch)
		//   and adding Steiner points in the volume.
		while(misseglist->objects > 0)
		{
			ms = misseglist->objects;
			for(i = 0; i < misseglist->objects; i++)
			{
				subsegstack->newindex( (void **)&paryseg );
				*paryseg = *(face *)fastlookup( misseglist, i );
			}
			misseglist->restart( );

			recoversegments( misseglist, 1, 1 );

			if(misseglist->objects < ms)
			{
				// The number of missing segments is reduced.
				continue;
			}
			else
			{
				break;
			}
		}
		if(b->verbose)
		{
			myprintf( "  Added %ld Steiner points in volume.\n", st_volref_count );
		}
	}

	if(misseglist->objects > 0)
	{
		// Last, trying to recover segments by doing more flips (fullsearch),
		//   and adding Steiner points in the volume, and splitting segments.
		long bak_inpoly_count = st_volref_count; //st_inpoly_count;
		for(i = 0; i < misseglist->objects; i++)
		{
			subsegstack->newindex( (void **)&paryseg );
			*paryseg = *(face *)fastlookup( misseglist, i );
		}
		misseglist->restart( );

		recoversegments( misseglist, 1, 2 );

		if(b->verbose)
		{
			myprintf( "  Added %ld Steiner points in segments.\n", st_segref_count );
			if(st_volref_count > bak_inpoly_count)
			{
				myprintf( "  Added another %ld Steiner points in volume.\n",
						st_volref_count - bak_inpoly_count );
			}
		}
	}


	if(st_segref_count > 0)
	{
		// Try to remove the Steiner points added in segments.
		bak_segref_count = st_segref_count;
		bak_volref_count = st_volref_count;
		for(i = 0; i < subvertstack->objects; i++)
		{
			// Get the Steiner point.
			parypt = (point *)fastlookup( subvertstack, i );
			rempt = *parypt;
			if(!removevertexbyflips( rempt ))
			{
				// Save it in list.
				bdrysteinerptlist->newindex( (void **)&parypt );
				*parypt = rempt;
			}
		}
		if(b->verbose)
		{
			if(st_segref_count < bak_segref_count)
			{
				if(bak_volref_count < st_volref_count)
				{
					myprintf( "  Suppressed %ld Steiner points in segments.\n",
							st_volref_count - bak_volref_count );
				}
				if(( st_segref_count + ( st_volref_count - bak_volref_count ) ) <
					bak_segref_count)
				{
					myprintf( "  Removed %ld Steiner points in segments.\n",
							bak_segref_count -
							( st_segref_count + ( st_volref_count - bak_volref_count ) ) );
				}
			}
		}
		subvertstack->restart( );
	}


	tv = clock( );

	if(b->verbose)
	{
		myprintf( "  Recovering facets.\n" );
	}

	// Subfaces will be introduced.
	checksubfaceflag = 1;

	misshlist = new arraypool( sizeof( face ), 8 );

	// Randomly order the subfaces.
	subfaces->traversalinit( );
	for(i = 0; i < subfaces->items; i++)
	{
		s = randomnation( i + 1 );
		// Move the s-th subface to the i-th.
		subfacstack->newindex( (void **)&parysh );
		*parysh = *(face *)fastlookup( subfacstack, s );
		// Put i-th subface to be the s-th.
		searchsh.sh = shellfacetraverse( subfaces );
		parysh = (face *)fastlookup( subfacstack, s );
		*parysh = searchsh;
	}

	ms = subfaces->items;
	nit = 0;
	b->fliplinklevel = -1; // Init.
	if(b->fliplinklevel < 0)
	{
		autofliplinklevel = 1; // Init value.
	}

	while(1)
	{
		recoversubfaces( misshlist, 0 );

		if(misshlist->objects > 0)
		{
			if(b->fliplinklevel >= 0)
			{
				break;
			}
			else
			{
				if(misshlist->objects >= ms)
				{
					nit++;
					if(nit >= 3)
					{
						//break;
						// Do the last round with unbounded flip link level.
						b->fliplinklevel = 100000;
					}
				}
				else
				{
					ms = misshlist->objects;
					if(nit > 0)
					{
						nit--;
					}
				}
				for(i = 0; i < misshlist->objects; i++)
				{
					subfacstack->newindex( (void **)&parysh );
					*parysh = *(face *)fastlookup( misshlist, i );
				}
				misshlist->restart( );
				autofliplinklevel += b->fliplinklevelinc;
			}
		}
		else
		{
			// All subfaces are recovered.
			break;
		}
	} // while (1)

	if(b->verbose)
	{
		myprintf( "  %ld (%ld) subfaces are recovered (missing).\n",
				subfaces->items - misshlist->objects, misshlist->objects );
	}

	if(misshlist->objects > 0)
	{
		// There are missing subfaces. Add Steiner points.
		for(i = 0; i < misshlist->objects; i++)
		{
			subfacstack->newindex( (void **)&parysh );
			*parysh = *(face *)fastlookup( misshlist, i );
		}
		misshlist->restart( );

		recoversubfaces( NULL, 1 );

		if(b->verbose)
		{
			myprintf( "  Added %ld Steiner points in facets.\n", st_facref_count );
		}
	}


	if(st_facref_count > 0)
	{
		// Try to remove the Steiner points added in facets.
		bak_facref_count = st_facref_count;
		for(i = 0; i < subvertstack->objects; i++)
		{
			// Get the Steiner point.
			parypt = (point *)fastlookup( subvertstack, i );
			rempt = *parypt;
			if(!removevertexbyflips( *parypt ))
			{
				// Save it in list.
				bdrysteinerptlist->newindex( (void **)&parypt );
				*parypt = rempt;
			}
		}
		if(b->verbose)
		{
			if(st_facref_count < bak_facref_count)
			{
				myprintf( "  Removed %ld Steiner points in facets.\n",
						bak_facref_count - st_facref_count );
			}
		}
		subvertstack->restart( );
	}


	if(bdrysteinerptlist->objects > 0)
	{
		if(b->verbose)
		{
			myprintf( "  %ld Steiner points remained in boundary.\n",
					bdrysteinerptlist->objects );
		}
	} // if


	// Accumulate the dynamic memory.
	totalworkmemory += ( misseglist->totalmemory + misshlist->totalmemory +
						 bdrysteinerptlist->totalmemory );

	delete bdrysteinerptlist;
	delete misseglist;
	delete misshlist;
}

////                                                                       ////
////                                                                       ////
//// steiner_cxx //////////////////////////////////////////////////////////////


//// reconstruct_cxx //////////////////////////////////////////////////////////
////                                                                       ////
////                                                                       ////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// carveholes()    Remove tetrahedra not in the mesh domain.                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


void tetgenmesh::carveholes( )
{
	arraypool *tetarray, *hullarray;
	triface tetloop, neightet, *parytet, *parytet1;
	triface *regiontets = NULL;
	face checksh, *parysh;
	face checkseg;
	point ptloop, *parypt;
	int t1ver;
	int i, j, k;

	if(!b->quiet)
	{
		if(b->convex)
		{
			myprintf( "Marking exterior tetrahedra ...\n" );
		}
		else
		{
			myprintf( "Removing exterior tetrahedra ...\n" );
		}
	}

	// Initialize the pool of exterior tets.
	tetarray = new arraypool( sizeof( triface ), 10 );
	hullarray = new arraypool( sizeof( triface ), 10 );

	// Collect unprotected tets and hull tets.
	tetrahedrons->traversalinit( );
	tetloop.ver = 11; // The face opposite to dummypoint.
	tetloop.tet = alltetrahedrontraverse( );
	while(tetloop.tet != (tetrahedron *)NULL)
	{
		if(ishulltet( tetloop ))
		{
			// Is this side protected by a subface?
			if(!issubface( tetloop ))
			{
				// Collect an unprotected hull tet and tet.
				infect( tetloop );
				hullarray->newindex( (void **)&parytet );
				*parytet = tetloop;
				// tetloop's face number is 11 & 3 = 3.
				decode( tetloop.tet[ 3 ], neightet );
				if(!infected( neightet ))
				{
					infect( neightet );
					tetarray->newindex( (void **)&parytet );
					*parytet = neightet;
				}
			}
		}
		tetloop.tet = alltetrahedrontraverse( );
	}

	if(in->numberofholes > 0)
	{
		// Mark as infected any tets inside volume holes.
		for(i = 0; i < 3 * in->numberofholes; i += 3)
		{
			// Search a tet containing the i-th hole point.
			neightet.tet = NULL;
			randomsample( &( in->holelist[ i ] ), &neightet );
			if(locate( &( in->holelist[ i ] ), &neightet ) != OUTSIDE)
			{
				// The tet 'neightet' contain this point.
				if(!infected( neightet ))
				{
					infect( neightet );
					tetarray->newindex( (void **)&parytet );
					*parytet = neightet;
					// Add its adjacent tet if it is not protected.
					if(!issubface( neightet ))
					{
						decode( neightet.tet[ neightet.ver & 3 ], tetloop );
						if(!infected( tetloop ))
						{
							infect( tetloop );
							if(ishulltet( tetloop ))
							{
								hullarray->newindex( (void **)&parytet );
							}
							else
							{
								tetarray->newindex( (void **)&parytet );
							}
							*parytet = tetloop;
						}
					}
					else
					{
						// It is protected. Check if its adjacent tet is a hull tet.
						decode( neightet.tet[ neightet.ver & 3 ], tetloop );
						if(ishulltet( tetloop ))
						{
							// It is hull tet, add it into the list. Moreover, the subface
							//   is dead, i.e., both sides are in exterior.
							if(!infected( tetloop ))
							{
								infect( tetloop );
								hullarray->newindex( (void **)&parytet );
								*parytet = tetloop;
							}
						}
						if(infected( tetloop ))
						{
							// Both sides of this subface are in exterior.
							tspivot( neightet, checksh );
							sinfect( checksh ); // Only queue it once.
							subfacstack->newindex( (void **)&parysh );
							*parysh = checksh;
						}
					}
				} // if (!infected(neightet))
			}
			else
			{
				// A hole point locates outside of the convex hull.
				if(!b->quiet)
				{
					myprintf( "Warning:  The %d-th hole point ", i / 3 + 1 );
					myprintf( "lies outside the convex hull.\n" );
				}
			}
		} // i
	} // if (in->numberofholes > 0)

	if(b->regionattrib && ( in->numberofregions > 0 ))
	{ // -A option.
		// Record the tetrahedra that contains the region points for assigning
		//   region attributes after the holes have been carved.
		regiontets = new triface[ in->numberofregions ];
		// Mark as marktested any tetrahedra inside volume regions.
		for(i = 0; i < 5 * in->numberofregions; i += 5)
		{
			// Search a tet containing the i-th region point.
			neightet.tet = NULL;
			randomsample( &( in->regionlist[ i ] ), &neightet );
			if(locate( &( in->regionlist[ i ] ), &neightet ) != OUTSIDE)
			{
				regiontets[ i / 5 ] = neightet;
			}
			else
			{
				if(!b->quiet)
				{
					myprintf( "Warning:  The %d-th region point ", i / 5 + 1 );
					myprintf( "lies outside the convex hull.\n" );
				}
				regiontets[ i / 5 ].tet = NULL;
			}
		}
	}

	// Collect all exterior tets (in concave place and in holes).
	for(i = 0; i < tetarray->objects; i++)
	{
		parytet = (triface *)fastlookup( tetarray, i );
		j = ( parytet->ver & 3 ); // j is the current face number.
		// Check the other three adjacent tets.
		for(k = 1; k < 4; k++)
		{
			decode( parytet->tet[ ( j + k ) % 4 ], neightet );
			// neightet may be a hull tet.
			if(!infected( neightet ))
			{
				// Is neightet protected by a subface.
				if(!issubface( neightet ))
				{
					// Not proected. Collect it. (It must not be a hull tet).
					infect( neightet );
					tetarray->newindex( (void **)&parytet1 );
					*parytet1 = neightet;
				}
				else
				{
					// Protected. Check if it is a hull tet.
					if(ishulltet( neightet ))
					{
						// A hull tet. Collect it.
						infect( neightet );
						hullarray->newindex( (void **)&parytet1 );
						*parytet1 = neightet;
						// Both sides of this subface are exterior.
						tspivot( neightet, checksh );
						// Queue this subface (to be deleted later).
						sinfect( checksh ); // Only queue it once.
						subfacstack->newindex( (void **)&parysh );
						*parysh = checksh;
					}
				}
			}
			else
			{
				// Both sides of this face are in exterior.
				// If there is a subface. It should be collected.
				if(issubface( neightet ))
				{
					tspivot( neightet, checksh );
					if(!sinfected( checksh ))
					{
						sinfect( checksh );
						subfacstack->newindex( (void **)&parysh );
						*parysh = checksh;
					}
				}
			}
		} // j, k
	} // i

	if(b->regionattrib && ( in->numberofregions > 0 ))
	{
		// Re-check saved region tets to see if they lie outside.
		for(i = 0; i < in->numberofregions; i++)
		{
			if(infected( regiontets[ i ] ))
			{
				if(b->verbose)
				{
					myprintf( "Warning:  The %d-th region point ", i + 1 );
					myprintf( "lies in the exterior of the domain.\n" );
				}
				regiontets[ i ].tet = NULL;
			}
		}
	}

	// Collect vertices which point to infected tets. These vertices
	//   may get deleted after the removal of exterior tets.
	//   If -Y1 option is used, collect all Steiner points for removal.
	//   The lists 'cavetetvertlist' and 'subvertstack' are re-used.
	points->traversalinit( );
	ptloop = pointtraverse( );
	while(ptloop != NULL)
	{
		if(( pointtype( ptloop ) != UNUSEDVERTEX ) &&
			( pointtype( ptloop ) != DUPLICATEDVERTEX ))
		{
			decode( point2tet( ptloop ), neightet );
			if(infected( neightet ))
			{
				cavetetvertlist->newindex( (void **)&parypt );
				*parypt = ptloop;
			}
			if(b->nobisect && ( b->supsteiner_level > 0 ))
			{ // -Y/1
				// Queue it if it is a Steiner point.
				if(pointmark( ptloop ) >
					( in->numberofpoints - ( in->firstnumber ? 0 : 1 ) ))
				{
					subvertstack->newindex( (void **)&parypt );
					*parypt = ptloop;
				}
			}
		}
		ptloop = pointtraverse( );
	}

	if(!b->convex && ( tetarray->objects > 0l ))
	{ // No -c option.
		// Remove exterior tets. Hull tets are updated.
		arraypool *newhullfacearray;
		triface hulltet, casface;
		face segloop, *paryseg;
		point pa, pb, pc;
		long delsegcount = 0l;

		// Collect segments which point to infected tets. Some segments
		//   may get deleted after the removal of exterior tets.
		subsegs->traversalinit( );
		segloop.sh = shellfacetraverse( subsegs );
		while(segloop.sh != NULL)
		{
			sstpivot1( segloop, neightet );
			if(infected( neightet ))
			{
				subsegstack->newindex( (void **)&paryseg );
				*paryseg = segloop;
			}
			segloop.sh = shellfacetraverse( subsegs );
		}

		newhullfacearray = new arraypool( sizeof( triface ), 10 );

		// Create and save new hull tets.
		for(i = 0; i < tetarray->objects; i++)
		{
			parytet = (triface *)fastlookup( tetarray, i );
			for(j = 0; j < 4; j++)
			{
				decode( parytet->tet[ j ], tetloop );
				if(!infected( tetloop ))
				{
					// Found a new hull face (must be a subface).
					tspivot( tetloop, checksh );
					maketetrahedron( &hulltet );
					pa = org( tetloop );
					pb = dest( tetloop );
					pc = apex( tetloop );
					setvertices( hulltet, pb, pa, pc, dummypoint );
					bond( tetloop, hulltet );
					// Update the subface-to-tet map.
					sesymself( checksh );
					tsbond( hulltet, checksh );
					// Update the segment-to-tet map.
					for(k = 0; k < 3; k++)
					{
						if(issubseg( tetloop ))
						{
							tsspivot1( tetloop, checkseg );
							tssbond1( hulltet, checkseg );
							sstbond1( checkseg, hulltet );
						}
						enextself( tetloop );
						eprevself( hulltet );
					}
					// Update the point-to-tet map.
					setpoint2tet( pa, (tetrahedron)tetloop.tet );
					setpoint2tet( pb, (tetrahedron)tetloop.tet );
					setpoint2tet( pc, (tetrahedron)tetloop.tet );
					// Save the exterior tet at this hull face. It still holds pointer
					//   to the adjacent interior tet. Use it to connect new hull tets. 
					newhullfacearray->newindex( (void **)&parytet1 );
					parytet1->tet = parytet->tet;
					parytet1->ver = j;
				} // if (!infected(tetloop))
			} // j
		} // i

		// Connect new hull tets.
		for(i = 0; i < newhullfacearray->objects; i++)
		{
			parytet = (triface *)fastlookup( newhullfacearray, i );
			fsym( *parytet, neightet );
			// Get the new hull tet.
			fsym( neightet, hulltet );
			for(j = 0; j < 3; j++)
			{
				esym( hulltet, casface );
				if(casface.tet[ casface.ver & 3 ] == NULL)
				{
					// Since the boundary of the domain may not be a manifold, we
					//   find the adjacent hull face by traversing the tets in the
					//   exterior (which are all infected tets).
					neightet = *parytet;
					while(1)
					{
						fnextself( neightet );
						if(!infected( neightet )) break;
					}
					if(!ishulltet( neightet ))
					{
						// An interior tet. Get the new hull tet.
						fsymself( neightet );
						esymself( neightet );
					}
					// Bond them together.
					bond( casface, neightet );
				}
				enextself( hulltet );
				enextself( *parytet );
			} // j
		} // i

		if(subfacstack->objects > 0l)
		{
			// Remove all subfaces which do not attach to any tetrahedron.
			//   Segments which are not attached to any subfaces and tets
			//   are deleted too.
			face casingout, casingin;

			for(i = 0; i < subfacstack->objects; i++)
			{
				parysh = (face *)fastlookup( subfacstack, i );
				if(i == 0)
				{
					if(b->verbose)
					{
						myprintf( "Warning:  Removed an exterior face (%d, %d, %d) #%d\n",
								pointmark( sorg( *parysh ) ), pointmark( sdest( *parysh ) ),
								pointmark( sapex( *parysh ) ), shellmark( *parysh ) );
					}
				}
				// Dissolve this subface from face links.
				for(j = 0; j < 3; j++)
				{
					spivot( *parysh, casingout );
					sspivot( *parysh, checkseg );
					if(casingout.sh != NULL)
					{
						casingin = casingout;
						while(1)
						{
							spivot( casingin, checksh );
							if(checksh.sh == parysh->sh) break;
							casingin = checksh;
						}
						if(casingin.sh != casingout.sh)
						{
							// Update the link: ... -> casingin -> casingout ->...
							sbond1( casingin, casingout );
						}
						else
						{
							// Only one subface at this edge is left.
							sdissolve( casingout );
						}
						if(checkseg.sh != NULL)
						{
							// Make sure the segment does not connect to a dead one.
							ssbond( casingout, checkseg );
						}
					}
					else
					{
						if(checkseg.sh != NULL)
						{
							//if (checkseg.sh[3] != NULL) {
							if(delsegcount == 0)
							{
								if(b->verbose)
								{
									myprintf( "Warning:  Removed an exterior segment (%d, %d) #%d\n",
											pointmark( sorg( checkseg ) ), pointmark( sdest( checkseg ) ),
											shellmark( checkseg ) );
								}
							}
							shellfacedealloc( subsegs, checkseg.sh );
							delsegcount++;
						}
					}
					senextself( *parysh );
				} // j
				// Delete this subface.
				shellfacedealloc( subfaces, parysh->sh );
			} // i
			if(b->verbose)
			{
				myprintf( "  Deleted %ld subfaces.\n", subfacstack->objects );
			}
			subfacstack->restart( );
		} // if (subfacstack->objects > 0l)

		if(subsegstack->objects > 0l)
		{
			for(i = 0; i < subsegstack->objects; i++)
			{
				paryseg = (face *)fastlookup( subsegstack, i );
				if(paryseg->sh && ( paryseg->sh[ 3 ] != NULL ))
				{
					sstpivot1( *paryseg, neightet );
					if(infected( neightet ))
					{
						if(b->verbose)
						{
							myprintf( "Warning:  Removed an exterior segment (%d, %d) #%d\n",
									pointmark( sorg( *paryseg ) ), pointmark( sdest( *paryseg ) ),
									shellmark( *paryseg ) );
						}
						shellfacedealloc( subsegs, paryseg->sh );
						delsegcount++;
					}
				}
			}
			subsegstack->restart( );
		} // if (subsegstack->objects > 0l)

		if(delsegcount > 0)
		{
			if(b->verbose)
			{
				myprintf( "  Deleted %ld segments.\n", delsegcount );
			}
		}

		if(cavetetvertlist->objects > 0l)
		{
			// Some vertices may lie in exterior. Marke them as UNUSEDVERTEX.
			long delvertcount = unuverts;
			long delsteinercount = 0l;

			for(i = 0; i < cavetetvertlist->objects; i++)
			{
				parypt = (point *)fastlookup( cavetetvertlist, i );
				decode( point2tet( *parypt ), neightet );
				if(infected( neightet ))
				{
					// Found an exterior vertex.
					if(pointmark( *parypt ) >
						( in->numberofpoints - ( in->firstnumber ? 0 : 1 ) ))
					{
						// A Steiner point.
						if(pointtype( *parypt ) == FREESEGVERTEX)
						{
							st_segref_count--;
						}
						else if(pointtype( *parypt ) == FREEFACETVERTEX)
						{
							st_facref_count--;
						}
						else
						{
							st_volref_count--;
						}
						delsteinercount++;
						if(steinerleft > 0) steinerleft++;
					}
					setpointtype( *parypt, UNUSEDVERTEX );
					unuverts++;
				}
			}

			if(b->verbose)
			{
				if(unuverts > delvertcount)
				{
					if(delsteinercount > 0l)
					{
						if(unuverts > ( delvertcount + delsteinercount ))
						{
							myprintf( "  Removed %ld exterior input vertices.\n",
									unuverts - delvertcount - delsteinercount );
						}
						myprintf( "  Removed %ld exterior Steiner vertices.\n",
								delsteinercount );
					}
					else
					{
						myprintf( "  Removed %ld exterior input vertices.\n",
								unuverts - delvertcount );
					}
				}
			}
			cavetetvertlist->restart( );
			// Comment: 'subvertstack' will be cleaned in routine
			//   suppresssteinerpoints().
		} // if (cavetetvertlist->objects > 0l)

		// Update the hull size.
		hullsize += ( newhullfacearray->objects - hullarray->objects );

		// Delete all exterior tets and old hull tets.
		for(i = 0; i < tetarray->objects; i++)
		{
			parytet = (triface *)fastlookup( tetarray, i );
			tetrahedrondealloc( parytet->tet );
		}
		tetarray->restart( );

		for(i = 0; i < hullarray->objects; i++)
		{
			parytet = (triface *)fastlookup( hullarray, i );
			tetrahedrondealloc( parytet->tet );
		}
		hullarray->restart( );

		delete newhullfacearray;
	} // if (!b->convex && (tetarray->objects > 0l))

	if(b->convex && ( tetarray->objects > 0l ))
	{ // With -c option
		// In this case, all exterior tets get a region marker '-1'.
		int attrnum = numelemattrib - 1;

		for(i = 0; i < tetarray->objects; i++)
		{
			parytet = (triface *)fastlookup( tetarray, i );
			setelemattribute( parytet->tet, attrnum, -1 );
		}
		tetarray->restart( );

		for(i = 0; i < hullarray->objects; i++)
		{
			parytet = (triface *)fastlookup( hullarray, i );
			uninfect( *parytet );
		}
		hullarray->restart( );

		if(subfacstack->objects > 0l)
		{
			for(i = 0; i < subfacstack->objects; i++)
			{
				parysh = (face *)fastlookup( subfacstack, i );
				suninfect( *parysh );
			}
			subfacstack->restart( );
		}

		if(cavetetvertlist->objects > 0l)
		{
			cavetetvertlist->restart( );
		}
	} // if (b->convex && (tetarray->objects > 0l))

	if(b->regionattrib)
	{ // With -A option.
		if(!b->quiet)
		{
			myprintf( "Spreading region attributes.\n" );
		}
		REAL volume;
		int attr, maxattr = 0; // Choose a small number here.
		int attrnum = numelemattrib - 1;
		// Comment: The element region marker is at the end of the list of
		//   the element attributes.
		int regioncount = 0;

		// If has user-defined region attributes.
		if(in->numberofregions > 0)
		{
			// Spread region attributes.
			for(i = 0; i < 5 * in->numberofregions; i += 5)
			{
				if(regiontets[ i / 5 ].tet != NULL)
				{
					attr = (int)in->regionlist[ i + 3 ];
					if(attr > maxattr)
					{
						maxattr = attr;
					}
					volume = in->regionlist[ i + 4 ];
					tetarray->restart( ); // Re-use this array.
					infect( regiontets[ i / 5 ] );
					tetarray->newindex( (void **)&parytet );
					*parytet = regiontets[ i / 5 ];
					// Collect and set attrs for all tets of this region.
					for(j = 0; j < tetarray->objects; j++)
					{
						parytet = (triface *)fastlookup( tetarray, j );
						tetloop = *parytet;
						setelemattribute( tetloop.tet, attrnum, attr );
						if(b->varvolume)
						{ // If has -a option.
							setvolumebound( tetloop.tet, volume );
						}
						for(k = 0; k < 4; k++)
						{
							decode( tetloop.tet[ k ], neightet );
							// Is the adjacent already checked?
							if(!infected( neightet ))
							{
								// Is this side protected by a subface?
								if(!issubface( neightet ))
								{
									infect( neightet );
									tetarray->newindex( (void **)&parytet );
									*parytet = neightet;
								}
							}
						} // k
					} // j
					regioncount++;
				} // if (regiontets[i/5].tet != NULL)
			} // i
		}

		// Set attributes for all tetrahedra.
		attr = maxattr + 1;
		tetrahedrons->traversalinit( );
		tetloop.tet = tetrahedrontraverse( );
		while(tetloop.tet != (tetrahedron *)NULL)
		{
			if(!infected( tetloop ))
			{
				// An unmarked region.
				tetarray->restart( ); // Re-use this array.
				infect( tetloop );
				tetarray->newindex( (void **)&parytet );
				*parytet = tetloop;
				// Find and mark all tets.
				for(j = 0; j < tetarray->objects; j++)
				{
					parytet = (triface *)fastlookup( tetarray, j );
					tetloop = *parytet;
					setelemattribute( tetloop.tet, attrnum, attr );
					for(k = 0; k < 4; k++)
					{
						decode( tetloop.tet[ k ], neightet );
						// Is the adjacent tet already checked?
						if(!infected( neightet ))
						{
							// Is this side protected by a subface?
							if(!issubface( neightet ))
							{
								infect( neightet );
								tetarray->newindex( (void **)&parytet );
								*parytet = neightet;
							}
						}
					} // k
				} // j
				attr++; // Increase the attribute.
				regioncount++;
			}
			tetloop.tet = tetrahedrontraverse( );
		}
		// Until here, every tet has a region attribute.

		// Uninfect processed tets.
		tetrahedrons->traversalinit( );
		tetloop.tet = tetrahedrontraverse( );
		while(tetloop.tet != (tetrahedron *)NULL)
		{
			uninfect( tetloop );
			tetloop.tet = tetrahedrontraverse( );
		}

		if(b->verbose)
		{
			//assert(regioncount > 0);
			if(regioncount > 1)
			{
				myprintf( "  Found %d subdomains.\n", regioncount );
			}
			else
			{
				myprintf( "  Found %d domain.\n", regioncount );
			}
		}
	} // if (b->regionattrib)

	if(regiontets != NULL)
	{
		delete[ ] regiontets;
	}
	delete tetarray;
	delete hullarray;

	if(!b->convex)
	{ // No -c option
		// The mesh is non-convex now.
		nonconvex = 1;

		// Push all hull tets into 'flipstack'.
		tetrahedrons->traversalinit( );
		tetloop.ver = 11; // The face opposite to dummypoint.
		tetloop.tet = alltetrahedrontraverse( );
		while(tetloop.tet != (tetrahedron *)NULL)
		{
			if((point)tetloop.tet[ 7 ] == dummypoint)
			{
				fsym( tetloop, neightet );
				flippush( flipstack, &neightet );
			}
			tetloop.tet = alltetrahedrontraverse( );
		}

		flipconstraints fc;
		fc.enqflag = 2;
		long sliver_peel_count = lawsonflip3d( &fc );

		if(sliver_peel_count > 0l)
		{
			if(b->verbose)
			{
				myprintf( "  Removed %ld hull slivers.\n", sliver_peel_count );
			}
		}
		unflipqueue->restart( );
	} // if (!b->convex)
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// reconstructmesh()    Reconstruct a tetrahedral mesh.                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::reconstructmesh( )
{
	tetrahedron *ver2tetarray;
	point *idx2verlist;
	triface tetloop, checktet, prevchktet;
	triface hulltet, face1, face2;
	tetrahedron tptr;
	face subloop, neighsh, nextsh;
	face segloop;
	shellface sptr;
	point p[ 4 ], q[ 3 ];
	REAL ori, attrib, volume;
	REAL cosang_tol, cosang;
	REAL n1[ 3 ], n2[ 3 ];
	int eextras, marker = 0;
	int bondflag;
	int t1ver;
	int idx, i, j, k;

	if(!b->quiet)
	{
		myprintf( "Reconstructing mesh ...\n" );
	}

	if(b->convex)
	{ // -c option.
		// Assume the mesh is convex. Exterior tets have region attribute -1.
		if(!( in->numberoftetrahedronattributes > 0 ))
		{
			terminatetetgen( this, 2 );
		}
	}
	else
	{
		// Assume the mesh is non-convex.
		nonconvex = 1;
	}

	// Create a map from indices to vertices. 
	makeindex2pointmap( idx2verlist );
	// 'idx2verlist' has length 'in->numberofpoints + 1'.
	if(in->firstnumber == 1)
	{
		idx2verlist[ 0 ] = dummypoint; // Let 0th-entry be dummypoint.
	}

	// Allocate an array that maps each vertex to its adjacent tets.
	ver2tetarray = new tetrahedron[ in->numberofpoints + 1 ];
	//for (i = 0; i < in->numberofpoints + 1; i++) {
	for(i = in->firstnumber; i < in->numberofpoints + in->firstnumber; i++)
	{
		setpointtype( idx2verlist[ i ], VOLVERTEX ); // initial type.
		ver2tetarray[ i ] = NULL;
	}

	// Create the tetrahedra and connect those that share a common face.
	for(i = 0; i < in->numberoftetrahedra; i++)
	{
		// Get the four vertices.
		idx = i * in->numberofcorners;
		for(j = 0; j < 4; j++)
		{
			p[ j ] = idx2verlist[ in->tetrahedronlist[ idx++ ] ];
		}
		// Check the orientation.
		ori = orient3d( p[ 0 ], p[ 1 ], p[ 2 ], p[ 3 ] );
		if(ori > 0.0)
		{
			// Swap the first two vertices.
			q[ 0 ] = p[ 0 ]; p[ 0 ] = p[ 1 ]; p[ 1 ] = q[ 0 ];
		}
		else if(ori == 0.0)
		{
			if(!b->quiet)
			{
				myprintf( "Warning:  Tet #%d is degenerate.\n", i + in->firstnumber );
			}
		}
		// Create a new tetrahedron.
		maketetrahedron( &tetloop ); // tetloop.ver = 11.
		setvertices( tetloop, p[ 0 ], p[ 1 ], p[ 2 ], p[ 3 ] );
		// Set element attributes if they exist.
		for(j = 0; j < in->numberoftetrahedronattributes; j++)
		{
			idx = i * in->numberoftetrahedronattributes;
			attrib = in->tetrahedronattributelist[ idx + j ];
			setelemattribute( tetloop.tet, j, attrib );
		}
		// If -a switch is used (with no number follows) Set a volume
		//   constraint if it exists.
		if(b->varvolume)
		{
			if(in->tetrahedronvolumelist != (REAL *)NULL)
			{
				volume = in->tetrahedronvolumelist[ i ];
			}
			else
			{
				volume = -1.0;
			}
			setvolumebound( tetloop.tet, volume );
		}
		// Try connecting this tet to others that share the common faces.
		for(tetloop.ver = 0; tetloop.ver < 4; tetloop.ver++)
		{
			p[ 3 ] = oppo( tetloop );
			// Look for other tets having this vertex.
			idx = pointmark( p[ 3 ] );
			tptr = ver2tetarray[ idx ];
			// Link the current tet to the next one in the stack.
			tetloop.tet[ 8 + tetloop.ver ] = tptr;
			// Push the current tet onto the stack.
			ver2tetarray[ idx ] = encode( tetloop );
			decode( tptr, checktet );
			if(checktet.tet != NULL)
			{
				p[ 0 ] = org( tetloop ); // a
				p[ 1 ] = dest( tetloop ); // b
				p[ 2 ] = apex( tetloop ); // c
				prevchktet = tetloop;
				do
				{
					q[ 0 ] = org( checktet ); // a'
					q[ 1 ] = dest( checktet ); // b'
					q[ 2 ] = apex( checktet ); // c'
					// Check the three faces at 'd' in 'checktet'.
					bondflag = 0;
					for(j = 0; j < 3; j++)
					{
						// Go to the face [b',a',d], or [c',b',d], or [a',c',d].
						esym( checktet, face2 );
						if(face2.tet[ face2.ver & 3 ] == NULL)
						{
							k = ( ( j + 1 ) % 3 );
							if(q[ k ] == p[ 0 ])
							{   // b', c', a' = a
								if(q[ j ] == p[ 1 ])
								{ // a', b', c' = b
									// [#,#,d] is matched to [b,a,d].
									esym( tetloop, face1 );
									bond( face1, face2 );
									bondflag++;
								}
							}
							if(q[ k ] == p[ 1 ])
							{   // b',c',a' = b
								if(q[ j ] == p[ 2 ])
								{ // a',b',c' = c
									// [#,#,d] is matched to [c,b,d].
									enext( tetloop, face1 );
									esymself( face1 );
									bond( face1, face2 );
									bondflag++;
								}
							}
							if(q[ k ] == p[ 2 ])
							{   // b',c',a' = c
								if(q[ j ] == p[ 0 ])
								{ // a',b',c' = a
									// [#,#,d] is matched to [a,c,d].
									eprev( tetloop, face1 );
									esymself( face1 );
									bond( face1, face2 );
									bondflag++;
								}
							}
						}
						else
						{
							bondflag++;
						}
						enextself( checktet );
					} // j
					// Go to the next tet in the link.
					tptr = checktet.tet[ 8 + checktet.ver ];
					if(bondflag == 3)
					{
						// All three faces at d in 'checktet' have been connected.
						// It can be removed from the link.            
						prevchktet.tet[ 8 + prevchktet.ver ] = tptr;
					}
					else
					{
						// Bakup the previous tet in the link.
						prevchktet = checktet;
					}
					decode( tptr, checktet );
				}
				while(checktet.tet != NULL);
			} // if (checktet.tet != NULL)
		} // for (tetloop.ver = 0; ...
	} // i

	// Remember a tet of the mesh.
	recenttet = tetloop;

	// Create hull tets, create the point-to-tet map, and clean up the
	//   temporary spaces used in each tet. 
	hullsize = tetrahedrons->items;

	tetrahedrons->traversalinit( );
	tetloop.tet = tetrahedrontraverse( );
	while(tetloop.tet != (tetrahedron *)NULL)
	{
		tptr = encode( tetloop );
		for(tetloop.ver = 0; tetloop.ver < 4; tetloop.ver++)
		{
			if(tetloop.tet[ tetloop.ver ] == NULL)
			{
				// Create a hull tet.
				maketetrahedron( &hulltet );
				p[ 0 ] = org( tetloop );
				p[ 1 ] = dest( tetloop );
				p[ 2 ] = apex( tetloop );
				setvertices( hulltet, p[ 1 ], p[ 0 ], p[ 2 ], dummypoint );
				bond( tetloop, hulltet );
				// Try connecting this to others that share common hull edges.
				for(j = 0; j < 3; j++)
				{
					fsym( hulltet, face2 );
					while(1)
					{
						if(face2.tet == NULL) break;
						esymself( face2 );
						if(apex( face2 ) == dummypoint) break;
						fsymself( face2 );
					}
					if(face2.tet != NULL)
					{
						// Found an adjacent hull tet.
						esym( hulltet, face1 );
						bond( face1, face2 );
					}
					enextself( hulltet );
				}
			}
			// Create the point-to-tet map.
			setpoint2tet( (point)( tetloop.tet[ 4 + tetloop.ver ] ), tptr );
			// Clean the temporary used space.
			tetloop.tet[ 8 + tetloop.ver ] = NULL;
		}
		tetloop.tet = tetrahedrontraverse( );
	}

	hullsize = tetrahedrons->items - hullsize;

	// Subfaces will be inserted into the mesh. 
	if(in->trifacelist != NULL)
	{
		// A .face file is given. It may contain boundary faces. Insert them.
		for(i = 0; i < in->numberoftrifaces; i++)
		{
			// Is it a subface?
			if(in->trifacemarkerlist != NULL)
			{
				marker = in->trifacemarkerlist[ i ];
			}
			else
			{
				// Face markers are not available. Assume all of them are subfaces.
				marker = -1; // The default marker.
			}
			if(marker != 0)
			{
				idx = i * 3;
				for(j = 0; j < 3; j++)
				{
					p[ j ] = idx2verlist[ in->trifacelist[ idx++ ] ];
				}
				// Search the subface.
				bondflag = 0;
				neighsh.sh = NULL;
				// Make sure all vertices are in the mesh. Avoid crash.
				for(j = 0; j < 3; j++)
				{
					decode( point2tet( p[ j ] ), checktet );
					if(checktet.tet == NULL) break;
				}
				if(( j == 3 ) && getedge( p[ 0 ], p[ 1 ], &checktet ))
				{
					tetloop = checktet;
					q[ 2 ] = apex( checktet );
					while(1)
					{
						if(apex( tetloop ) == p[ 2 ])
						{
							// Found the face.
							// Check if there exist a subface already?
							tspivot( tetloop, neighsh );
							if(neighsh.sh != NULL)
							{
								// Found a duplicated subface. 
								// This happens when the mesh was generated by other mesher.
								bondflag = 0;
							}
							else
							{
								bondflag = 1;
							}
							break;
						}
						fnextself( tetloop );
						if(apex( tetloop ) == q[ 2 ]) break;
					}
				}
				if(bondflag)
				{
					// Create a new subface.
					makeshellface( subfaces, &subloop );
					setshvertices( subloop, p[ 0 ], p[ 1 ], p[ 2 ] );
					// Create the point-to-subface map.
					sptr = sencode( subloop );
					for(j = 0; j < 3; j++)
					{
						setpointtype( p[ j ], FACETVERTEX ); // initial type.
						setpoint2sh( p[ j ], sptr );
					}
					setshellmark( subloop, marker );
					// Insert the subface into the mesh.
					tsbond( tetloop, subloop );
					fsymself( tetloop );
					sesymself( subloop );
					tsbond( tetloop, subloop );
				}
				else
				{
					if(neighsh.sh != NULL)
					{
						// The subface already exists. Only set its mark.
						setshellmark( neighsh, marker );
					}
					else
					{
						if(!b->quiet)
						{
							if(neighsh.sh == NULL)
							{
								myprintf( "Warning:  Subface #%d [%d,%d,%d] is missing.\n",
										i + in->firstnumber, pointmark( p[ 0 ] ), pointmark( p[ 1 ] ),
										pointmark( p[ 2 ] ) );
							}
						}
					}
				} // if (bondflag)
			} // if (marker != 0)
		} // i
	} // if (in->trifacelist)

	// Indentify subfaces from the mesh.
	// Create subfaces for hull faces (if they're not subface yet) and
	//   interior faces which separate two different materials.
	eextras = in->numberoftetrahedronattributes;
	tetrahedrons->traversalinit( );
	tetloop.tet = tetrahedrontraverse( );
	while(tetloop.tet != (tetrahedron *)NULL)
	{
		for(tetloop.ver = 0; tetloop.ver < 4; tetloop.ver++)
		{
			tspivot( tetloop, neighsh );
			if(neighsh.sh == NULL)
			{
				bondflag = 0;
				fsym( tetloop, checktet );
				if(ishulltet( checktet ))
				{
					// A hull face.
					if(!b->convex)
					{
						bondflag = 1;  // Insert a hull subface.
					}
				}
				else
				{
					if(eextras > 0)
					{
						if(elemattribute( tetloop.tet, eextras - 1 ) !=
							elemattribute( checktet.tet, eextras - 1 ))
						{
							bondflag = 1; // Insert an interior interface.
						}
					}
				}
				if(bondflag)
				{
					// Create a new subface.
					makeshellface( subfaces, &subloop );
					p[ 0 ] = org( tetloop );
					p[ 1 ] = dest( tetloop );
					p[ 2 ] = apex( tetloop );
					setshvertices( subloop, p[ 0 ], p[ 1 ], p[ 2 ] );
					// Create the point-to-subface map.
					sptr = sencode( subloop );
					for(j = 0; j < 3; j++)
					{
						setpointtype( p[ j ], FACETVERTEX ); // initial type.
						setpoint2sh( p[ j ], sptr );
					}
					setshellmark( subloop, -1 ); // Default marker.
					// Insert the subface into the mesh.
					tsbond( tetloop, subloop );
					sesymself( subloop );
					tsbond( checktet, subloop );
				} // if (bondflag)
			} // if (neighsh.sh == NULL)
		}
		tetloop.tet = tetrahedrontraverse( );
	}

	// Connect subfaces together. 
	subfaces->traversalinit( );
	subloop.shver = 0;
	subloop.sh = shellfacetraverse( subfaces );
	while(subloop.sh != (shellface *)NULL)
	{
		for(i = 0; i < 3; i++)
		{
			spivot( subloop, neighsh );
			if(neighsh.sh == NULL)
			{
				// Form a subface ring by linking all subfaces at this edge.
				// Traversing all faces of the tets at this edge.
				stpivot( subloop, tetloop );
				q[ 2 ] = apex( tetloop );
				neighsh = subloop;
				while(1)
				{
					fnextself( tetloop );
					tspivot( tetloop, nextsh );
					if(nextsh.sh != NULL)
					{
						// Do not connect itself.
						if(nextsh.sh != neighsh.sh)
						{
							// Link neighsh <= nextsh.
							sbond1( neighsh, nextsh );
							neighsh = nextsh;
						}
					}
					if(apex( tetloop ) == q[ 2 ])
					{
						break;
					}
				} // while (1)
			} // if (neighsh.sh == NULL)
			senextself( subloop );
		}
		subloop.sh = shellfacetraverse( subfaces );
	}


	// Segments will be introduced. 
	if(in->edgelist != NULL)
	{
		// A .edge file is given. It may contain boundary edges. Insert them.
		for(i = 0; i < in->numberofedges; i++)
		{
			// Is it a segment?
			if(in->edgemarkerlist != NULL)
			{
				marker = in->edgemarkerlist[ i ];
			}
			else
			{
				// Edge markers are not available. Assume all of them are segments.
				marker = -1; // Default marker.
			}
			if(marker != 0)
			{
				// Insert a segment.
				idx = i * 2;
				for(j = 0; j < 2; j++)
				{
					p[ j ] = idx2verlist[ in->edgelist[ idx++ ] ];
				}
				// Make sure all vertices are in the mesh. Avoid crash.
				for(j = 0; j < 2; j++)
				{
					decode( point2tet( p[ j ] ), checktet );
					if(checktet.tet == NULL) break;
				}
				// Search the segment.
				if(( j == 2 ) && getedge( p[ 0 ], p[ 1 ], &checktet ))
				{
					// Create a new segment.
					makeshellface( subsegs, &segloop );
					setshvertices( segloop, p[ 0 ], p[ 1 ], NULL );
					// Create the point-to-segment map.
					sptr = sencode( segloop );
					for(j = 0; j < 2; j++)
					{
						setpointtype( p[ j ], RIDGEVERTEX ); // initial type.
						setpoint2sh( p[ j ], sptr );
					}
					setshellmark( segloop, marker );
					// Insert the segment into the mesh.
					tetloop = checktet;
					q[ 2 ] = apex( checktet );
					subloop.sh = NULL;
					while(1)
					{
						tssbond1( tetloop, segloop );
						tspivot( tetloop, subloop );
						if(subloop.sh != NULL)
						{
							ssbond1( subloop, segloop );
							sbond1( segloop, subloop );
						}
						fnextself( tetloop );
						if(apex( tetloop ) == q[ 2 ]) break;
					} // while (1)
					// Remember an adjacent tet for this segment.
					sstbond1( segloop, tetloop );
				}
				else
				{
					if(!b->quiet)
					{
						myprintf( "Warning:  Segment #%d [%d,%d] is missing.\n",
								i + in->firstnumber, pointmark( p[ 0 ] ), pointmark( p[ 1 ] ) );
					}
				}
			} // if (marker != 0)
		} // i
	} // if (in->edgelist)

	// Identify segments from the mesh. 
	// Create segments for non-manifold edges (which are shared by more 
	//   than two subfaces), and for non-coplanar edges, i.e., two subfaces
	//   form an dihedral angle > 'b->facet_separate_ang_tol' (degree).
	cosang_tol = cos( b->facet_separate_ang_tol / 180.0 * PIconstant );
	subfaces->traversalinit( );
	subloop.shver = 0;
	subloop.sh = shellfacetraverse( subfaces );
	while(subloop.sh != (shellface *)NULL)
	{
		for(i = 0; i < 3; i++)
		{
			sspivot( subloop, segloop );
			if(segloop.sh == NULL)
			{
				// Check if this edge is a segment.
				bondflag = 0;
				// Counter the number of subfaces at this edge.
				idx = 0;
				nextsh = subloop;
				while(1)
				{
					idx++;
					spivotself( nextsh );
					if(nextsh.sh == subloop.sh) break;
				}
				if(idx != 2)
				{
					// It's a non-manifold edge. Insert a segment.
					p[ 0 ] = sorg( subloop );
					p[ 1 ] = sdest( subloop );
					bondflag = 1;
				}
				else
				{
					spivot( subloop, neighsh );
					if(shellmark( subloop ) != shellmark( neighsh ))
					{
						// It's an interior interface. Insert a segment.
						p[ 0 ] = sorg( subloop );
						p[ 1 ] = sdest( subloop );
						bondflag = 1;
					}
					else
					{
						if(!b->convex)
						{
							// Check the dihedral angle formed by the two subfaces.
							p[ 0 ] = sorg( subloop );
							p[ 1 ] = sdest( subloop );
							p[ 2 ] = sapex( subloop );
							p[ 3 ] = sapex( neighsh );
							facenormal( p[ 0 ], p[ 1 ], p[ 2 ], n1, 1, NULL );
							facenormal( p[ 0 ], p[ 1 ], p[ 3 ], n2, 1, NULL );
							cosang = dot( n1, n2 ) / ( sqrt( dot( n1, n1 ) ) * sqrt( dot( n2, n2 ) ) );
							// Rounding.
							if(cosang > 1.0) cosang = 1.0;
							else if(cosang < -1.0) cosang = -1.0;
							if(cosang > cosang_tol)
							{
								bondflag = 1;
							}
						}
					}
				}
				if(bondflag)
				{
					// Create a new segment.
					makeshellface( subsegs, &segloop );
					setshvertices( segloop, p[ 0 ], p[ 1 ], NULL );
					// Create the point-to-segment map.
					sptr = sencode( segloop );
					for(j = 0; j < 2; j++)
					{
						setpointtype( p[ j ], RIDGEVERTEX ); // initial type.
						setpoint2sh( p[ j ], sptr );
					}
					setshellmark( segloop, -1 ); // Default marker.
					// Insert the subface into the mesh.
					stpivot( subloop, tetloop );
					q[ 2 ] = apex( tetloop );
					while(1)
					{
						tssbond1( tetloop, segloop );
						tspivot( tetloop, neighsh );
						if(neighsh.sh != NULL)
						{
							ssbond1( neighsh, segloop );
						}
						fnextself( tetloop );
						if(apex( tetloop ) == q[ 2 ]) break;
					} // while (1)
					// Remember an adjacent tet for this segment.
					sstbond1( segloop, tetloop );
					sbond1( segloop, subloop );
				} // if (bondflag)
			} // if (neighsh.sh == NULL)
			senextself( subloop );
		} // i
		subloop.sh = shellfacetraverse( subfaces );
	}

	// Remember the number of input segments.
	insegments = subsegs->items;

	if(!b->nobisect || checkconstraints)
	{
		// Mark Steiner points on segments and facets.
		//   - all vertices which remaining type FEACTVERTEX become
		//     Steiner points in facets (= FREEFACERVERTEX).
		//   - vertices on segment need to be checked.
		face* segperverlist;
		int* idx2seglist;
		face parentseg, nextseg;
		verttype vt;
		REAL area, len, l1, l2;
		int fmarker;

		makepoint2submap( subsegs, idx2seglist, segperverlist );

		points->traversalinit( );
		point ptloop = pointtraverse( );
		while(ptloop != NULL)
		{
			vt = pointtype( ptloop );
			if(vt == VOLVERTEX)
			{
				setpointtype( ptloop, FREEVOLVERTEX );
				st_volref_count++;
			}
			else if(vt == FACETVERTEX)
			{
				setpointtype( ptloop, FREEFACETVERTEX );
				st_facref_count++;
			}
			else if(vt == RIDGEVERTEX)
			{
				idx = pointmark( ptloop ) - in->firstnumber;
				if(( idx2seglist[ idx + 1 ] - idx2seglist[ idx ] ) == 2)
				{
					i = idx2seglist[ idx ];
					parentseg = segperverlist[ i ];
					nextseg = segperverlist[ i + 1 ];
					sesymself( nextseg );
					p[ 0 ] = sorg( nextseg );
					p[ 1 ] = sdest( parentseg );
					// Check if three points p[0], ptloop, p[2] are (nearly) collinear.
					len = distance( p[ 0 ], p[ 1 ] );
					l1 = distance( p[ 0 ], ptloop );
					l2 = distance( ptloop, p[ 1 ] );
					if(( ( l1 + l2 - len ) / len ) < b->epsilon)
					{
						// They are (nearly) collinear.
						setpointtype( ptloop, FREESEGVERTEX );
						// Connect nextseg and parentseg together at ptloop.
						senextself( nextseg );
						senext2self( parentseg );
						sbond( nextseg, parentseg );
						st_segref_count++;
					}
				}
			}
			ptloop = pointtraverse( );
		}

		// Are there area constraints?
		if(b->quality && ( in->facetconstraintlist != (REAL *)NULL ))
		{
			// Set maximum area constraints on facets.
			for(i = 0; i < in->numberoffacetconstraints; i++)
			{
				fmarker = (int)in->facetconstraintlist[ i * 2 ];
				area = in->facetconstraintlist[ i * 2 + 1 ];
				subfaces->traversalinit( );
				subloop.sh = shellfacetraverse( subfaces );
				while(subloop.sh != NULL)
				{
					if(shellmark( subloop ) == fmarker)
					{
						setareabound( subloop, area );
					}
					subloop.sh = shellfacetraverse( subfaces );
				}
			}
		}

		// Are there length constraints?
		if(b->quality && ( in->segmentconstraintlist != (REAL *)NULL ))
		{
			// Set maximum length constraints on segments.
			int e1, e2;
			for(i = 0; i < in->numberofsegmentconstraints; i++)
			{
				e1 = (int)in->segmentconstraintlist[ i * 3 ];
				e2 = (int)in->segmentconstraintlist[ i * 3 + 1 ];
				len = in->segmentconstraintlist[ i * 3 + 2 ];
				// Search for edge [e1, e2].
				idx = e1 - in->firstnumber;
				for(j = idx2seglist[ idx ]; j < idx2seglist[ idx + 1 ]; j++)
				{
					parentseg = segperverlist[ j ];
					if(pointmark( sdest( parentseg ) ) == e2)
					{
						setareabound( parentseg, len );
						break;
					}
				}
			}
		}

		delete[ ] idx2seglist;
		delete[ ] segperverlist;
	}


	// Set global flags.
	checksubsegflag = 1;
	checksubfaceflag = 1;

	delete[ ] idx2verlist;
	delete[ ] ver2tetarray;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// scoutpoint()    Search a point in mesh.                                   //
//                                                                           //
// This function searches the point in a mesh whose domain may be not convex.//
// In case of a convex domain, the locate() function is sufficient.          //
//                                                                           //
// If 'randflag' is used, randomly select a start searching tet.  Otherwise, //
// start searching directly from 'searchtet'.                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::scoutpoint( point searchpt, triface *searchtet, int randflag )
{
	point pa, pb, pc, pd;
	enum locateresult loc = OUTSIDE;
	REAL vol, ori1, ori2 = 0, ori3 = 0, ori4 = 0;
	int t1ver;


	// Randomly select a good starting tet.
	if(randflag)
	{
		randomsample( searchpt, searchtet );
	}
	else
	{
		if(searchtet->tet == NULL)
		{
			*searchtet = recenttet;
		}
	}
	loc = locate( searchpt, searchtet );

	if(loc == OUTSIDE)
	{
		if(b->convex)
		{ // -c option
			// The point lies outside of the convex hull.
			return (int)loc;
		}
		// Test if it lies nearly on the hull face.
		// Reuse vol, ori1.
		pa = org( *searchtet );
		pb = dest( *searchtet );
		pc = apex( *searchtet );
		vol = triarea( pa, pb, pc );
		ori1 = orient3dfast( pa, pb, pc, searchpt );
		if(fabs( ori1 / vol ) < b->epsilon)
		{
			loc = ONFACE; // On face (or on edge, or on vertex).
			fsymself( *searchtet );
		}
	}

	if(loc != OUTSIDE)
	{
		// Round the result of location.
		pa = org( *searchtet );
		pb = dest( *searchtet );
		pc = apex( *searchtet );
		pd = oppo( *searchtet );
		vol = orient3dfast( pa, pb, pc, pd );
		ori1 = orient3dfast( pa, pb, pc, searchpt );
		ori2 = orient3dfast( pb, pa, pd, searchpt );
		ori3 = orient3dfast( pc, pb, pd, searchpt );
		ori4 = orient3dfast( pa, pc, pd, searchpt );
		if(fabs( ori1 / vol ) < b->epsilon) ori1 = 0;
		if(fabs( ori2 / vol ) < b->epsilon) ori2 = 0;
		if(fabs( ori3 / vol ) < b->epsilon) ori3 = 0;
		if(fabs( ori4 / vol ) < b->epsilon) ori4 = 0;
	}
	else
	{ // if (loc == OUTSIDE) {
		// Do a brute force search for the point (with rounding).
		tetrahedrons->traversalinit( );
		searchtet->tet = tetrahedrontraverse( );
		while(searchtet->tet != NULL)
		{
			pa = org( *searchtet );
			pb = dest( *searchtet );
			pc = apex( *searchtet );
			pd = oppo( *searchtet );

			vol = orient3dfast( pa, pb, pc, pd );
			if(vol < 0)
			{
				ori1 = orient3dfast( pa, pb, pc, searchpt );
				if(fabs( ori1 / vol ) < b->epsilon) ori1 = 0; // Rounding.
				if(ori1 <= 0)
				{
					ori2 = orient3dfast( pb, pa, pd, searchpt );
					if(fabs( ori2 / vol ) < b->epsilon) ori2 = 0;
					if(ori2 <= 0)
					{
						ori3 = orient3dfast( pc, pb, pd, searchpt );
						if(fabs( ori3 / vol ) < b->epsilon) ori3 = 0;
						if(ori3 <= 0)
						{
							ori4 = orient3dfast( pa, pc, pd, searchpt );
							if(fabs( ori4 / vol ) < b->epsilon) ori4 = 0;
							if(ori4 <= 0)
							{
								// Found the tet. Return its location. 
								break;
							} // ori4
						} // ori3
					} // ori2
				} // ori1
			}

			searchtet->tet = tetrahedrontraverse( );
		} // while (searchtet->tet != NULL)
		nonregularcount++;  // Re-use this counter.
	}

	if(searchtet->tet != NULL)
	{
		// Return the point location.
		if(ori1 == 0)
		{ // on face [a,b,c]
			if(ori2 == 0)
			{ // on edge [a,b].
				if(ori3 == 0)
				{ // on vertex [b].
					enextself( *searchtet ); // [b,c,a,d]
					loc = ONVERTEX;
				}
				else
				{
					if(ori4 == 0)
					{ // on vertex [a]
						loc = ONVERTEX; // [a,b,c,d]
					}
					else
					{
						loc = ONEDGE; // [a,b,c,d]
					}
				}
			}
			else
			{ // ori2 != 0
				if(ori3 == 0)
				{ // on edge [b,c]
					if(ori4 == 0)
					{ // on vertex [c]
						eprevself( *searchtet ); // [c,a,b,d]
						loc = ONVERTEX;
					}
					else
					{
						enextself( *searchtet ); // [b,c,a,d]
						loc = ONEDGE;
					}
				}
				else
				{ // ori3 != 0
					if(ori4 == 0)
					{ // on edge [c,a]
						eprevself( *searchtet ); // [c,a,b,d]
						loc = ONEDGE;
					}
					else
					{
						loc = ONFACE;
					}
				}
			}
		}
		else
		{ // ori1 != 0
			if(ori2 == 0)
			{ // on face [b,a,d]
				esymself( *searchtet ); // [b,a,d,c]
				if(ori3 == 0)
				{ // on edge [b,d]
					eprevself( *searchtet ); // [d,b,a,c]
					if(ori4 == 0)
					{ // on vertex [d]                      
						loc = ONVERTEX;
					}
					else
					{
						loc = ONEDGE;
					}
				}
				else
				{ // ori3 != 0
					if(ori4 == 0)
					{ // on edge [a,d]
						enextself( *searchtet ); // [a,d,b,c]
						loc = ONEDGE;
					}
					else
					{
						loc = ONFACE;
					}
				}
			}
			else
			{ // ori2 != 0
				if(ori3 == 0)
				{ // on face [c,b,d]
					enextself( *searchtet );
					esymself( *searchtet );
					if(ori4 == 0)
					{ // on edge [c,d]
						eprevself( *searchtet );
						loc = ONEDGE;
					}
					else
					{
						loc = ONFACE;
					}
				}
				else
				{
					if(ori4 == 0)
					{ // on face [a,c,d]
						eprevself( *searchtet );
						esymself( *searchtet );
						loc = ONFACE;
					}
					else
					{ // inside tet [a,b,c,d]
						loc = INTETRAHEDRON;
					} // ori4
				} // ori3
			} // ori2
		} // ori1
	}
	else
	{
		loc = OUTSIDE;
	}

	return (int)loc;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// getpointmeshsize()    Interpolate the mesh size at given point.           //
//                                                                           //
// 'iloc' indicates the location of the point w.r.t. 'searchtet'.  The size  //
// is obtained by linear interpolation on the vertices of the tet.           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

REAL tetgenmesh::getpointmeshsize( point searchpt, triface *searchtet, int iloc )
{
	point *pts, pa, pb, pc;
	REAL volume, vol[ 4 ], wei[ 4 ];
	REAL size;
	int i;

	size = 0;

	if(iloc == (int)INTETRAHEDRON)
	{
		pts = (point *)&( searchtet->tet[ 4 ] );
		// Only do interpolation if all vertices have non-zero sizes.
		if(( pts[ 0 ][ pointmtrindex ] > 0 ) && ( pts[ 1 ][ pointmtrindex ] > 0 ) &&
			( pts[ 2 ][ pointmtrindex ] > 0 ) && ( pts[ 3 ][ pointmtrindex ] > 0 ))
		{
			// P1 interpolation.
			volume = orient3dfast( pts[ 0 ], pts[ 1 ], pts[ 2 ], pts[ 3 ] );
			vol[ 0 ] = orient3dfast( searchpt, pts[ 1 ], pts[ 2 ], pts[ 3 ] );
			vol[ 1 ] = orient3dfast( pts[ 0 ], searchpt, pts[ 2 ], pts[ 3 ] );
			vol[ 2 ] = orient3dfast( pts[ 0 ], pts[ 1 ], searchpt, pts[ 3 ] );
			vol[ 3 ] = orient3dfast( pts[ 0 ], pts[ 1 ], pts[ 2 ], searchpt );
			for(i = 0; i < 4; i++)
			{
				wei[ i ] = fabs( vol[ i ] / volume );
				size += ( wei[ i ] * pts[ i ][ pointmtrindex ] );
			}
		}
	}
	else if(iloc == (int)ONFACE)
	{
		pa = org( *searchtet );
		pb = dest( *searchtet );
		pc = apex( *searchtet );
		if(( pa[ pointmtrindex ] > 0 ) && ( pb[ pointmtrindex ] > 0 ) &&
			( pc[ pointmtrindex ] > 0 ))
		{
			volume = triarea( pa, pb, pc );
			vol[ 0 ] = triarea( searchpt, pb, pc );
			vol[ 1 ] = triarea( pa, searchpt, pc );
			vol[ 2 ] = triarea( pa, pb, searchpt );
			size = ( vol[ 0 ] / volume ) * pa[ pointmtrindex ]
				+ ( vol[ 1 ] / volume ) * pb[ pointmtrindex ]
				+ ( vol[ 2 ] / volume ) * pc[ pointmtrindex ];
		}
	}
	else if(iloc == (int)ONEDGE)
	{
		pa = org( *searchtet );
		pb = dest( *searchtet );
		if(( pa[ pointmtrindex ] > 0 ) && ( pb[ pointmtrindex ] > 0 ))
		{
			volume = distance( pa, pb );
			vol[ 0 ] = distance( searchpt, pb );
			vol[ 1 ] = distance( pa, searchpt );
			size = ( vol[ 0 ] / volume ) * pa[ pointmtrindex ]
				+ ( vol[ 1 ] / volume ) * pb[ pointmtrindex ];
		}
	}
	else if(iloc == (int)ONVERTEX)
	{
		pa = org( *searchtet );
		if(pa[ pointmtrindex ] > 0)
		{
			size = pa[ pointmtrindex ];
		}
	}

	return size;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// interpolatemeshsize()    Interpolate the mesh size from a background mesh //
//                          (source) to the current mesh (destination).      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::interpolatemeshsize( )
{
	triface searchtet;
	point ploop;
	REAL minval = 0.0, maxval = 0.0;
	int iloc;
	int count;

	if(!b->quiet)
	{
		myprintf( "Interpolating mesh size ...\n" );
	}

	long bak_nonregularcount = nonregularcount;
	nonregularcount = 0l; // Count the number of (slow) global searches.
	long baksmaples = bgm->samples;
	bgm->samples = 3l;
	count = 0; // Count the number of interpolated points.

	// Interpolate sizes for all points in the current mesh.
	points->traversalinit( );
	ploop = pointtraverse( );
	while(ploop != NULL)
	{
		// Search a tet in bgm which containing this point.
		searchtet.tet = NULL;
		iloc = bgm->scoutpoint( ploop, &searchtet, 1 ); // randflag = 1
		if(iloc != (int)OUTSIDE)
		{
			// Interpolate the mesh size.
			ploop[ pointmtrindex ] = bgm->getpointmeshsize( ploop, &searchtet, iloc );
			setpoint2bgmtet( ploop, bgm->encode( searchtet ) );
			if(count == 0)
			{
				// This is the first interpolated point.
				minval = maxval = ploop[ pointmtrindex ];
			}
			else
			{
				if(ploop[ pointmtrindex ] < minval)
				{
					minval = ploop[ pointmtrindex ];
				}
				if(ploop[ pointmtrindex ] > maxval)
				{
					maxval = ploop[ pointmtrindex ];
				}
			}
			count++;
		}
		else
		{
			if(!b->quiet)
			{
				myprintf( "Warnning:  Failed to locate point %d in source mesh.\n",
						pointmark( ploop ) );
			}
		}
		ploop = pointtraverse( );
	}

	if(b->verbose)
	{
		myprintf( "  Interoplated %d points.\n", count );
		if(nonregularcount > 0l)
		{
			myprintf( "  Performed %ld brute-force searches.\n", nonregularcount );
		}
		myprintf( "  Size rangle [%.17g, %.17g].\n", minval, maxval );
	}

	bgm->samples = baksmaples;
	nonregularcount = bak_nonregularcount;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// insertconstrainedpoints()    Insert a list of points into the mesh.       //
//                                                                           //
// Assumption:  The bounding box of the insert point set should be no larger //
// than the bounding box of the mesh.  (Required by point sorting).          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::insertconstrainedpoints( point *insertarray, int arylen,
										  int rejflag )
{
	triface searchtet, spintet;
	face splitsh;
	face splitseg;
	insertvertexflags ivf;
	flipconstraints fc;
	int randflag = 0;
	int t1ver;
	int i;

	if(b->verbose)
	{
		myprintf( "  Inserting %d constrained points\n", arylen );
	}

	if(b->no_sort)
	{ // -b/1 option.
		if(b->verbose)
		{
			myprintf( "  Using the input order.\n" );
		}
	}
	else
	{
		if(b->verbose)
		{
			myprintf( "  Permuting vertices.\n" );
		}
		point swappoint;
		int randindex;
		srand( arylen );
		for(i = 0; i < arylen; i++)
		{
			randindex = rand( ) % ( i + 1 );
			swappoint = insertarray[ i ];
			insertarray[ i ] = insertarray[ randindex ];
			insertarray[ randindex ] = swappoint;
		}
		if(b->brio_hilbert)
		{ // -b1 option
			if(b->verbose)
			{
				myprintf( "  Sorting vertices.\n" );
			}
			hilbert_init( in->mesh_dim );
			int ngroup = 0;
			brio_multiscale_sort( insertarray, arylen, b->brio_threshold,
								  b->brio_ratio, &ngroup );
		}
		else
		{ // -b0 option.
			randflag = 1;
		} // if (!b->brio_hilbert)
	} // if (!b->no_sort)

	long bak_nonregularcount = nonregularcount;
	nonregularcount = 0l;
	long baksmaples = samples;
	samples = 3l; // Use at least 3 samples. Updated in randomsample().

	long bak_seg_count = st_segref_count;
	long bak_fac_count = st_facref_count;
	long bak_vol_count = st_volref_count;

	// Initialize the insertion parameters. 
	if(b->incrflip)
	{ // -l option
		// Use incremental flip algorithm.
		ivf.bowywat = 0;
		ivf.lawson = 1;
		ivf.validflag = 0; // No need to validate the cavity.
		fc.enqflag = 2;
	}
	else
	{
		// Use Bowyer-Watson algorithm.
		ivf.bowywat = 1;
		ivf.lawson = 0;
		ivf.validflag = 1; // Validate the B-W cavity.
	}
	ivf.rejflag = rejflag;
	ivf.chkencflag = 0;
	ivf.sloc = (int)INSTAR;
	ivf.sbowywat = 3;
	ivf.splitbdflag = 1;
	ivf.respectbdflag = 1;
	ivf.assignmeshsize = b->metric;

	encseglist = new arraypool( sizeof( face ), 8 );
	encshlist = new arraypool( sizeof( badface ), 8 );

	// Insert the points.
	for(i = 0; i < arylen; i++)
	{
		// Find the location of the inserted point.
		// Do not use 'recenttet', since the mesh may be non-convex.
		searchtet.tet = NULL;
		ivf.iloc = scoutpoint( insertarray[ i ], &searchtet, randflag );

		// Decide the right type for this point.
		setpointtype( insertarray[ i ], FREEVOLVERTEX ); // Default.
		splitsh.sh = NULL;
		splitseg.sh = NULL;
		if(ivf.iloc == (int)ONEDGE)
		{
			if(issubseg( searchtet ))
			{
				tsspivot1( searchtet, splitseg );
				setpointtype( insertarray[ i ], FREESEGVERTEX );
				//ivf.rejflag = 0;
			}
			else
			{
				// Check if it is a subface edge.
				spintet = searchtet;
				while(1)
				{
					if(issubface( spintet ))
					{
						tspivot( spintet, splitsh );
						setpointtype( insertarray[ i ], FREEFACETVERTEX );
						//ivf.rejflag |= 1;
						break;
					}
					fnextself( spintet );
					if(spintet.tet == searchtet.tet) break;
				}
			}
		}
		else if(ivf.iloc == (int)ONFACE)
		{
			if(issubface( searchtet ))
			{
				tspivot( searchtet, splitsh );
				setpointtype( insertarray[ i ], FREEFACETVERTEX );
				//ivf.rejflag |= 1;
			}
		}

		// Now insert the point.
		if(insertpoint( insertarray[ i ], &searchtet, &splitsh, &splitseg, &ivf ))
		{
			if(flipstack != NULL)
			{
				// There are queued faces. Use flips to recover Delaunayness.
				lawsonflip3d( &fc );
				// There may be unflippable edges. Ignore them.
				unflipqueue->restart( );
			}
			// Update the Steiner counters.
			if(pointtype( insertarray[ i ] ) == FREESEGVERTEX)
			{
				st_segref_count++;
			}
			else if(pointtype( insertarray[ i ] ) == FREEFACETVERTEX)
			{
				st_facref_count++;
			}
			else
			{
				st_volref_count++;
			}
		}
		else
		{
			// Point is not inserted.
			//pointdealloc(insertarray[i]);
			setpointtype( insertarray[ i ], UNUSEDVERTEX );
			unuverts++;
			encseglist->restart( );
			encshlist->restart( );
		}
	} // i

	delete encseglist;
	delete encshlist;

	if(b->verbose)
	{
		myprintf( "  Inserted %ld (%ld, %ld, %ld) vertices.\n",
				st_segref_count + st_facref_count + st_volref_count -
				( bak_seg_count + bak_fac_count + bak_vol_count ),
				st_segref_count - bak_seg_count, st_facref_count - bak_fac_count,
				st_volref_count - bak_vol_count );
		if(nonregularcount > 0l)
		{
			myprintf( "  Performed %ld brute-force searches.\n", nonregularcount );
		}
	}

	nonregularcount = bak_nonregularcount;
	samples = baksmaples;
}

void tetgenmesh::insertconstrainedpoints( tetgenio *addio )
{
	point *insertarray, newpt;
	REAL x, y, z, w;
	int index, attribindex, mtrindex;
	int arylen, i, j;

	if(!b->quiet)
	{
		myprintf( "Inserting constrained points ...\n" );
	}

	insertarray = new point[ addio->numberofpoints ];
	arylen = 0;
	index = 0;
	attribindex = 0;
	mtrindex = 0;

	for(i = 0; i < addio->numberofpoints; i++)
	{
		x = addio->pointlist[ index++ ];
		y = addio->pointlist[ index++ ];
		z = addio->pointlist[ index++ ];
		// Test if this point lies inside the bounding box.
		if(( x < xmin ) || ( x > xmax ) || ( y < ymin ) || ( y > ymax ) ||
			( z < zmin ) || ( z > zmax ))
		{
			if(b->verbose)
			{
				myprintf( "Warning:  Point #%d lies outside the bounding box. Ignored\n",
						i + in->firstnumber );
			}
			continue;
		}
		makepoint( &newpt, UNUSEDVERTEX );
		setpointgeomuv( newpt, 0, 19.0 );
		setpointgeomuv( newpt, 1, 19.0 );
		// UV: TODO
		newpt[ 0 ] = x;
		newpt[ 1 ] = y;
		newpt[ 2 ] = z;
		// Read the point attributes. (Including point weights.)
		for(j = 0; j < addio->numberofpointattributes; j++)
		{
			newpt[ 3 + j ] = addio->pointattributelist[ attribindex++ ];
		}
		// Read the point metric tensor.
		for(j = 0; j < addio->numberofpointmtrs; j++)
		{
			newpt[ pointmtrindex + j ] = addio->pointmtrlist[ mtrindex++ ];
		}
		if(b->weighted)
		{ // -w option
			if(addio->numberofpointattributes > 0)
			{
				// The first point attribute is its weight.
				w = newpt[ 3 ];
			}
			else
			{
				// No given weight available. Default choose the maximum
				//   absolute value among its coordinates.        
				w = fabs( x );
				if(w < fabs( y )) w = fabs( y );
				if(w < fabs( z )) w = fabs( z );
			}
			if(b->weighted_param == 0)
			{
				newpt[ 3 ] = x * x + y * y + z * z - w; // Weighted DT.
			}
			else
			{ // -w1 option
				newpt[ 3 ] = w;  // Regular tetrahedralization.
			}
		}
		insertarray[ arylen ] = newpt;
		arylen++;
	} // i

	// Insert the points.
	int rejflag = 0;  // Do not check encroachment.
	if(b->metric)
	{ // -m option.
		rejflag |= 4; // Reject it if it lies in some protecting balls.
	}

	insertconstrainedpoints( insertarray, arylen, rejflag );

	delete[ ] insertarray;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// meshcoarsening()    Deleting (selected) vertices.                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::collectremovepoints( arraypool *remptlist )
{
	point ptloop, *parypt;
	verttype vt;

	// If a mesh sizing function is given. Collect vertices whose mesh size
	//   is greater than its smallest edge length.
	if(b->metric)
	{ // -m option
		REAL len, smlen;
		int i;
		points->traversalinit( );
		ptloop = pointtraverse( );
		while(ptloop != NULL)
		{
			if(ptloop[ pointmtrindex ] > 0)
			{
				// Get the smallest edge length at this vertex.
				getvertexstar( 1, ptloop, cavetetlist, cavetetvertlist, NULL );
				parypt = (point *)fastlookup( cavetetvertlist, 0 );
				smlen = distance( ptloop, *parypt );
				for(i = 1; i < cavetetvertlist->objects; i++)
				{
					parypt = (point *)fastlookup( cavetetvertlist, i );
					len = distance( ptloop, *parypt );
					if(len < smlen)
					{
						smlen = len;
					}
				}
				cavetetvertlist->restart( );
				cavetetlist->restart( );
				if(smlen < ptloop[ pointmtrindex ])
				{
					pinfect( ptloop );
					remptlist->newindex( (void **)&parypt );
					*parypt = ptloop;
				}
			}
			ptloop = pointtraverse( );
		}
		if(b->verbose > 1)
		{
			myprintf( "    Coarsen %ld oversized points.\n", remptlist->objects );
		}
	}

	// If 'in->pointmarkerlist' exists, Collect vertices with markers '-1'.
	if(in->pointmarkerlist != NULL)
	{
		long bak_count = remptlist->objects;
		points->traversalinit( );
		ptloop = pointtraverse( );
		int index = 0;
		while(ptloop != NULL)
		{
			if(index < in->numberofpoints)
			{
				if(in->pointmarkerlist[ index ] == -1)
				{
					pinfect( ptloop );
					remptlist->newindex( (void **)&parypt );
					*parypt = ptloop;
				}
			}
			else
			{
				// Remaining are not input points. Stop here.
				break;
			}
			index++;
			ptloop = pointtraverse( );
		}
		if(b->verbose > 1)
		{
			myprintf( "    Coarsen %ld marked points.\n", remptlist->objects - bak_count );
		}
	} // if (in->pointmarkerlist != NULL)

	if(b->coarsen_param > 0)
	{ // -R1/#
		// Remove a coarsen_percent number of interior points.
		if(b->verbose > 1)
		{
			myprintf( "    Coarsen %g percent of interior points.\n",
					b->coarsen_percent * 100.0 );
		}
		arraypool *intptlist = new arraypool( sizeof( point * ), 10 );
		// Count the total number of interior points.
		points->traversalinit( );
		ptloop = pointtraverse( );
		while(ptloop != NULL)
		{
			vt = pointtype( ptloop );
			if(( vt == VOLVERTEX ) || ( vt == FREEVOLVERTEX ) ||
				( vt == FREEFACETVERTEX ) || ( vt == FREESEGVERTEX ))
			{
				intptlist->newindex( (void **)&parypt );
				*parypt = ptloop;
			}
			ptloop = pointtraverse( );
		}
		if(intptlist->objects > 0l)
		{
			// Sort the list of points randomly.
			point *parypt_i, swappt;
			int randindex, i;
			srand( intptlist->objects );
			for(i = 0; i < intptlist->objects; i++)
			{
				randindex = rand( ) % ( i + 1 ); // randomnation(i + 1);
				parypt_i = (point *)fastlookup( intptlist, i );
				parypt = (point *)fastlookup( intptlist, randindex );
				// Swap this two points.
				swappt = *parypt_i;
				*parypt_i = *parypt;
				*parypt = swappt;
			}
			int remcount = (int)( (REAL)intptlist->objects * b->coarsen_percent );
			// Return the first remcount points.
			for(i = 0; i < remcount; i++)
			{
				parypt_i = (point *)fastlookup( intptlist, i );
				if(!pinfected( *parypt_i ))
				{
					pinfected( *parypt_i );
					remptlist->newindex( (void **)&parypt );
					*parypt = *parypt_i;
				}
			}
		}
		delete intptlist;
	}

	// Unmark all collected vertices.
	for(int i = 0; i < remptlist->objects; i++)
	{
		parypt = (point *)fastlookup( remptlist, i );
		puninfect( *parypt );
	}
}

void tetgenmesh::meshcoarsening( )
{
	arraypool *remptlist;

	if(!b->quiet)
	{
		myprintf( "Mesh coarsening ...\n" );
	}

	// Collect the set of points to be removed
	remptlist = new arraypool( sizeof( point * ), 10 );
	collectremovepoints( remptlist );

	if(remptlist->objects == 0l)
	{
		delete remptlist;
		return;
	}

	if(b->verbose)
	{
		if(remptlist->objects > 0l)
		{
			myprintf( "  Removing %ld points...\n", remptlist->objects );
		}
	}

	point *parypt, *plastpt;
	long ms = remptlist->objects;
	int nit = 0;
	int bak_fliplinklevel = b->fliplinklevel;
	b->fliplinklevel = -1;
	autofliplinklevel = 1; // Init value.
	int i;

	while(1)
	{

		if(b->verbose > 1)
		{
			myprintf( "    Removing points [%s level = %2d] #:  %ld.\n",
					( b->fliplinklevel > 0 ) ? "fixed" : "auto",
					( b->fliplinklevel > 0 ) ? b->fliplinklevel : autofliplinklevel,
					remptlist->objects );
		}

		// Remove the list of points.
		for(i = 0; i < remptlist->objects; i++)
		{
			parypt = (point *)fastlookup( remptlist, i );
			if(removevertexbyflips( *parypt ))
			{
				// Move the last entry to the current place.
				plastpt = (point *)fastlookup( remptlist, remptlist->objects - 1 );
				*parypt = *plastpt;
				remptlist->objects--;
				i--;
			}
		}

		if(remptlist->objects > 0l)
		{
			if(b->fliplinklevel >= 0)
			{
				break; // We have tried all levels.
			}
			if(remptlist->objects == ms)
			{
				nit++;
				if(nit >= 3)
				{
					// Do the last round with unbounded flip link level.
					b->fliplinklevel = 100000;
				}
			}
			else
			{
				ms = remptlist->objects;
				if(nit > 0)
				{
					nit--;
				}
			}
			autofliplinklevel += b->fliplinklevelinc;
		}
		else
		{
			// All points are removed.
			break;
		}
	} // while (1)

	if(remptlist->objects > 0l)
	{
		if(b->verbose)
		{
			myprintf( "  %ld points are not removed !\n", remptlist->objects );
		}
	}

	b->fliplinklevel = bak_fliplinklevel;
	delete remptlist;
}

////                                                                       ////
////                                                                       ////
//// reconstruct_cxx //////////////////////////////////////////////////////////

//// refine_cxx ///////////////////////////////////////////////////////////////
////                                                                       ////
////                                                                       ////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// makefacetverticesmap()    Create a map from facet to its vertices.        //
//                                                                           //
// All facets will be indexed (starting from 0).  The map is saved in two    //
// global arrays: 'idx2facetlist' and 'facetverticeslist'.                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::makefacetverticesmap( )
{
	arraypool *facetvertexlist, *vertlist, **paryvertlist;
	face subloop, neighsh, *parysh, *parysh1;
	point pa, *ppt, *parypt;
	verttype vt;
	int facetindex, totalvertices;
	int i, j, k;

	if(b->verbose)
	{
		myprintf( "  Creating the facet vertices map.\n" );
	}

	facetvertexlist = new arraypool( sizeof( arraypool * ), 10 );
	facetindex = totalvertices = 0;

	subfaces->traversalinit( );
	subloop.sh = shellfacetraverse( subfaces );
	while(subloop.sh != NULL)
	{
		if(!sinfected( subloop ))
		{
			// A new facet. Create its vertices list.
			vertlist = new arraypool( sizeof( point * ), 8 );
			ppt = (point *)&( subloop.sh[ 3 ] );
			for(k = 0; k < 3; k++)
			{
				vt = pointtype( ppt[ k ] );
				if(( vt != FREESEGVERTEX ) && ( vt != FREEFACETVERTEX ))
				{
					pinfect( ppt[ k ] );
					vertlist->newindex( (void **)&parypt );
					*parypt = ppt[ k ];
				}
			}
			sinfect( subloop );
			caveshlist->newindex( (void **)&parysh );
			*parysh = subloop;
			for(i = 0; i < caveshlist->objects; i++)
			{
				parysh = (face *)fastlookup( caveshlist, i );
				setfacetindex( *parysh, facetindex );
				for(j = 0; j < 3; j++)
				{
					if(!isshsubseg( *parysh ))
					{
						spivot( *parysh, neighsh );
						if(!sinfected( neighsh ))
						{
							pa = sapex( neighsh );
							if(!pinfected( pa ))
							{
								vt = pointtype( pa );
								if(( vt != FREESEGVERTEX ) && ( vt != FREEFACETVERTEX ))
								{
									pinfect( pa );
									vertlist->newindex( (void **)&parypt );
									*parypt = pa;
								}
							}
							sinfect( neighsh );
							caveshlist->newindex( (void **)&parysh1 );
							*parysh1 = neighsh;
						}
					}
					senextself( *parysh );
				}
			} // i
			totalvertices += (int)vertlist->objects;
			// Uninfect facet vertices.
			for(k = 0; k < vertlist->objects; k++)
			{
				parypt = (point *)fastlookup( vertlist, k );
				puninfect( *parypt );
			}
			caveshlist->restart( );
			// Save this vertex list.
			facetvertexlist->newindex( (void **)&paryvertlist );
			*paryvertlist = vertlist;
			facetindex++;
		}
		subloop.sh = shellfacetraverse( subfaces );
	}

	// All subfaces are infected. Uninfect them.
	subfaces->traversalinit( );
	subloop.sh = shellfacetraverse( subfaces );
	while(subloop.sh != NULL)
	{
		suninfect( subloop );
		subloop.sh = shellfacetraverse( subfaces );
	}

	if(b->verbose)
	{
		myprintf( "  Found %ld facets.\n", facetvertexlist->objects );
	}

	idx2facetlist = new int[ facetindex + 1 ];
	facetverticeslist = new point[ totalvertices ];

	totalworkmemory += ( ( facetindex + 1 ) * sizeof(int)+
						 totalvertices * sizeof( point * ) );

	idx2facetlist[ 0 ] = 0;
	for(i = 0, k = 0; i < facetindex; i++)
	{
		paryvertlist = (arraypool **)fastlookup( facetvertexlist, i );
		vertlist = *paryvertlist;
		idx2facetlist[ i + 1 ] = ( idx2facetlist[ i ] + (int)vertlist->objects );
		for(j = 0; j < vertlist->objects; j++)
		{
			parypt = (point *)fastlookup( vertlist, j );
			facetverticeslist[ k ] = *parypt;
			k++;
		}
	}

	// Free the lists.
	for(i = 0; i < facetvertexlist->objects; i++)
	{
		paryvertlist = (arraypool **)fastlookup( facetvertexlist, i );
		vertlist = *paryvertlist;
		delete vertlist;
	}
	delete facetvertexlist;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Check whether two segments, or a segment and a facet, or two facets are   //
// adjacent to each other.                                                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::segsegadjacent( face *seg1, face *seg2 )
{
	int segidx1 = getfacetindex( *seg1 );
	int segidx2 = getfacetindex( *seg2 );

	if(segidx1 == segidx2) return 0;

	point pa1 = segmentendpointslist[ segidx1 * 2 ];
	point pb1 = segmentendpointslist[ segidx1 * 2 + 1 ];
	point pa2 = segmentendpointslist[ segidx2 * 2 ];
	point pb2 = segmentendpointslist[ segidx2 * 2 + 1 ];

	if(( pa1 == pa2 ) || ( pa1 == pb2 ) || ( pb1 == pa2 ) || ( pb1 == pb2 ))
	{
		return 1;
	}
	return 0;
}

int tetgenmesh::segfacetadjacent( face *subseg, face *subsh )
{
	int segidx = getfacetindex( *subseg );
	point pa = segmentendpointslist[ segidx * 2 ];
	point pb = segmentendpointslist[ segidx * 2 + 1 ];

	pinfect( pa );
	pinfect( pb );

	int fidx = getfacetindex( *subsh );
	int count = 0, i;

	for(i = idx2facetlist[ fidx ]; i < idx2facetlist[ fidx + 1 ]; i++)
	{
		if(pinfected( facetverticeslist[ i ] )) count++;
	}

	puninfect( pa );
	puninfect( pb );

	return count == 1;
}

int tetgenmesh::facetfacetadjacent( face *subsh1, face *subsh2 )
{
	int count = 0, i;

	int fidx1 = getfacetindex( *subsh1 );
	int fidx2 = getfacetindex( *subsh2 );

	if(fidx1 == fidx2) return 0;

	for(i = idx2facetlist[ fidx1 ]; i < idx2facetlist[ fidx1 + 1 ]; i++)
	{
		pinfect( facetverticeslist[ i ] );
	}

	for(i = idx2facetlist[ fidx2 ]; i < idx2facetlist[ fidx2 + 1 ]; i++)
	{
		if(pinfected( facetverticeslist[ i ] )) count++;
	}

	// Uninfect the vertices.
	for(i = idx2facetlist[ fidx1 ]; i < idx2facetlist[ fidx1 + 1 ]; i++)
	{
		puninfect( facetverticeslist[ i ] );
	}

	return count > 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// save_segmentpoint_insradius(), save_facetpoint_insradius()                //
//                                                                           //
// Determine and save the relaxed insertion radius of a Steiner point on a   //
// segment or a facet.  By default, it is the closet distance to the parent  //
// point of this Steiner point. But may be larger than it.                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::save_segmentpoint_insradius( point segpt, point parentpt, REAL r )
{
	REAL rv = r, rp;
	if(pointtype( parentpt ) == FREESEGVERTEX)
	{
		face parentseg1, parentseg2;
		sdecode( point2sh( segpt ), parentseg1 );
		sdecode( point2sh( parentpt ), parentseg2 );
		if(segsegadjacent( &parentseg1, &parentseg2 ))
		{
			rp = getpointinsradius( parentpt );
			if(rv < rp)
			{
				// The relaxed insertion radius of 'newpt'.
				rv = rp;
			}
		}
	}
	else if(pointtype( parentpt ) == FREEFACETVERTEX)
	{
		face parentseg, parentsh;
		sdecode( point2sh( segpt ), parentseg );
		sdecode( point2sh( parentpt ), parentsh );
		if(segfacetadjacent( &parentseg, &parentsh ))
		{
			rp = getpointinsradius( parentpt );
			if(( sqrt( 2.0 ) * rv ) < rp)
			{ // if (rv < rp) {
				// The relaxed insertion radius of 'newpt'.
				rv = rp / sqrt( 2.0 ); // rv = rp; 
			}
		}
	}
	setpointinsradius( segpt, rv );
}

void tetgenmesh::save_facetpoint_insradius( point facpt, point parentpt, REAL r )
{
	REAL rv = r, rp;
	if(pointtype( parentpt ) == FREESEGVERTEX)
	{
		face parentseg, parentsh;
		sdecode( point2sh( parentpt ), parentseg );
		sdecode( point2sh( facpt ), parentsh );
		if(segfacetadjacent( &parentseg, &parentsh ))
		{
			rp = getpointinsradius( parentpt );
			if(rv < ( sqrt( 2.0 ) * rp ))
			{
				rv = sqrt( 2.0 ) * rp; // The relaxed insertion radius of 'newpt'.
			}
		}
	}
	else if(pointtype( parentpt ) == FREEFACETVERTEX)
	{
		face parentsh1, parentsh2;
		sdecode( point2sh( parentpt ), parentsh1 );
		sdecode( point2sh( facpt ), parentsh2 );
		if(facetfacetadjacent( &parentsh1, &parentsh2 ))
		{
			rp = getpointinsradius( parentpt );
			if(rv < rp)
			{
				rv = rp; // The relaxed insertion radius of 'newpt'.
			}
		}
	}
	setpointinsradius( facpt, rv );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// enqueuesubface()    Queue a subface or a subsegment for encroachment chk. //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::enqueuesubface( memorypool *pool, face *chkface )
{
	if(!smarktest2ed( *chkface ))
	{
		smarktest2( *chkface ); // Only queue it once.
		face *queface = (face *)pool->alloc( );
		*queface = *chkface;
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// enqueuetetrahedron()    Queue a tetrahedron for quality check.            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::enqueuetetrahedron( triface *chktet )
{
	if(!marktest2ed( *chktet ))
	{
		marktest2( *chktet ); // Only queue it once.
		triface *quetet = (triface *)badtetrahedrons->alloc( );
		*quetet = *chktet;
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// checkseg4encroach()    Check if an edge is encroached upon by a point.    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::checkseg4encroach( point pa, point pb, point checkpt )
{
	// Check if the point lies inside the diametrical sphere of this seg. 
	REAL v1[ 3 ], v2[ 3 ];

	v1[ 0 ] = pa[ 0 ] - checkpt[ 0 ];
	v1[ 1 ] = pa[ 1 ] - checkpt[ 1 ];
	v1[ 2 ] = pa[ 2 ] - checkpt[ 2 ];
	v2[ 0 ] = pb[ 0 ] - checkpt[ 0 ];
	v2[ 1 ] = pb[ 1 ] - checkpt[ 1 ];
	v2[ 2 ] = pb[ 2 ] - checkpt[ 2 ];

	if(dot( v1, v2 ) < 0)
	{
		// Inside.
		if(b->metric)
		{ // -m option.
			if(( pa[ pointmtrindex ] > 0 ) && ( pb[ pointmtrindex ] > 0 ))
			{
				// The projection of 'checkpt' lies inside the segment [a,b].
				REAL prjpt[ 3 ], u, v, t;
				projpt2edge( checkpt, pa, pb, prjpt );
				// Interoplate the mesh size at the location 'prjpt'.
				u = distance( pa, pb );
				v = distance( pa, prjpt );
				t = v / u;
				// 'u' is the mesh size at 'prjpt'
				u = pa[ pointmtrindex ] + t * ( pb[ pointmtrindex ] - pa[ pointmtrindex ] );
				v = distance( checkpt, prjpt );
				if(v < u)
				{
					return 1; // Encroached prot-ball!
				}
			}
			else
			{
				return 1; // NO protecting ball. Encroached.
			}
		}
		else
		{
			return 1; // Inside! Encroached.
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// checkseg4split()    Check if we need to split a segment.                  //
//                                                                           //
// A segment needs to be split if it is in the following case:               //
//  (1) It is encroached by an existing vertex.                              //
//  (2) It has bad quality (too long).                                       //
//  (3) Its length is larger than the mesh sizes at its endpoints.           //
//                                                                           //
// Return 1 if it needs to be split, otherwise, return 0.  'pencpt' returns  //
// an encroaching point if there exists. 'qflag' returns '1' if the segment  //
// has a length larger than the desired edge length.                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::checkseg4split( face *chkseg, point& encpt, int& qflag )
{
	REAL ccent[ 3 ], len, r;
	int i;

	point forg = sorg( *chkseg );
	point fdest = sdest( *chkseg );

	// Initialize the return values.
	encpt = NULL;
	qflag = 0;

	len = distance( forg, fdest );
	r = 0.5 * len;
	for(i = 0; i < 3; i++)
	{
		ccent[ i ] = 0.5 * ( forg[ i ] + fdest[ i ] );
	}

	// First check its quality.
	if(checkconstraints && ( areabound( *chkseg ) > 0.0 ))
	{
		if(len > areabound( *chkseg ))
		{
			qflag = 1;
			return 1;
		}
	}

	if(b->fixedvolume)
	{
		if(( len * len * len ) > b->maxvolume)
		{
			qflag = 1;
			return 1;
		}
	}

	if(b->metric)
	{ // -m option. Check mesh size. 
		// Check if the ccent lies outside one of the prot.balls at vertices.
		if(( ( forg[ pointmtrindex ] > 0 ) && ( r > forg[ pointmtrindex ] ) ) ||
			( ( fdest[ pointmtrindex ] ) > 0 && ( r > fdest[ pointmtrindex ] ) ))
		{
			qflag = 1; // Enforce mesh size.
			return 1;
		}
	}


	// Second check if it is encroached.
	// Comment: There may exist more than one encroaching points of this segment. 
	//   The 'encpt' returns the one which is closet to it.
	triface searchtet, spintet;
	point eapex;
	REAL d, diff, smdist = 0;
	int t1ver;

	sstpivot1( *chkseg, searchtet );
	spintet = searchtet;
	while(1)
	{
		eapex = apex( spintet );
		if(eapex != dummypoint)
		{
			d = distance( ccent, eapex );
			diff = d - r;
			if(fabs( diff ) / r < b->epsilon) diff = 0.0; // Rounding.
			if(diff < 0)
			{
				// This segment is encroached by eapex.
				if(useinsertradius)
				{
					if(encpt == NULL)
					{
						encpt = eapex;
						smdist = d;
					}
					else
					{
						// Choose the closet encroaching point.
						if(d < smdist)
						{
							encpt = eapex;
							smdist = d;
						}
					}
				}
				else
				{
					encpt = eapex;
					break;
				}
			}
		}
		fnextself( spintet );
		if(spintet.tet == searchtet.tet) break;
	} // while (1)

	if(encpt != NULL)
	{
		return 1;
	}

	return 0; // No need to split it.
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// splitsegment()    Split a segment.                                        //
//                                                                           //
// The segment 'splitseg' is intended to be split. It will be split if it    //
// is in one of the following cases:                                         //
//   (1) It is encroached by an existing vertex 'encpt != NULL'; or          //
//   (2) It is in bad quality 'qflag == 1'; or                               //
//   (3) Its length is larger than the mesh sizes at its endpoints.          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::splitsegment( face *splitseg, point encpt, REAL rrp,
							  point encpt1, point encpt2, int qflag,
							  int chkencflag )
{

	if(!qflag && smarktest3ed( *splitseg ))
	{
		// Do not try to re-split a marked segment.
		return 0;
	}

	if(b->nobisect)
	{ // With -Y option.
		// Only split this segment if it is allowed to be split.
		if(checkconstraints)
		{
			// Check if it has a non-zero length bound. 
			if(areabound( *splitseg ) == 0)
			{
				// It is not allowed.  However, if all of facets containing this seg
				//   is allowed to be split, we still split it.
				face parentsh, spinsh;
				//splitseg.shver = 0;
				spivot( *splitseg, parentsh );
				if(parentsh.sh == NULL)
				{
					return 0; // A dangling segment. Do not split it.
				}
				spinsh = parentsh;
				while(1)
				{
					if(areabound( spinsh ) == 0) break;
					spivotself( spinsh );
					if(spinsh.sh == parentsh.sh) break;
					if(spinsh.sh == NULL) break;  // It belongs to only one facet.
				}
				if(( !spinsh.sh ) || ( areabound( spinsh ) == 0 ))
				{
					// All facets at this seg are not allowed to be split.
					return 0;  // Do not split it.
				}
			}
		}
		else
		{
			return 0; // Do not split this segment.
		}
	} // if (b->nobisect)

	triface searchtet;
	face searchsh;
	point newpt;
	insertvertexflags ivf;

	REAL uv[ tetgenio::pointparam::maxuvs ];
	makepoint( &newpt, FREESEGVERTEX );
	getsteinerptonsegment( splitseg, encpt, newpt, uv );
	if(b->psc)
	{
		// UV
		for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
		{
			setpointgeomuv( newpt, tt, uv[ tt ] );
		}
	}

	if(!qflag && !b->cdtrefine)
	{
		// Do not insert the point if it encroaches upon an adjacent segment.
		face parentsh;
		spivot( *splitseg, parentsh );
		if(parentsh.sh != NULL)
		{
			face spinsh, neighsh;
			face neighseg;
			spinsh = parentsh;
			while(1)
			{
				for(int i = 0; i < 2; i++)
				{
					if(i == 0)
					{
						senext( spinsh, neighsh );
					}
					else
					{
						senext2( spinsh, neighsh );
					}
					if(isshsubseg( neighsh ))
					{
						sspivot( neighsh, neighseg );
						if(checkseg4encroach( sorg( neighseg ), sdest( neighseg ), newpt ))
						{
							pointdealloc( newpt );
							return 0; // Do not split this segment.
						}
					}
				} // i
				spivotself( spinsh );
				if(spinsh.sh == NULL) break;
				if(spinsh.sh == parentsh.sh) break;
			} // while (1)
		}
	}

	// Split the segment by the Bowyer-Watson algorithm.
	sstpivot1( *splitseg, searchtet );
	ivf.iloc = (int)ONEDGE;
	ivf.bowywat = 3; // Use Bowyer-Watson, preserve subsegments and subfaces;
	ivf.validflag = 1; // Validate the B-W cavity.
	ivf.lawson = 2; // Do flips to recover Delaunayness.
	ivf.rejflag = 0;     // Do not check encroachment of new segments/facets.
	if(b->metric)
	{
		ivf.rejflag |= 4;  // Do check encroachment of protecting balls.
	}
	ivf.chkencflag = chkencflag;
	ivf.sloc = (int)INSTAR; // ivf.iloc;
	ivf.sbowywat = 3; // ivf.bowywat;  // Surface mesh options.
	ivf.splitbdflag = 1;
	ivf.respectbdflag = 1;
	ivf.assignmeshsize = b->metric;
	ivf.smlenflag = useinsertradius; // Return the closet mesh vertex.


	if(insertpoint( newpt, &searchtet, &searchsh, splitseg, &ivf ))
	{
		st_segref_count++;
		if(steinerleft > 0) steinerleft--;
		if(useinsertradius)
		{
			save_segmentpoint_insradius( newpt, ivf.parentpt, ivf.smlen );
		}
		if(flipstack != NULL)
		{
			flipconstraints fc;
			fc.chkencflag = chkencflag;
			fc.enqflag = 2;
			lawsonflip3d( &fc );
			unflipqueue->restart( );
		}
		return 1;
	}
	else
	{
		// Point is not inserted.
		if(ivf.iloc == (int)NEARVERTEX)
		{
			terminatetetgen( this, 2 );
		}
		pointdealloc( newpt );
		// Mark this segment to avoid splitting in the future.
		smarktest3( *splitseg );
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// repairencsegs()    Repair encroached (sub) segments.                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::repairencsegs( int chkencflag )
{
	face *bface;
	point encpt = NULL;
	int qflag = 0;

	// Loop until the pool 'badsubsegs' is empty. Note that steinerleft == -1
	//   if an unlimited number of Steiner points is allowed.
	while(( badsubsegs->items > 0 ) && ( steinerleft != 0 ))
	{
		badsubsegs->traversalinit( );
		bface = (face *)badsubsegs->traverse( );
		while(( bface != NULL ) && ( steinerleft != 0 ))
		{
			// Skip a deleleted element.
			if(bface->shver >= 0)
			{
				// A queued segment may have been deleted (split).
				if(( bface->sh != NULL ) && ( bface->sh[ 3 ] != NULL ))
				{
					// A queued segment may have been processed. 
					if(smarktest2ed( *bface ))
					{
						sunmarktest2( *bface );
						if(checkseg4split( bface, encpt, qflag ))
						{
							splitsegment( bface, encpt, 0, NULL, NULL, qflag, chkencflag );
						}
					}
				}
				// Remove this entry from list.
				bface->shver = -1; // Signal it as a deleted element.
				badsubsegs->dealloc( (void *)bface );
			}
			bface = (face *)badsubsegs->traverse( );
		}
	}

	if(badsubsegs->items > 0)
	{
		if(b->verbose)
		{
			myprintf( "The desired number of Steiner points is reached.\n" );
		}
		badsubsegs->traversalinit( );
		bface = (face *)badsubsegs->traverse( );
		while(bface != NULL)
		{
			// Skip a deleleted element.
			if(bface->shver >= 0)
			{
				if(( bface->sh != NULL ) && ( bface->sh[ 3 ] != NULL ))
				{
					if(smarktest2ed( *bface ))
					{
						sunmarktest2( *bface );
					}
				}
			}
			bface = (face *)badsubsegs->traverse( );
		}
		badsubsegs->restart( );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// checkfac4encroach()    Check if a subface is encroached by a point.       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::checkfac4encroach( point pa, point pb, point pc, point checkpt,
								   REAL* cent, REAL* r, REAL* uv )
{
	REAL rd, len;

	// UV: TODO
	if(uv)
	{
		for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
		{
			uv[ tt ] = 1999;
		}
	}

	circumsphere( pa, pb, pc, NULL, cent, &rd );
	if(rd == 0)
	{
		terminatetetgen( this, 2 );
	}
	len = distance( cent, checkpt );
	if(( fabs( len - rd ) / rd ) < b->epsilon) len = rd; // Rounding.

	if(len < rd)
	{
		// The point lies inside the circumsphere of this face.
		if(b->metric)
		{ // -m option.
			if(( pa[ pointmtrindex ] > 0 ) && ( pb[ pointmtrindex ] > 0 ) &&
				( pc[ pointmtrindex ] > 0 ))
			{
				// Get the projection of 'checkpt' in the plane of pa, pb, and pc.
				REAL prjpt[ 3 ], n[ 3 ];
				REAL a, a1, a2, a3;
				projpt2face( checkpt, pa, pb, pc, prjpt );
				// Get the face area of [a,b,c].
				facenormal( pa, pb, pc, n, 1, NULL );
				a = sqrt( dot( n, n ) );
				// Get the face areas of [a,b,p], [b,c,p], and [c,a,p].
				facenormal( pa, pb, prjpt, n, 1, NULL );
				a1 = sqrt( dot( n, n ) );
				facenormal( pb, pc, prjpt, n, 1, NULL );
				a2 = sqrt( dot( n, n ) );
				facenormal( pc, pa, prjpt, n, 1, NULL );
				a3 = sqrt( dot( n, n ) );
				if(( fabs( a1 + a2 + a3 - a ) / a ) < b->epsilon)
				{
					// This face contains the projection.
					// Get the mesh size at the location of the projection point.
					rd = a1 / a * pc[ pointmtrindex ]
						+ a2 / a * pa[ pointmtrindex ]
						+ a3 / a * pb[ pointmtrindex ];
					len = distance( prjpt, checkpt );
					if(len < rd)
					{
						return 1; // Encroached.
					}
				}
			}
			else
			{
				return 1;  // No protecting ball. Encroached.
			}
		}
		else
		{
			*r = rd;
			return 1;  // Encroached.
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// checkfac4split()    Check if a subface needs to be split.                 //
//                                                                           //
// A subface needs to be split if it is in the following case:               //
//  (1) It is encroached by an existing vertex.                              //
//  (2) It has bad quality (has a small angle, -q).                          //
//  (3) It's area is larger than a prescribed value (.var).                  //
//                                                                           //
// Return 1 if it needs to be split, otherwise, return 0.                    //
// 'chkfac' represents its longest edge.                                     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::checkfac4split( face *chkfac, point& encpt, int& qflag,
								REAL *cent, REAL* uv )
{
	point pa, pb, pc;
	REAL area, rd, len;
	REAL A[ 4 ][ 4 ], rhs[ 4 ], D;
	int indx[ 4 ];
	int i;

	encpt = NULL;
	qflag = 0;

	pa = sorg( *chkfac );
	pb = sdest( *chkfac );
	pc = sapex( *chkfac );

	// UV: Solve identical to vertex coordinates
	{
		// Solve per pairs of 2 UVs (TODO: very naive!)
		for(int tpairs = 0; tpairs < ( tetgenio::pointparam::maxuvs / 2 ); ++tpairs)
		{
			REAL auv[ 2 ], buv[ 2 ], cuv[ 2 ];

			auv[ 0 ] = pointgeomuv( pa, tpairs * 2 + 0 );
			auv[ 1 ] = pointgeomuv( pa, tpairs * 2 + 1 );
			buv[ 0 ] = pointgeomuv( pb, tpairs * 2 + 0 );
			buv[ 1 ] = pointgeomuv( pb, tpairs * 2 + 1 );
			cuv[ 0 ] = pointgeomuv( pc, tpairs * 2 + 0 );
			cuv[ 1 ] = pointgeomuv( pc, tpairs * 2 + 1 );

			// Compute the coefficient matrix A (3x3).
			A[ 0 ][ 0 ] = buv[ 0 ] - auv[ 0 ];
			A[ 0 ][ 1 ] = buv[ 1 ] - auv[ 1 ];
			A[ 0 ][ 2 ] = 0; // vector V1 (pa->pb)
			A[ 1 ][ 0 ] = cuv[ 0 ] - auv[ 0 ];
			A[ 1 ][ 1 ] = cuv[ 1 ] - auv[ 1 ];
			A[ 1 ][ 2 ] = 0; // vector V2 (pa->pc)
			cross( A[ 0 ], A[ 1 ], A[ 2 ] ); // vector V3 (V1 X V2)

			// Compute the right hand side vector b (3x1).
			rhs[ 0 ] = 0.5 * dot( A[ 0 ], A[ 0 ] ); // edge [a,b]
			rhs[ 1 ] = 0.5 * dot( A[ 1 ], A[ 1 ] ); // edge [a,c]
			rhs[ 2 ] = 0.0;

			// Solve the 3 by 3 equations use LU decomposition with partial 
			//   pivoting and backward and forward substitute.
			if(!lu_decmp( A, 3, indx, &D, 0 ))
			{
				// A degenerate triangle. 
				//terminatetetgen( this, 2 );
				uv[ tpairs * 2 + 0 ] = 0;
				uv[ tpairs * 2 + 1 ] = 0;
			}
			else
			{
				lu_solve( A, 3, indx, rhs, 0 );
				uv[ tpairs * 2 + 0 ] = auv[ 0 ] + rhs[ 0 ];
				uv[ tpairs * 2 + 1 ] = auv[ 1 ] + rhs[ 1 ];
			}
		}
	}

	// Compute the coefficient matrix A (3x3).
	A[ 0 ][ 0 ] = pb[ 0 ] - pa[ 0 ];
	A[ 0 ][ 1 ] = pb[ 1 ] - pa[ 1 ];
	A[ 0 ][ 2 ] = pb[ 2 ] - pa[ 2 ]; // vector V1 (pa->pb)
	A[ 1 ][ 0 ] = pc[ 0 ] - pa[ 0 ];
	A[ 1 ][ 1 ] = pc[ 1 ] - pa[ 1 ];
	A[ 1 ][ 2 ] = pc[ 2 ] - pa[ 2 ]; // vector V2 (pa->pc)
	cross( A[ 0 ], A[ 1 ], A[ 2 ] ); // vector V3 (V1 X V2)

	area = 0.5 * sqrt( dot( A[ 2 ], A[ 2 ] ) ); // The area of [a,b,c].

	// Compute the right hand side vector b (3x1).
	rhs[ 0 ] = 0.5 * dot( A[ 0 ], A[ 0 ] ); // edge [a,b]
	rhs[ 1 ] = 0.5 * dot( A[ 1 ], A[ 1 ] ); // edge [a,c]
	rhs[ 2 ] = 0.0;

	// Solve the 3 by 3 equations use LU decomposition with partial 
	//   pivoting and backward and forward substitute.
	if(!lu_decmp( A, 3, indx, &D, 0 ))
	{
		// A degenerate triangle. 
		terminatetetgen( this, 2 );
	}

	lu_solve( A, 3, indx, rhs, 0 );
	cent[ 0 ] = pa[ 0 ] + rhs[ 0 ];
	cent[ 1 ] = pa[ 1 ] + rhs[ 1 ];
	cent[ 2 ] = pa[ 2 ] + rhs[ 2 ];
	rd = sqrt( rhs[ 0 ] * rhs[ 0 ] + rhs[ 1 ] * rhs[ 1 ] + rhs[ 2 ] * rhs[ 2 ] );

	if(checkconstraints && ( areabound( *chkfac ) > 0.0 ))
	{
		// Check if the subface has too big area.
		if(area > areabound( *chkfac ))
		{
			qflag = 1;
			return 1;
		}
	}

	if(b->fixedvolume)
	{
		if(( area * sqrt( area ) ) > b->maxvolume)
		{
			qflag = 1;
			return 1;
		}
	}

	if(b->varvolume)
	{
		triface adjtet;
		REAL volbnd;
		int t1ver;

		stpivot( *chkfac, adjtet );
		if(!ishulltet( adjtet ))
		{
			volbnd = volumebound( adjtet.tet );
			if(( volbnd > 0 ) && ( area * sqrt( area ) ) > volbnd)
			{
				qflag = 1;
				return 1;
			}
		}
		fsymself( adjtet );
		if(!ishulltet( adjtet ))
		{
			volbnd = volumebound( adjtet.tet );
			if(( volbnd > 0 ) && ( area * sqrt( area ) ) > volbnd)
			{
				qflag = 1;
				return 1;
			}
		}
	}

	if(b->metric)
	{ // -m option. Check mesh size. 
		// Check if the ccent lies outside one of the prot.balls at vertices.
		if(( ( pa[ pointmtrindex ] > 0 ) && ( rd > pa[ pointmtrindex ] ) ) ||
			( ( pb[ pointmtrindex ] > 0 ) && ( rd > pb[ pointmtrindex ] ) ) ||
			( ( pc[ pointmtrindex ] > 0 ) && ( rd > pc[ pointmtrindex ] ) ))
		{
			qflag = 1; // Enforce mesh size.
			return 1;
		}
	}

	triface searchtet;
	REAL smlen = 0;

	// Check if this subface is locally encroached.
	for(i = 0; i < 2; i++)
	{
		stpivot( *chkfac, searchtet );
		if(!ishulltet( searchtet ))
		{
			len = distance( oppo( searchtet ), cent );
			if(( fabs( len - rd ) / rd ) < b->epsilon) len = rd;// Rounding.
			if(len < rd)
			{
				if(smlen == 0)
				{
					smlen = len;
					encpt = oppo( searchtet );
				}
				else
				{
					if(len < smlen)
					{
						smlen = len;
						encpt = oppo( searchtet );
					}
				}
				//return 1;
			}
		}
		sesymself( *chkfac );
	}

	return encpt != NULL; //return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// splitsubface()    Split a subface.                                        //
//                                                                           //
// The subface may be encroached, or in bad-quality. It is split at its cir- //
// cumcenter ('ccent'). Do not split it if 'ccent' encroaches upon any seg-  //
// ment. Instead, one of the encroached segments is split.  It is possible   //
// that none of the encroached segments can be split.                        //
//                                                                           //
// The return value indicates whether a new point is inserted (> 0) or not   //
// (= 0).  Furthermore, it is inserted on an encroached segment (= 1) or     //
// in-side the facet (= 2).                                                  //
//                                                                           //
// 'encpt' is a vertex encroaching upon this subface, i.e., it causes the    //
// split of this subface. If 'encpt' is NULL, then the cause of the split    //
// this subface is a rejected tet circumcenter 'p', and 'encpt1' is the      //
// parent of 'p'.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::splitsubface( face *splitfac, point encpt, point encpt1,
							  int qflag, REAL *ccent, REAL* uv, int chkencflag )
{

	if(!qflag && smarktest3ed( *splitfac ))
	{
		// Do not try to re-split a marked subface.
		return 0;
	}

	if(b->nobisect)
	{ // With -Y option.
		if(checkconstraints)
		{
			// Only split if it is allowed to be split.
			// Check if this facet has a non-zero constraint.
			if(areabound( *splitfac ) == 0)
			{
				return 0; // Do not split it.
			}
		}
		else
		{
			return 0;
		}
	} // if (b->nobisect)

	if(useinsertradius)
	{
		if(encpt != NULL)
		{
			REAL rp; // Insertion radius of newpt.
			REAL rv = distance( encpt, ccent );
			if(pointtype( encpt ) == FREESEGVERTEX)
			{
				face parentseg;
				sdecode( point2sh( encpt ), parentseg );
				if(segfacetadjacent( &parentseg, splitfac ))
				{
					rp = getpointinsradius( encpt );
					if(rv < ( sqrt( 2.0 ) * rp ))
					{
						// This insertion may cause no termination. 
						return 0; // Reject the insertion of newpt.
					}
				}
			}
			else if(pointtype( encpt ) == FREEFACETVERTEX)
			{
				face parentsh;
				sdecode( point2sh( encpt ), parentsh );
				if(facetfacetadjacent( &parentsh, splitfac ))
				{
					rp = getpointinsradius( encpt );
					if(rv < rp)
					{
						return 0; // Reject the insertion of newpt.
					}
				}
			}
		}
	} // if (useinsertradius)

	face searchsh;
	insertvertexflags ivf;
	point newpt;
	int i;

	// Initialize the inserting point.
	makepoint( &newpt, FREEFACETVERTEX );
	// Split the subface at its circumcenter.
	for(i = 0; i < 3; i++) newpt[ i ] = ccent[ i ];
	// UV
	for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
	{
		setpointgeomuv( newpt, tt, uv[ tt ] );
	}

	// Search a subface which contains 'newpt'.
	searchsh = *splitfac;
	// Calculate an above point. It lies above the plane containing
	//   the subface [a,b,c], and save it in dummypoint. Moreover,
	//   the vector cent->dummypoint is the normal of the plane.
	calculateabovepoint4( newpt, sorg( *splitfac ), sdest( *splitfac ),
						  sapex( *splitfac ) );
	//   Parameters: 'aflag' = 1, - above point exists.
	//   'cflag' = 0, - non-convex, check co-planarity of the result.
	//   'rflag' = 0, - no need to round the locating result.
	ivf.iloc = (int)slocate( newpt, &searchsh, 1, 0, 0 );

	if(!( ( ivf.iloc == (int)ONFACE ) || ( ivf.iloc == (int)ONEDGE ) ))
	{
		// Point location failed.
		pointdealloc( newpt );
		// Mark this subface to avoid splitting in the future.
		smarktest3( *splitfac );
		return 0;
	}


	triface searchtet;

	// Insert the point.
	stpivot( searchsh, searchtet );
	ivf.bowywat = 3; // Use Bowyer-Watson. Preserve subsegments and subfaces;
	ivf.lawson = 2;
	ivf.rejflag = 1; // Do check the encroachment of segments.
	if(b->metric)
	{
		ivf.rejflag |= 4;  // Do check encroachment of protecting balls.
	}
	ivf.chkencflag = chkencflag;
	ivf.sloc = (int)INSTAR; // ivf.iloc;
	ivf.sbowywat = 3; // ivf.bowywat;
	ivf.splitbdflag = 1;
	ivf.validflag = 1;
	ivf.respectbdflag = 1;
	ivf.assignmeshsize = b->metric;
	ivf.refineflag = 2;
	ivf.refinesh = *splitfac;
	ivf.smlenflag = useinsertradius; // Update the insertion radius.


	if(insertpoint( newpt, &searchtet, &searchsh, NULL, &ivf ))
	{
		st_facref_count++;
		if(steinerleft > 0) steinerleft--;
		if(useinsertradius)
		{
			save_facetpoint_insradius( newpt, ivf.parentpt, ivf.smlen );
		} // if (useinsertradius)
		if(flipstack != NULL)
		{
			flipconstraints fc;
			fc.chkencflag = chkencflag;
			fc.enqflag = 2;
			lawsonflip3d( &fc );
			unflipqueue->restart( );
		}
		return 1;
	}
	else
	{
		// Point was not inserted.
		pointdealloc( newpt );
		if(ivf.iloc == (int)ENCSEGMENT)
		{
			// Select an encroached segment and split it.
			face *paryseg;
			int splitflag = 0;
			for(i = 0; i < encseglist->objects; i++)
			{
				paryseg = (face *)fastlookup( encseglist, i );
				if(splitsegment( paryseg, NULL, 0.0, encpt, encpt1, qflag,
					chkencflag | 1 ))
				{
					splitflag = 1; // A point is inserted on a segment.
					break;
				}
			} // i
			encseglist->restart( );
			if(splitflag)
			{
				// Some segments may need to be repaired.
				if(badsubsegs->items > 0)
				{
					repairencsegs( chkencflag | 1 );
				}
				return 1;
			}
		}
		else
		{
			if(ivf.iloc == (int)NEARVERTEX)
			{
				terminatetetgen( this, 2 );
			}
		}
		// Mark this subface to avoid splitting in the future.
		smarktest3( *splitfac );
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// repairencfacs()    Repair encroached subfaces.                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::repairencfacs( int chkencflag )
{
	face *bface;
	point encpt = NULL;
	int qflag = 0;
	REAL ccent[ 3 ], uv[ tetgenio::pointparam::maxuvs ];

	// Loop until the pool 'badsubfacs' is empty. Note that steinerleft == -1
	//   if an unlimited number of Steiner points is allowed.
	while(( badsubfacs->items > 0 ) && ( steinerleft != 0 ))
	{
		badsubfacs->traversalinit( );
		bface = (face *)badsubfacs->traverse( );
		while(( bface != NULL ) && ( steinerleft != 0 ))
		{
			// Skip a deleted element.
			if(bface->shver >= 0)
			{
				// A queued subface may have been deleted (split).
				if(( bface->sh != NULL ) && ( bface->sh[ 3 ] != NULL ))
				{
					// A queued subface may have been processed. 
					if(smarktest2ed( *bface ))
					{
						sunmarktest2( *bface );
						if(checkfac4split( bface, encpt, qflag, ccent, uv ))
						{
							splitsubface( bface, encpt, NULL, qflag, ccent, uv, chkencflag );
						}
					}
				}
				bface->shver = -1; // Signal it as a deleted element.
				badsubfacs->dealloc( (void *)bface ); // Remove this entry from list.
			}
			bface = (face *)badsubfacs->traverse( );
		}
	}

	if(badsubfacs->items > 0)
	{
		if(steinerleft == 0)
		{
			if(b->verbose)
			{
				myprintf( "The desired number of Steiner points is reached.\n" );
			}
		}
		else
		{
			terminatetetgen( this, 2 );
		}
		badsubfacs->traversalinit( );
		bface = (face *)badsubfacs->traverse( );
		while(bface != NULL)
		{
			// Skip a deleted element.
			if(bface->shver >= 0)
			{
				if(( bface->sh != NULL ) && ( bface->sh[ 3 ] != NULL ))
				{
					if(smarktest2ed( *bface ))
					{
						sunmarktest2( *bface );
					}
				}
			}
			bface = (face *)badsubfacs->traverse( );
		}
		badsubfacs->restart( );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// checktet4split()    Check if the tet needs to be split.                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::checktet4split( triface *chktet, int &qflag, REAL *ccent )
{
	point pa, pb, pc, pd, *ppt;
	REAL vda[ 3 ], vdb[ 3 ], vdc[ 3 ];
	REAL vab[ 3 ], vbc[ 3 ], vca[ 3 ];
	REAL N[ 4 ][ 3 ], L[ 4 ], cosd[ 6 ], elen[ 6 ];
	REAL maxcosd, vol, volbnd, smlen = 0, rd;
	REAL A[ 4 ][ 4 ], rhs[ 4 ], D;
	int indx[ 4 ];
	int i, j;

	if(b->convex)
	{ // -c
		// Skip this tet if it lies in the exterior.
		if(elemattribute( chktet->tet, numelemattrib - 1 ) == -1.0)
		{
			return 0;
		}
	}

	qflag = 0;

	pd = (point)chktet->tet[ 7 ];
	if(pd == dummypoint)
	{
		return 0; // Do not split a hull tet.
	}

	pa = (point)chktet->tet[ 4 ];
	pb = (point)chktet->tet[ 5 ];
	pc = (point)chktet->tet[ 6 ];

	// Get the edge vectors vda: d->a, vdb: d->b, vdc: d->c.
	// Set the matrix A = [vda, vdb, vdc]^T.
	for(i = 0; i < 3; i++) A[ 0 ][ i ] = vda[ i ] = pa[ i ] - pd[ i ];
	for(i = 0; i < 3; i++) A[ 1 ][ i ] = vdb[ i ] = pb[ i ] - pd[ i ];
	for(i = 0; i < 3; i++) A[ 2 ][ i ] = vdc[ i ] = pc[ i ] - pd[ i ];

	// Get the other edge vectors.
	for(i = 0; i < 3; i++) vab[ i ] = pb[ i ] - pa[ i ];
	for(i = 0; i < 3; i++) vbc[ i ] = pc[ i ] - pb[ i ];
	for(i = 0; i < 3; i++) vca[ i ] = pa[ i ] - pc[ i ];

	if(!lu_decmp( A, 3, indx, &D, 0 ))
	{
		// A degenerated tet (vol = 0).
		// This is possible due to the use of exact arithmetic.  We temporarily
		//   leave this tet. It should be fixed by mesh optimization.
		return 0;
	}

	// Check volume if '-a#' and '-a' options are used.
	if(b->varvolume || b->fixedvolume)
	{
		vol = fabs( A[ indx[ 0 ] ][ 0 ] * A[ indx[ 1 ] ][ 1 ] * A[ indx[ 2 ] ][ 2 ] ) / 6.0;
		if(b->fixedvolume)
		{
			if(vol > b->maxvolume)
			{
				qflag = 1;
			}
		}
		if(!qflag && b->varvolume)
		{
			volbnd = volumebound( chktet->tet );
			if(( volbnd > 0.0 ) && ( vol > volbnd ))
			{
				qflag = 1;
			}
		}
		if(qflag == 1)
		{
			// Calculate the circumcenter of this tet.
			rhs[ 0 ] = 0.5 * dot( vda, vda );
			rhs[ 1 ] = 0.5 * dot( vdb, vdb );
			rhs[ 2 ] = 0.5 * dot( vdc, vdc );
			lu_solve( A, 3, indx, rhs, 0 );
			for(i = 0; i < 3; i++) ccent[ i ] = pd[ i ] + rhs[ i ];
			return 1;
		}
	}

	if(b->metric)
	{ // -m option. Check mesh size. 
		// Calculate the circumradius of this tet.
		rhs[ 0 ] = 0.5 * dot( vda, vda );
		rhs[ 1 ] = 0.5 * dot( vdb, vdb );
		rhs[ 2 ] = 0.5 * dot( vdc, vdc );
		lu_solve( A, 3, indx, rhs, 0 );
		for(i = 0; i < 3; i++) ccent[ i ] = pd[ i ] + rhs[ i ];
		rd = sqrt( dot( rhs, rhs ) );
		// Check if the ccent lies outside one of the prot.balls at vertices.
		ppt = (point *)&( chktet->tet[ 4 ] );
		for(i = 0; i < 4; i++)
		{
			if(ppt[ i ][ pointmtrindex ] > 0)
			{
				if(rd > ppt[ i ][ pointmtrindex ])
				{
					qflag = 1; // Enforce mesh size.
					return 1;
				}
			}
		}
	}

	if(in->tetunsuitable != NULL)
	{
		// Execute the user-defined meshing sizing evaluation.
		if(( *( in->tetunsuitable ) )( pa, pb, pc, pd, NULL, 0 ))
		{
			// Calculate the circumcenter of this tet.
			rhs[ 0 ] = 0.5 * dot( vda, vda );
			rhs[ 1 ] = 0.5 * dot( vdb, vdb );
			rhs[ 2 ] = 0.5 * dot( vdc, vdc );
			lu_solve( A, 3, indx, rhs, 0 );
			for(i = 0; i < 3; i++) ccent[ i ] = pd[ i ] + rhs[ i ];
			return 1;
		}
	}

	if(useinsertradius)
	{
		// Do not split this tet if the shortest edge is shorter than the
		//   insertion radius of one of its endpoints.
		triface checkedge;
		point e1, e2;
		REAL rrv, smrrv;

		// Get the shortest edge of this tet.
		checkedge.tet = chktet->tet;
		for(i = 0; i < 6; i++)
		{
			checkedge.ver = edge2ver[ i ];
			e1 = org( checkedge );
			e2 = dest( checkedge );
			elen[ i ] = distance( e1, e2 );
			if(i == 0)
			{
				smlen = elen[ i ];
				j = 0;
			}
			else
			{
				if(elen[ i ] < smlen)
				{
					smlen = elen[ i ];
					j = i;
				}
			}
		}
		// Check if the edge is too short.
		checkedge.ver = edge2ver[ j ];
		// Get the smallest rrv of e1 and e2.
		// Note: if rrv of e1 and e2 is zero. Do not use it.
		e1 = org( checkedge );
		smrrv = getpointinsradius( e1 );
		e2 = dest( checkedge );
		rrv = getpointinsradius( e2 );
		if(rrv > 0)
		{
			if(smrrv > 0)
			{
				if(rrv < smrrv)
				{
					smrrv = rrv;
				}
			}
			else
			{
				smrrv = rrv;
			}
		}
		if(smrrv > 0)
		{
			// To avoid rounding error, round smrrv before doing comparison.
			if(( fabs( smrrv - smlen ) / smlen ) < b->epsilon)
			{
				smrrv = smlen;
			}
			if(smrrv > smlen)
			{
				return 0;
			}
		}
	} // if (useinsertradius)

	// Check the radius-edge ratio. Set by -q#.
	if(b->minratio > 0)
	{
		// Calculate the circumcenter and radius of this tet.
		rhs[ 0 ] = 0.5 * dot( vda, vda );
		rhs[ 1 ] = 0.5 * dot( vdb, vdb );
		rhs[ 2 ] = 0.5 * dot( vdc, vdc );
		lu_solve( A, 3, indx, rhs, 0 );
		for(i = 0; i < 3; i++) ccent[ i ] = pd[ i ] + rhs[ i ];
		rd = sqrt( dot( rhs, rhs ) );
		if(!useinsertradius)
		{
			// Calculate the shortest edge length.
			elen[ 0 ] = dot( vda, vda );
			elen[ 1 ] = dot( vdb, vdb );
			elen[ 2 ] = dot( vdc, vdc );
			elen[ 3 ] = dot( vab, vab );
			elen[ 4 ] = dot( vbc, vbc );
			elen[ 5 ] = dot( vca, vca );
			smlen = elen[ 0 ]; //sidx = 0;
			for(i = 1; i < 6; i++)
			{
				if(smlen > elen[ i ])
				{
					smlen = elen[ i ]; //sidx = i; 
				}
			}
			smlen = sqrt( smlen );
		}
		D = rd / smlen;
		if(D > b->minratio)
		{
			// A bad radius-edge ratio.
			return 1;
		}
	}

	// Check the minimum dihedral angle. Set by -qq#.
	if(b->mindihedral > 0)
	{
		// Compute the 4 face normals (N[0], ..., N[3]).
		for(j = 0; j < 3; j++)
		{
			for(i = 0; i < 3; i++) N[ j ][ i ] = 0.0;
			N[ j ][ j ] = 1.0;  // Positive means the inside direction
			lu_solve( A, 3, indx, N[ j ], 0 );
		}
		for(i = 0; i < 3; i++) N[ 3 ][ i ] = -N[ 0 ][ i ] - N[ 1 ][ i ] - N[ 2 ][ i ];
		// Normalize the normals.
		for(i = 0; i < 4; i++)
		{
			L[ i ] = sqrt( dot( N[ i ], N[ i ] ) );
			if(L[ i ] == 0)
			{
				terminatetetgen( this, 2 );
			}
			for(j = 0; j < 3; j++) N[ i ][ j ] /= L[ i ];
		}
		// Calculate the six dihedral angles.
		cosd[ 0 ] = -dot( N[ 0 ], N[ 1 ] ); // Edge cd, bd, bc.
		cosd[ 1 ] = -dot( N[ 0 ], N[ 2 ] );
		cosd[ 2 ] = -dot( N[ 0 ], N[ 3 ] );
		cosd[ 3 ] = -dot( N[ 1 ], N[ 2 ] ); // Edge ad, ac
		cosd[ 4 ] = -dot( N[ 1 ], N[ 3 ] );
		cosd[ 5 ] = -dot( N[ 2 ], N[ 3 ] ); // Edge ab
		// Get the smallest dihedral angle.
		//maxcosd = mincosd = cosd[0];
		maxcosd = cosd[ 0 ];
		for(i = 1; i < 6; i++)
		{
			//if (cosd[i] > maxcosd) maxcosd = cosd[i];
			maxcosd = ( cosd[ i ] > maxcosd ? cosd[ i ] : maxcosd );
			//mincosd = (cosd[i] < mincosd ? cosd[i] : maxcosd);
		}
		if(maxcosd > cosmindihed)
		{
			// Calculate the circumcenter of this tet.
			// A bad dihedral angle.
			//if ((b->quality & 1) == 0) {
			rhs[ 0 ] = 0.5 * dot( vda, vda );
			rhs[ 1 ] = 0.5 * dot( vdb, vdb );
			rhs[ 2 ] = 0.5 * dot( vdc, vdc );
			lu_solve( A, 3, indx, rhs, 0 );
			for(i = 0; i < 3; i++) ccent[ i ] = pd[ i ] + rhs[ i ];
			//*rd = sqrt(dot(rhs, rhs));
			//}
			return 1;
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// splittetrahedron()    Split a tetrahedron.                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::splittetrahedron( triface* splittet, int qflag, REAL *ccent,
								  int chkencflag )
{
	triface searchtet;
	face *paryseg;
	point newpt, *ppt;
	badface *bface;
	insertvertexflags ivf;
	int splitflag = 0;
	int i;



	REAL rv = 0.; // Insertion radius of 'newpt'.

	makepoint( &newpt, FREEVOLVERTEX );
	for(i = 0; i < 3; i++) newpt[ i ] = ccent[ i ];
	// UV: TODO (only called for internal vertices?)
	setpointgeomuv( newpt, 0, 9.0 );
	setpointgeomuv( newpt, 1, 9.0 );

	if(useinsertradius)
	{
		rv = distance( newpt, org( *splittet ) );
		setpointinsradius( newpt, rv );
	}

	// Locate the new point. Starting from an interior point 'q' of the
	//   splittet. We perform a walk from q to the 'newpt', stop walking
	//   either we hit a subface or enter OUTSIDE.
	searchtet = *splittet;
	ivf.iloc = (int)OUTSIDE;
	ivf.iloc = locate( newpt, &searchtet, 1 ); // 'chkencflag' = 1.

	if(( ivf.iloc == (int)OUTSIDE ) || ( ivf.iloc == (int)ENCSUBFACE ))
	{
		// The circumcenter 'c' is not visible from 'q' (the interior of the tet).
		// iffalse
		if(b->verbose > 2)
		{
			myprintf( "    New point %d is blocked by a polygon.\n", pointmark( newpt ) );
		}
		// \fi
		pointdealloc( newpt );  // Do not insert this vertex.
		if(b->nobisect) return 0; // -Y option.
		// There must be a polygon that blocks the visibility.
		// Search a subpolygon that contains the proj(c).
		face searchsh;
		REAL prjpt[ 3 ];
		locateresult sloc = OUTSIDE;
		tspivot( searchtet, searchsh );
		ppt = (point *)&( searchsh.sh[ 3 ] );
		projpt2face( ccent, ppt[ 0 ], ppt[ 1 ], ppt[ 2 ], prjpt );
		// Locate proj(c) on polygon. 
		sloc = slocate( prjpt, &searchsh, 0, 0, 1 );
		if(( sloc == ONEDGE ) || ( sloc == ONFACE ))
		{
			// Found a subface/edge containing proj(c).
			// Check if 'c' encoraches upon this subface.
			REAL fcent[ 3 ], r = 0, uv[ tetgenio::pointparam::maxuvs ];
			ppt = (point *)&( searchsh.sh[ 3 ] );
			if(checkfac4encroach( ppt[ 0 ], ppt[ 1 ], ppt[ 2 ], ccent, fcent, &r, uv ))
			{
				// Encroached. Split this subface.
				splitflag = splitsubface( &searchsh, NULL, org( *splittet ), qflag,
										  fcent, uv, chkencflag | 2 );
				if(splitflag)
				{
					// Some subfaces may need to be repaired.
					repairencfacs( chkencflag | 2 );
				}
			}
		}
		else if(( sloc == OUTSIDE ) || ( sloc == ENCSEGMENT ))
		{
			// Hit a segment. We should split it. 
			// To be done...
		}
		if(splitflag)
		{
			// Queue the tet if it is still alive.
			if(( splittet->tet != NULL ) && ( splittet->tet[ 4 ] != NULL ))
			{
				enqueuetetrahedron( splittet );
			}
		}
		return splitflag;
	}



	// Use Bowyer-Watson algorithm. Preserve subsegments and subfaces;
	ivf.bowywat = 3;
	ivf.lawson = 2;
	ivf.rejflag = 3;  // Do check for encroached segments and subfaces.
	if(b->metric)
	{
		ivf.rejflag |= 4; // Reject it if it lies in some protecting balls.
	}
	ivf.chkencflag = chkencflag;
	ivf.sloc = ivf.sbowywat = 0; // No use.
	ivf.splitbdflag = 0; // No use.
	ivf.validflag = 1;
	ivf.respectbdflag = 1;
	ivf.assignmeshsize = b->metric;

	ivf.refineflag = 1;
	ivf.refinetet = *splittet;


	if(insertpoint( newpt, &searchtet, NULL, NULL, &ivf ))
	{
		// Vertex is inserted.
		st_volref_count++;
		if(steinerleft > 0) steinerleft--;
		if(flipstack != NULL)
		{
			flipconstraints fc;
			fc.chkencflag = chkencflag;
			fc.enqflag = 2;
			lawsonflip3d( &fc );
			unflipqueue->restart( );
		}
		return 1;
	}
	else
	{
		// Point is not inserted.
		pointdealloc( newpt );
		// Check if there are encroached segments/subfaces.
		if(ivf.iloc == (int)ENCSEGMENT)
		{
			if(!b->nobisect || checkconstraints)
			{
				// Select an encroached segment and split it.
				for(i = 0; i < encseglist->objects; i++)
				{
					paryseg = (face *)fastlookup( encseglist, i );
					if(splitsegment( paryseg, NULL, rv, org( *splittet ), NULL, qflag,
						chkencflag | 3 ))
					{
						splitflag = 1; // A point is inserted on a segment.
						break;
					}
				}
			} // if (!b->nobisect)
			encseglist->restart( );
			if(splitflag)
			{
				// Some segments may need to be repaired.
				if(badsubsegs->items > 0)
				{
					repairencsegs( chkencflag | 3 );
				}
				// Some subfaces may need to be repaired.
				if(badsubfacs->items > 0)
				{
					repairencfacs( chkencflag | 2 );
				}
			}
		}
		else if(ivf.iloc == (int)ENCSUBFACE)
		{
			if(!b->nobisect || checkconstraints)
			{
				// Select an encroached subface and split it.
				for(i = 0; i < encshlist->objects; i++)
				{
					bface = (badface *)fastlookup( encshlist, i );

					REAL uv[ tetgenio::pointparam::maxuvs ];
					// UV: Use circumcenter UV coordinates
					{
						// Solve per pairs of 2 UVs (TODO: very naive!)
						for(int tpairs = 0; tpairs < ( tetgenio::pointparam::maxuvs / 2 ); ++tpairs)
						{
							REAL A[ 4 ][ 4 ], rhs[ 4 ], D;
							int indx[ 4 ];
							REAL auv[ 2 ], buv[ 2 ], cuv[ 2 ];

							auv[ 0 ] = pointgeomuv( (point)bface->ss.sh[ 3 ], tpairs * 2 + 0 );
							auv[ 1 ] = pointgeomuv( (point)bface->ss.sh[ 3 ], tpairs * 2 + 1 );
							buv[ 0 ] = pointgeomuv( (point)bface->ss.sh[ 4 ], tpairs * 2 + 0 );
							buv[ 1 ] = pointgeomuv( (point)bface->ss.sh[ 4 ], tpairs * 2 + 1 );
							cuv[ 0 ] = pointgeomuv( (point)bface->ss.sh[ 5 ], tpairs * 2 + 0 );
							cuv[ 1 ] = pointgeomuv( (point)bface->ss.sh[ 5 ], tpairs * 2 + 1 );

							// Compute the coefficient matrix A (3x3).
							A[ 0 ][ 0 ] = buv[ 0 ] - auv[ 0 ];
							A[ 0 ][ 1 ] = buv[ 1 ] - auv[ 1 ];
							A[ 0 ][ 2 ] = 0; // vector V1 (pa->pb)
							A[ 1 ][ 0 ] = cuv[ 0 ] - auv[ 0 ];
							A[ 1 ][ 1 ] = cuv[ 1 ] - auv[ 1 ];
							A[ 1 ][ 2 ] = 0; // vector V2 (pa->pc)
							cross( A[ 0 ], A[ 1 ], A[ 2 ] ); // vector V3 (V1 X V2)

							// Compute the right hand side vector b (3x1).
							rhs[ 0 ] = 0.5 * dot( A[ 0 ], A[ 0 ] ); // edge [a,b]
							rhs[ 1 ] = 0.5 * dot( A[ 1 ], A[ 1 ] ); // edge [a,c]
							rhs[ 2 ] = 0.0;

							// Solve the 3 by 3 equations use LU decomposition with partial 
							//   pivoting and backward and forward substitute.
							if(!lu_decmp( A, 3, indx, &D, 0 ))
							{
								// A degenerate triangle. 
								//terminatetetgen( this, 2 );
								uv[ tpairs * 2 + 0 ] = 0;
								uv[ tpairs * 2 + 1 ] = 0;
							}
							else
							{
								lu_solve( A, 3, indx, rhs, 0 );
								uv[ tpairs * 2 + 0 ] = auv[ 0 ] + rhs[ 0 ];
								uv[ tpairs * 2 + 1 ] = auv[ 1 ] + rhs[ 1 ];
							}
						}
					}
					if(splitsubface( &( bface->ss ), NULL, org( *splittet ), qflag,
						bface->cent, uv, chkencflag | 2 ))
					{
						splitflag = 1; // A point is inserted on a subface or a segment.
						break;
					}
				}
			} // if (!b->nobisect)
			encshlist->restart( );
			if(splitflag)
			{
				// Some subfaces may need to be repaired.
				if(badsubfacs->items > 0)
				{
					repairencfacs( chkencflag | 2 );
				}
			}
		}
		else
		{
			if(ivf.iloc == (int)NEARVERTEX)
			{
				terminatetetgen( this, 2 );
			}
		}
		if(splitflag)
		{
			// Queue the tet if it is still alive.
			if(( splittet->tet != NULL ) && ( splittet->tet[ 4 ] != NULL ))
			{
				enqueuetetrahedron( splittet );
			}
		}
		return splitflag;
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// repairbadtets()    Repair bad quality tetrahedra.                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::repairbadtets( int chkencflag )
{
	triface *bface;
	REAL ccent[ 3 ];
	int qflag = 0;


	// Loop until the pool 'badsubfacs' is empty. Note that steinerleft == -1
	//   if an unlimited number of Steiner points is allowed.
	while(( badtetrahedrons->items > 0 ) && ( steinerleft != 0 ))
	{
		badtetrahedrons->traversalinit( );
		bface = (triface *)badtetrahedrons->traverse( );
		while(( bface != NULL ) && ( steinerleft != 0 ))
		{
			// Skip a deleted element.
			if(bface->ver >= 0)
			{
				// A queued tet may have been deleted.
				if(!isdeadtet( *bface ))
				{
					// A queued tet may have been processed.
					if(marktest2ed( *bface ))
					{
						unmarktest2( *bface );
						if(checktet4split( bface, qflag, ccent ))
						{
							splittetrahedron( bface, qflag, ccent, chkencflag );
						}
					}
				}
				bface->ver = -1; // Signal it as a deleted element.
				badtetrahedrons->dealloc( (void *)bface );
			}
			bface = (triface *)badtetrahedrons->traverse( );
		}
	}

	if(badtetrahedrons->items > 0)
	{
		if(steinerleft == 0)
		{
			if(b->verbose)
			{
				myprintf( "The desired number of Steiner points is reached.\n" );
			}
		}
		else
		{
			terminatetetgen( this, 2 ); // Unknown case.
		}
		// Unmark all queued tet.
		badtetrahedrons->traversalinit( );
		bface = (triface *)badtetrahedrons->traverse( );
		while(bface != NULL)
		{
			// Skip a deleted element.
			if(bface->ver >= 0)
			{
				if(!isdeadtet( *bface ))
				{
					if(marktest2ed( *bface ))
					{
						unmarktest2( *bface );
					}
				}
			}
			bface = (triface *)badtetrahedrons->traverse( );
		}
		// Clear the pool.
		badtetrahedrons->restart( );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// delaunayrefinement()    Refine the mesh by Delaunay refinement.           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::delaunayrefinement( )
{
	triface checktet;
	face checksh;
	face checkseg;
	long steinercount;
	int chkencflag;

	long bak_segref_count, bak_facref_count, bak_volref_count;
	long bak_flipcount = flip23count + flip32count + flip44count;

	if(!b->quiet)
	{
		myprintf( "Refining mesh...\n" );
	}

	if(b->verbose)
	{
		myprintf( "  Min radiu-edge ratio = %g.\n", b->minratio );
		myprintf( "  Min dihedral   angle = %g.\n", b->mindihedral );
		//myprintf("  Min Edge length = %g.\n", b->minedgelength);
	}

	steinerleft = b->steinerleft;  // Upperbound of # Steiner points (by -S#).
	if(steinerleft > 0)
	{
		// Check if we've already used up the given number of Steiner points.
		steinercount = st_segref_count + st_facref_count + st_volref_count;
		if(steinercount < steinerleft)
		{
			steinerleft -= steinercount;
		}
		else
		{
			if(!b->quiet)
			{
				myprintf( "\nWarning:  " );
				myprintf( "The desired number of Steiner points (%d) has reached.\n\n",
						b->steinerleft );
			}
			return; // No more Steiner points.
		}
	}

	if(useinsertradius)
	{
		if(( b->plc && b->nobisect ) || b->refine)
		{ // '-pY' or '-r' option.
			makesegmentendpointsmap( );
			makefacetverticesmap( );
		}
	}


	encseglist = new arraypool( sizeof( face ), 8 );
	encshlist = new arraypool( sizeof( badface ), 8 );


	//if (!b->nobisect) { // if no '-Y' option
	if(!b->nobisect || checkconstraints)
	{
		if(b->verbose)
		{
			myprintf( "  Splitting encroached subsegments.\n" );
		}

		chkencflag = 1; // Only check encroaching subsegments.
		steinercount = points->items;

		// Initialize the pool of encroached subsegments.
		badsubsegs = new memorypool( sizeof( face ), b->shellfaceperblock,
									 sizeof( void * ), 0 );

		// Add all segments into the pool.
		subsegs->traversalinit( );
		checkseg.sh = shellfacetraverse( subsegs );
		while(checkseg.sh != (shellface *)NULL)
		{
			enqueuesubface( badsubsegs, &checkseg );
			checkseg.sh = shellfacetraverse( subsegs );
		}

		// Split all encroached segments.
		repairencsegs( chkencflag );

		if(b->verbose)
		{
			myprintf( "  Added %ld Steiner points.\n", points->items - steinercount );
		}

		if(b->reflevel > 1)
		{ // '-D2' option
			if(b->verbose)
			{
				myprintf( "  Splitting encroached subfaces.\n" );
			}

			chkencflag = 2; // Only check encroaching subfaces.
			steinercount = points->items;
			bak_segref_count = st_segref_count;
			bak_facref_count = st_facref_count;

			// Initialize the pool of encroached subfaces.
			badsubfacs = new memorypool( sizeof( face ), b->shellfaceperblock,
										 sizeof( void * ), 0 );

			// Add all subfaces into the pool.
			subfaces->traversalinit( );
			checksh.sh = shellfacetraverse( subfaces );
			while(checksh.sh != (shellface *)NULL)
			{
				enqueuesubface( badsubfacs, &checksh );
				checksh.sh = shellfacetraverse( subfaces );
			}

			// Split all encroached subfaces.
			repairencfacs( chkencflag );

			if(b->verbose)
			{
				myprintf( "  Added %ld (%ld,%ld) Steiner points.\n",
						points->items - steinercount, st_segref_count - bak_segref_count,
						st_facref_count - bak_facref_count );
			}
		} // if (b->reflevel > 1)
	} // if (!b->nobisect)

	if(b->reflevel > 2)
	{ // '-D3' option (The default option)
		if(b->verbose)
		{
			myprintf( "  Splitting bad quality tets.\n" );
		}

		chkencflag = 4; // Only check tetrahedra.
		steinercount = points->items;
		bak_segref_count = st_segref_count;
		bak_facref_count = st_facref_count;
		bak_volref_count = st_volref_count;

		// The cosine value of the min dihedral angle (-qq) for tetrahedra.
		cosmindihed = cos( b->mindihedral / 180.0 * PIconstant );

		// Initialize the pool of bad quality tetrahedra.
		badtetrahedrons = new memorypool( sizeof( triface ), b->tetrahedraperblock,
										  sizeof( void * ), 0 );
		// Add all tetrahedra (no hull tets) into the pool.
		tetrahedrons->traversalinit( );
		checktet.tet = tetrahedrontraverse( );
		while(checktet.tet != NULL)
		{
			enqueuetetrahedron( &checktet );
			checktet.tet = tetrahedrontraverse( );
		}

		// Split all bad quality tetrahedra.
		repairbadtets( chkencflag );

		if(b->verbose)
		{
			myprintf( "  Added %ld (%ld,%ld,%ld) Steiner points.\n",
					points->items - steinercount,
					st_segref_count - bak_segref_count,
					st_facref_count - bak_facref_count,
					st_volref_count - bak_volref_count );
		}
	} // if (b->reflevel > 2)

	if(b->verbose)
	{
		if(flip23count + flip32count + flip44count > bak_flipcount)
		{
			myprintf( "  Performed %ld flips.\n", flip23count + flip32count +
					flip44count - bak_flipcount );
		}
	}

	if(steinerleft == 0)
	{
		if(!b->quiet)
		{
			myprintf( "\nWarnning:  " );
			myprintf( "The desired number of Steiner points (%d) is reached.\n\n",
					b->steinerleft );
		}
	}


	delete encseglist;
	delete encshlist;
	encseglist = NULL;
	encshlist = NULL;

	if(!b->nobisect || checkconstraints)
	{
		totalworkmemory += ( badsubsegs->maxitems * badsubsegs->itembytes );
		delete badsubsegs;
		badsubsegs = NULL;
		if(b->reflevel > 1)
		{
			totalworkmemory += ( badsubfacs->maxitems * badsubfacs->itembytes );
			delete badsubfacs;
			badsubfacs = NULL;
		}
	}
	if(b->reflevel > 2)
	{
		totalworkmemory += ( badtetrahedrons->maxitems*badtetrahedrons->itembytes );
		delete badtetrahedrons;
		badtetrahedrons = NULL;
	}
}

////                                                                       ////
////                                                                       ////
//// refine_cxx ///////////////////////////////////////////////////////////////

//// optimize_cxx /////////////////////////////////////////////////////////////
////                                                                       ////
////                                                                       ////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// lawsonflip3d()    A three-dimensional Lawson's algorithm.                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

long tetgenmesh::lawsonflip3d( flipconstraints *fc )
{
	triface fliptets[ 5 ], neightet, hulltet;
	face checksh, casingout;
	badface *popface, *bface;
	point pd, pe, *pts;
	REAL sign, ori;
	REAL vol, len3;
	long flipcount, totalcount = 0l;
	long sliver_peels = 0l;
	int t1ver;
	int i;


	while(1)
	{

		if(b->verbose > 2)
		{
			myprintf( "      Lawson flip %ld faces.\n", flippool->items );
		}
		flipcount = 0l;

		while(flipstack != (badface *)NULL)
		{
			// Pop a face from the stack.
			popface = flipstack;
			fliptets[ 0 ] = popface->tt;
			flipstack = flipstack->nextitem; // The next top item in stack.
			flippool->dealloc( (void *)popface );

			// Skip it if it is a dead tet (destroyed by previous flips).
			if(isdeadtet( fliptets[ 0 ] )) continue;
			// Skip it if it is not the same tet as we saved.
			if(!facemarked( fliptets[ 0 ] )) continue;

			unmarkface( fliptets[ 0 ] );

			if(ishulltet( fliptets[ 0 ] )) continue;

			fsym( fliptets[ 0 ], fliptets[ 1 ] );
			if(ishulltet( fliptets[ 1 ] ))
			{
				if(nonconvex)
				{
					// Check if 'fliptets[0]' it is a hull sliver.
					tspivot( fliptets[ 0 ], checksh );
					for(i = 0; i < 3; i++)
					{
						if(!isshsubseg( checksh ))
						{
							spivot( checksh, casingout );
							//assert(casingout.sh != NULL);
							if(sorg( checksh ) != sdest( casingout )) sesymself( casingout );
							stpivot( casingout, neightet );
							if(neightet.tet == fliptets[ 0 ].tet)
							{
								// Found a hull sliver 'neightet'. Let it be [e,d,a,b], where 
								//   [e,d,a] and [d,e,b] are hull faces.
								edestoppo( neightet, hulltet ); // [a,b,e,d]
								fsymself( hulltet ); // [b,a,e,#]
								if(oppo( hulltet ) == dummypoint)
								{
									pe = org( neightet );
									if(( pointtype( pe ) == FREEFACETVERTEX ) ||
										( pointtype( pe ) == FREESEGVERTEX ))
									{
										removevertexbyflips( pe );
									}
								}
								else
								{
									eorgoppo( neightet, hulltet ); // [b,a,d,e]
									fsymself( hulltet ); // [a,b,d,#]
									if(oppo( hulltet ) == dummypoint)
									{
										pd = dest( neightet );
										if(( pointtype( pd ) == FREEFACETVERTEX ) ||
											( pointtype( pd ) == FREESEGVERTEX ))
										{
											removevertexbyflips( pd );
										}
									}
									else
									{
										// Perform a 3-to-2 flip to remove the sliver.
										fliptets[ 0 ] = neightet;          // [e,d,a,b]
										fnext( fliptets[ 0 ], fliptets[ 1 ] ); // [e,d,b,c]
										fnext( fliptets[ 1 ], fliptets[ 2 ] ); // [e,d,c,a]
										flip32( fliptets, 1, fc );
										// Update counters.
										flip32count--;
										flip22count--;
										sliver_peels++;
										if(fc->remove_ndelaunay_edge)
										{
											// Update the volume (must be decreased).
											//assert(fc->tetprism_vol_sum <= 0);
											tetprism_vol_sum += fc->tetprism_vol_sum;
											fc->tetprism_vol_sum = 0.0; // Clear it.
										}
									}
								}
								break;
							} // if (neightet.tet == fliptets[0].tet)
						} // if (!isshsubseg(checksh))
						senextself( checksh );
					} // i
				} // if (nonconvex)
				continue;
			}

			if(checksubfaceflag)
			{
				// Do not flip if it is a subface.
				if(issubface( fliptets[ 0 ] )) continue;
			}

			// Test whether the face is locally Delaunay or not.
			pts = (point *)fliptets[ 1 ].tet;
			sign = insphere_s( pts[ 4 ], pts[ 5 ], pts[ 6 ], pts[ 7 ], oppo( fliptets[ 0 ] ) );

			if(sign < 0)
			{
				// A non-Delaunay face. Try to flip it.
				pd = oppo( fliptets[ 0 ] );
				pe = oppo( fliptets[ 1 ] );

				// Use the length of the edge [d,e] as a reference to determine
				//   a nearly degenerated new tet.
				len3 = distance( pd, pe );
				len3 = ( len3 * len3 * len3 );

				// Check the convexity of its three edges. Stop checking either a
				//   locally non-convex edge (ori < 0) or a flat edge (ori = 0) is
				//   encountered, and 'fliptet' represents that edge.
				for(i = 0; i < 3; i++)
				{
					ori = orient3d( org( fliptets[ 0 ] ), dest( fliptets[ 0 ] ), pd, pe );
					if(ori > 0)
					{
						// Avoid creating a nearly degenerated new tet at boundary.
						//   Re-use fliptets[2], fliptets[3];
						esym( fliptets[ 0 ], fliptets[ 2 ] );
						esym( fliptets[ 1 ], fliptets[ 3 ] );
						if(issubface( fliptets[ 2 ] ) || issubface( fliptets[ 3 ] ))
						{
							vol = orient3dfast( org( fliptets[ 0 ] ), dest( fliptets[ 0 ] ), pd, pe );
							if(( fabs( vol ) / len3 ) < b->epsilon)
							{
								ori = 0.0; // Do rounding.
							}
						}
					} // Rounding check
					if(ori <= 0) break;
					enextself( fliptets[ 0 ] );
					eprevself( fliptets[ 1 ] );
				}

				if(ori > 0)
				{
					// A 2-to-3 flip is found.
					//   [0] [a,b,c,d], 
					//   [1] [b,a,c,e]. no dummypoint.
					flip23( fliptets, 0, fc );
					flipcount++;
					if(fc->remove_ndelaunay_edge)
					{
						// Update the volume (must be decreased).
						//assert(fc->tetprism_vol_sum <= 0);
						tetprism_vol_sum += fc->tetprism_vol_sum;
						fc->tetprism_vol_sum = 0.0; // Clear it.
					}
					continue;
				}
				else
				{ // ori <= 0
					// The edge ('fliptets[0]' = [a',b',c',d]) is non-convex or flat,
					//   where the edge [a',b'] is one of [a,b], [b,c], and [c,a].
					if(checksubsegflag)
					{
						// Do not flip if it is a segment.
						if(issubseg( fliptets[ 0 ] )) continue;
					}
					// Check if there are three or four tets sharing at this edge.        
					esymself( fliptets[ 0 ] ); // [b,a,d,c]
					for(i = 0; i < 3; i++)
					{
						fnext( fliptets[ i ], fliptets[ i + 1 ] );
					}
					if(fliptets[ 3 ].tet == fliptets[ 0 ].tet)
					{
						// A 3-to-2 flip is found. (No hull tet.)
						flip32( fliptets, 0, fc );
						flipcount++;
						if(fc->remove_ndelaunay_edge)
						{
							// Update the volume (must be decreased).
							//assert(fc->tetprism_vol_sum <= 0);
							tetprism_vol_sum += fc->tetprism_vol_sum;
							fc->tetprism_vol_sum = 0.0; // Clear it.
						}
						continue;
					}
					else
					{
						// There are more than 3 tets at this edge.
						fnext( fliptets[ 3 ], fliptets[ 4 ] );
						if(fliptets[ 4 ].tet == fliptets[ 0 ].tet)
						{
							// There are exactly 4 tets at this edge.
							if(nonconvex)
							{
								if(apex( fliptets[ 3 ] ) == dummypoint)
								{
									// This edge is locally non-convex on the hull.
									// It can be removed by a 4-to-4 flip.                  
									ori = 0;
								}
							} // if (nonconvex)
							if(ori == 0)
							{
								// A 4-to-4 flip is found. (Two hull tets may be involved.)
								// Current tets in 'fliptets':
								//   [0] [b,a,d,c] (d may be newpt)
								//   [1] [b,a,c,e]
								//   [2] [b,a,e,f] (f may be dummypoint)
								//   [3] [b,a,f,d]
								esymself( fliptets[ 0 ] ); // [a,b,c,d] 
								// A 2-to-3 flip replaces face [a,b,c] by edge [e,d].
								//   This creates a degenerate tet [e,d,a,b] (tmpfliptets[0]).
								//   It will be removed by the followed 3-to-2 flip.
								flip23( fliptets, 0, fc ); // No hull tet.
								fnext( fliptets[ 3 ], fliptets[ 1 ] );
								fnext( fliptets[ 1 ], fliptets[ 2 ] );
								// Current tets in 'fliptets':
								//   [0] [...]
								//   [1] [b,a,d,e] (degenerated, d may be new point).
								//   [2] [b,a,e,f] (f may be dummypoint)
								//   [3] [b,a,f,d]
								// A 3-to-2 flip replaces edge [b,a] by face [d,e,f].
								//   Hull tets may be involved (f may be dummypoint).
								flip32( &( fliptets[ 1 ] ), ( apex( fliptets[ 3 ] ) == dummypoint ), fc );
								flipcount++;
								flip23count--;
								flip32count--;
								flip44count++;
								if(fc->remove_ndelaunay_edge)
								{
									// Update the volume (must be decreased).
									//assert(fc->tetprism_vol_sum <= 0);
									tetprism_vol_sum += fc->tetprism_vol_sum;
									fc->tetprism_vol_sum = 0.0; // Clear it.
								}
								continue;
							} // if (ori == 0)
						}
					}
				} // if (ori <= 0)

				// This non-Delaunay face is unflippable. Save it.
				unflipqueue->newindex( (void **)&bface );
				bface->tt = fliptets[ 0 ];
				bface->forg = org( fliptets[ 0 ] );
				bface->fdest = dest( fliptets[ 0 ] );
				bface->fapex = apex( fliptets[ 0 ] );
			} // if (sign < 0)
		} // while (flipstack)

		if(b->verbose > 2)
		{
			if(flipcount > 0)
			{
				myprintf( "      Performed %ld flips.\n", flipcount );
			}
		}
		// Accumulate the counter of flips.
		totalcount += flipcount;

		// Return if no unflippable faces left.
		if(unflipqueue->objects == 0l) break;
		// Return if no flip has been performed.
		if(flipcount == 0l) break;

		// Try to flip the unflippable faces.
		for(i = 0; i < unflipqueue->objects; i++)
		{
			bface = (badface *)fastlookup( unflipqueue, i );
			if(!isdeadtet( bface->tt ) &&
				( org( bface->tt ) == bface->forg ) &&
				( dest( bface->tt ) == bface->fdest ) &&
				( apex( bface->tt ) == bface->fapex ))
			{
				flippush( flipstack, &( bface->tt ) );
			}
		}
		unflipqueue->restart( );

	} // while (1)

	if(b->verbose > 2)
	{
		if(totalcount > 0)
		{
			myprintf( "      Performed %ld flips.\n", totalcount );
		}
		if(sliver_peels > 0)
		{
			myprintf( "      Removed %ld hull slivers.\n", sliver_peels );
		}
		if(unflipqueue->objects > 0l)
		{
			myprintf( "      %ld unflippable edges remained.\n", unflipqueue->objects );
		}
	}

	return totalcount + sliver_peels;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// recoverdelaunay()    Recovery the locally Delaunay property.              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::recoverdelaunay( )
{
	arraypool *flipqueue, *nextflipqueue, *swapqueue;
	triface tetloop, neightet, *parytet;
	badface *bface, *parybface;
	point *ppt;
	flipconstraints fc;
	int i, j;

	if(!b->quiet)
	{
		myprintf( "Recovering Delaunayness...\n" );
	}

	tetprism_vol_sum = 0.0; // Initialize it.

	// Put all interior faces of the mesh into 'flipstack'.
	tetrahedrons->traversalinit( );
	tetloop.tet = tetrahedrontraverse( );
	while(tetloop.tet != NULL)
	{
		for(tetloop.ver = 0; tetloop.ver < 4; tetloop.ver++)
		{
			decode( tetloop.tet[ tetloop.ver ], neightet );
			if(!facemarked( neightet ))
			{
				flippush( flipstack, &tetloop );
			}
		}
		ppt = (point *)&( tetloop.tet[ 4 ] );
		tetprism_vol_sum += tetprismvol( ppt[ 0 ], ppt[ 1 ], ppt[ 2 ], ppt[ 3 ] );
		tetloop.tet = tetrahedrontraverse( );
	}

	// Calulate a relatively lower bound for small improvement. 
	//   Used to avoid rounding error in volume calculation.
	fc.bak_tetprism_vol = tetprism_vol_sum * b->epsilon * 1e-3;

	if(b->verbose)
	{
		myprintf( "  Initial obj = %.17g\n", tetprism_vol_sum );
	}

	if(b->verbose > 1)
	{
		myprintf( "    Recover Delaunay [Lawson] : %ld\n", flippool->items );
	}

	// First only use the basic Lawson's flip.
	fc.remove_ndelaunay_edge = 1;
	fc.enqflag = 2;

	lawsonflip3d( &fc );

	if(b->verbose > 1)
	{
		myprintf( "    obj (after Lawson) = %.17g\n", tetprism_vol_sum );
	}

	if(unflipqueue->objects == 0l)
	{
		return; // The mesh is Delaunay.
	}

	fc.unflip = 1; // Unflip if the edge is not flipped.
	fc.collectnewtets = 1; // new tets are returned in 'cavetetlist'.
	fc.enqflag = 0;

	autofliplinklevel = 1; // Init level.
	b->fliplinklevel = -1; // No fixed level.

	// For efficiency reason, we limit the maximium size of the edge star.
	int bakmaxflipstarsize = b->flipstarsize;
	b->flipstarsize = 10; // default

	flipqueue = new arraypool( sizeof( badface ), 10 );
	nextflipqueue = new arraypool( sizeof( badface ), 10 );

	// Swap the two flip queues.
	swapqueue = flipqueue;
	flipqueue = unflipqueue;
	unflipqueue = swapqueue;

	while(flipqueue->objects > 0l)
	{

		if(b->verbose > 1)
		{
			myprintf( "    Recover Delaunay [level = %2d] #:  %ld.\n",
					autofliplinklevel, flipqueue->objects );
		}

		for(i = 0; i < flipqueue->objects; i++)
		{
			bface = (badface *)fastlookup( flipqueue, i );
			if(getedge( bface->forg, bface->fdest, &bface->tt ))
			{
				if(removeedgebyflips( &( bface->tt ), &fc ) == 2)
				{
					tetprism_vol_sum += fc.tetprism_vol_sum;
					fc.tetprism_vol_sum = 0.0; // Clear it.
					// Queue new faces for flips.
					for(j = 0; j < cavetetlist->objects; j++)
					{
						parytet = (triface *)fastlookup( cavetetlist, j );
						// A queued new tet may be dead.
						if(!isdeadtet( *parytet ))
						{
							for(parytet->ver = 0; parytet->ver < 4; parytet->ver++)
							{
								// Avoid queue a face twice.
								decode( parytet->tet[ parytet->ver ], neightet );
								if(!facemarked( neightet ))
								{
									flippush( flipstack, parytet );
								}
							} // parytet->ver
						}
					} // j
					cavetetlist->restart( );
					// Remove locally non-Delaunay faces. New non-Delaunay edges
					//   may be found. They are saved in 'unflipqueue'.
					fc.enqflag = 2;
					lawsonflip3d( &fc );
					fc.enqflag = 0;
					// There may be unflipable faces. Add them in flipqueue.
					for(j = 0; j < unflipqueue->objects; j++)
					{
						bface = (badface *)fastlookup( unflipqueue, j );
						flipqueue->newindex( (void **)&parybface );
						*parybface = *bface;
					}
					unflipqueue->restart( );
				}
				else
				{
					// Unable to remove this edge. Save it.
					nextflipqueue->newindex( (void **)&parybface );
					*parybface = *bface;
					// Normally, it should be zero. 
					//assert(fc.tetprism_vol_sum == 0.0);
					// However, due to rounding errors, a tiny value may appear.
					fc.tetprism_vol_sum = 0.0;
				}
			}
		} // i

		if(b->verbose > 1)
		{
			myprintf( "    obj (after level %d) = %.17g.\n", autofliplinklevel,
					tetprism_vol_sum );
		}
		flipqueue->restart( );

		// Swap the two flip queues.
		swapqueue = flipqueue;
		flipqueue = nextflipqueue;
		nextflipqueue = swapqueue;

		if(flipqueue->objects > 0l)
		{
			// default 'b->delmaxfliplevel' is 1.
			if(autofliplinklevel >= b->delmaxfliplevel)
			{
				// For efficiency reason, we do not search too far.
				break;
			}
			autofliplinklevel += b->fliplinklevelinc;
		}
	} // while (flipqueue->objects > 0l)

	if(flipqueue->objects > 0l)
	{
		if(b->verbose > 1)
		{
			myprintf( "    %ld non-Delaunay edges remained.\n", flipqueue->objects );
		}
	}

	if(b->verbose)
	{
		myprintf( "  Final obj  = %.17g\n", tetprism_vol_sum );
	}

	b->flipstarsize = bakmaxflipstarsize;
	delete flipqueue;
	delete nextflipqueue;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// gettetrahedron()    Get a tetrahedron which have the given vertices.      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::gettetrahedron( point pa, point pb, point pc, point pd,
								triface *searchtet )
{
	triface spintet;
	int t1ver;

	if(getedge( pa, pb, searchtet ))
	{
		spintet = *searchtet;
		while(1)
		{
			if(apex( spintet ) == pc)
			{
				*searchtet = spintet;
				break;
			}
			fnextself( spintet );
			if(spintet.tet == searchtet->tet) break;
		}
		if(apex( *searchtet ) == pc)
		{
			if(oppo( *searchtet ) == pd)
			{
				return 1;
			}
			else
			{
				fsymself( *searchtet );
				if(oppo( *searchtet ) == pd)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// improvequalitybyflips()    Improve the mesh quality by flips.             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

long tetgenmesh::improvequalitybyflips( )
{
	arraypool *flipqueue, *nextflipqueue, *swapqueue;
	badface *bface, *parybface;
	triface *parytet;
	point *ppt;
	flipconstraints fc;
	REAL *cosdd, ncosdd[ 6 ], maxdd;
	long totalremcount, remcount;
	int remflag;
	int n, i, j, k;

	//assert(unflipqueue->objects > 0l);
	flipqueue = new arraypool( sizeof( badface ), 10 );
	nextflipqueue = new arraypool( sizeof( badface ), 10 );

	// Backup flip edge options.
	int bakautofliplinklevel = autofliplinklevel;
	int bakfliplinklevel = b->fliplinklevel;
	int bakmaxflipstarsize = b->flipstarsize;

	// Set flip edge options.
	autofliplinklevel = 1;
	b->fliplinklevel = -1;
	b->flipstarsize = 10; // b->optmaxflipstarsize;

	fc.remove_large_angle = 1;
	fc.unflip = 1;
	fc.collectnewtets = 1;
	fc.checkflipeligibility = 1;

	totalremcount = 0l;

	// Swap the two flip queues.
	swapqueue = flipqueue;
	flipqueue = unflipqueue;
	unflipqueue = swapqueue;

	while(flipqueue->objects > 0l)
	{

		remcount = 0l;

		while(flipqueue->objects > 0l)
		{
			if(b->verbose > 1)
			{
				myprintf( "    Improving mesh qualiy by flips [%d]#:  %ld.\n",
						autofliplinklevel, flipqueue->objects );
			}

			for(k = 0; k < flipqueue->objects; k++)
			{
				bface = (badface *)fastlookup( flipqueue, k );
				if(gettetrahedron( bface->forg, bface->fdest, bface->fapex,
					bface->foppo, &bface->tt ))
				{
					//assert(!ishulltet(bface->tt));
					// There are bad dihedral angles in this tet.
					if(bface->tt.ver != 11)
					{
						// The dihedral angles are permuted.
						// Here we simply re-compute them. Slow!!.
						ppt = (point *)& ( bface->tt.tet[ 4 ] );
						tetalldihedral( ppt[ 0 ], ppt[ 1 ], ppt[ 2 ], ppt[ 3 ], bface->cent,
										&bface->key, NULL );
						bface->forg = ppt[ 0 ];
						bface->fdest = ppt[ 1 ];
						bface->fapex = ppt[ 2 ];
						bface->foppo = ppt[ 3 ];
						bface->tt.ver = 11;
					}
					if(bface->key == 0)
					{
						// Re-comput the quality values. Due to smoothing operations.
						ppt = (point *)& ( bface->tt.tet[ 4 ] );
						tetalldihedral( ppt[ 0 ], ppt[ 1 ], ppt[ 2 ], ppt[ 3 ], bface->cent,
										&bface->key, NULL );
					}
					cosdd = bface->cent;
					remflag = 0;
					for(i = 0; ( i < 6 ) && !remflag; i++)
					{
						if(cosdd[ i ] < cosmaxdihed)
						{
							// Found a large dihedral angle.
							bface->tt.ver = edge2ver[ i ]; // Go to the edge.
							fc.cosdihed_in = cosdd[ i ];
							fc.cosdihed_out = 0.0; // 90 degree.
							n = removeedgebyflips( &( bface->tt ), &fc );
							if(n == 2)
							{
								// Edge is flipped.
								remflag = 1;
								if(fc.cosdihed_out < cosmaxdihed)
								{
									// Queue new bad tets for further improvements.
									for(j = 0; j < cavetetlist->objects; j++)
									{
										parytet = (triface *)fastlookup( cavetetlist, j );
										if(!isdeadtet( *parytet ))
										{
											ppt = (point *)& ( parytet->tet[ 4 ] );
											// Do not test a hull tet.
											if(ppt[ 3 ] != dummypoint)
											{
												tetalldihedral( ppt[ 0 ], ppt[ 1 ], ppt[ 2 ], ppt[ 3 ], ncosdd,
																&maxdd, NULL );
												if(maxdd < cosmaxdihed)
												{
													// There are bad dihedral angles in this tet.
													nextflipqueue->newindex( (void **)&parybface );
													parybface->tt.tet = parytet->tet;
													parybface->tt.ver = 11;
													parybface->forg = ppt[ 0 ];
													parybface->fdest = ppt[ 1 ];
													parybface->fapex = ppt[ 2 ];
													parybface->foppo = ppt[ 3 ];
													parybface->key = maxdd;
													for(n = 0; n < 6; n++)
													{
														parybface->cent[ n ] = ncosdd[ n ];
													}
												}
											} // if (ppt[3] != dummypoint) 
										}
									} // j
								} // if (fc.cosdihed_out < cosmaxdihed)
								cavetetlist->restart( );
								remcount++;
							}
						}
					} // i          
					if(!remflag)
					{
						// An unremoved bad tet. Queue it again. 
						unflipqueue->newindex( (void **)&parybface );
						*parybface = *bface;
					}
				} // if (gettetrahedron(...))
			} // k

			flipqueue->restart( );

			// Swap the two flip queues.
			swapqueue = flipqueue;
			flipqueue = nextflipqueue;
			nextflipqueue = swapqueue;
		} // while (flipqueues->objects > 0)

		if(b->verbose > 1)
		{
			myprintf( "    Removed %ld bad tets.\n", remcount );
		}
		totalremcount += remcount;

		if(unflipqueue->objects > 0l)
		{
			//if (autofliplinklevel >= b->optmaxfliplevel) {
			if(autofliplinklevel >= b->optlevel)
			{
				break;
			}
			autofliplinklevel += b->fliplinklevelinc;
			//b->flipstarsize = 10 + (1 << (b->optlevel - 1));
		}

		// Swap the two flip queues.
		swapqueue = flipqueue;
		flipqueue = unflipqueue;
		unflipqueue = swapqueue;
	} // while (flipqueues->objects > 0)

	// Restore original flip edge options.
	autofliplinklevel = bakautofliplinklevel;
	b->fliplinklevel = bakfliplinklevel;
	b->flipstarsize = bakmaxflipstarsize;

	delete flipqueue;
	delete nextflipqueue;

	return totalremcount;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// smoothpoint()    Moving a vertex to improve the mesh quality.             //
//                                                                           //
// 'smtpt' (p) is a point to be smoothed. Generally, it is a Steiner point.  //
// It may be not a vertex of the mesh.                                       //
//                                                                           //
// This routine tries to move 'p' inside its star until a selected objective //
// function over all tetrahedra in the star is improved. The function may be //
// the some quality measures, i.e., aspect ratio, maximum dihedral angel, or //
// simply the volume of the tetrahedra.                                      //
//                                                                           //
// 'linkfacelist' contains the list of link faces of 'p'.  Since a link face //
// has two orientations, ccw or cw, with respect to 'p'.  'ccw' indicates    //
// the orientation is ccw (1) or not (0).                                    //
//                                                                           //
// 'opm' is a structure contains the parameters of the objective function.   //
// It is needed by the evaluation of the function value.                     //
//                                                                           //
// The return value indicates weather the point is smoothed or not.          //
//                                                                           //
// ASSUMPTION: This routine assumes that all link faces are true faces, i.e, //
// no face has 'dummypoint' as its vertex.                                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::smoothpoint( point smtpt, arraypool *linkfacelist, int ccw,
							 optparameters *opm )
{
	triface *parytet, *parytet1, swaptet;
	point pa, pb, pc;
	REAL fcent[ 3 ], startpt[ 3 ], nextpt[ 3 ], bestpt[ 3 ];
	REAL oldval, minval = 0.0, val;
	REAL maxcosd; // oldang, newang;
	REAL ori, diff;
	int numdirs, iter;
	int i, j, k;

	// Decide the number of moving directions.
	numdirs = (int)linkfacelist->objects;
	if(numdirs > opm->numofsearchdirs)
	{
		numdirs = opm->numofsearchdirs; // Maximum search directions.
	}

	// Set the initial value.
	opm->imprval = opm->initval;
	iter = 0;

	for(i = 0; i < 3; i++)
	{
		bestpt[ i ] = startpt[ i ] = smtpt[ i ];
	}

	// Iterate until the obj function is not improved.
	while(1)
	{

		// Find the best next location.
		oldval = opm->imprval;

		for(i = 0; i < numdirs; i++)
		{
			// Randomly pick a link face (0 <= k <= objects - i - 1).
			k = (int)randomnation( linkfacelist->objects - i );
			parytet = (triface *)fastlookup( linkfacelist, k );
			// Calculate a new position from 'p' to the center of this face.
			pa = org( *parytet );
			pb = dest( *parytet );
			pc = apex( *parytet );
			for(j = 0; j < 3; j++)
			{
				fcent[ j ] = ( pa[ j ] + pb[ j ] + pc[ j ] ) / 3.0;
			}
			for(j = 0; j < 3; j++)
			{
				nextpt[ j ] = startpt[ j ] + opm->searchstep * ( fcent[ j ] - startpt[ j ] );
			}
			// Calculate the largest minimum function value for the new location.
			for(j = 0; j < linkfacelist->objects; j++)
			{
				parytet = (triface *)fastlookup( linkfacelist, j );
				if(ccw)
				{
					pa = org( *parytet );
					pb = dest( *parytet );
				}
				else
				{
					pb = org( *parytet );
					pa = dest( *parytet );
				}
				pc = apex( *parytet );
				ori = orient3d( pa, pb, pc, nextpt );
				if(ori < 0.0)
				{
					// Calcuate the objective function value. 
					if(opm->max_min_volume)
					{
						//val = -ori;
						val = -orient3dfast( pa, pb, pc, nextpt );
					}
					else if(opm->min_max_aspectratio)
					{
						val = 1.0 / tetaspectratio( pa, pb, pc, nextpt );
					}
					else if(opm->min_max_dihedangle)
					{
						tetalldihedral( pa, pb, pc, nextpt, NULL, &maxcosd, NULL );
						if(maxcosd < -1) maxcosd = -1.0; // Rounding.
						val = maxcosd + 1.0; // Make it be positive. 
					}
					else
					{
						// Unknown objective function.
						val = 0.0;
					}
				}
				else
				{ // ori >= 0.0;
					// An invalid new tet. 
					// This may happen if the mesh contains inverted elements.
					if(opm->max_min_volume)
					{
						//val = -ori;
						val = -orient3dfast( pa, pb, pc, nextpt );
					}
					else
					{
						// Discard this point.
						break; // j
					}
				} // if (ori >= 0.0)
				// Stop looping when the object value is not improved.
				if(val <= opm->imprval)
				{
					break; // j
				}
				else
				{
					// Remember the smallest improved value.
					if(j == 0)
					{
						minval = val;
					}
					else
					{
						minval = ( val < minval ) ? val : minval;
					}
				}
			} // j
			if(j == linkfacelist->objects)
			{
				// The function value has been improved.
				opm->imprval = minval;
				// Save the new location of the point.
				for(j = 0; j < 3; j++) bestpt[ j ] = nextpt[ j ];
			}
			// Swap k-th and (object-i-1)-th entries.
			j = linkfacelist->objects - i - 1;
			parytet = (triface *)fastlookup( linkfacelist, k );
			parytet1 = (triface *)fastlookup( linkfacelist, j );
			swaptet = *parytet1;
			*parytet1 = *parytet;
			*parytet = swaptet;
		} // i

		diff = opm->imprval - oldval;
		if(diff > 0.0)
		{
			// Is the function value improved effectively?
			if(opm->max_min_volume)
			{
				//if ((diff / oldval) < b->epsilon) diff = 0.0;  
			}
			else if(opm->min_max_aspectratio)
			{
				if(( diff / oldval ) < 1e-3) diff = 0.0;
			}
			else if(opm->min_max_dihedangle)
			{
				//oldang = acos(oldval - 1.0);
				//newang = acos(opm->imprval - 1.0);
				//if ((oldang - newang) < 0.00174) diff = 0.0; // about 0.1 degree.
			}
			else
			{
				// Unknown objective function.
				terminatetetgen( this, 2 );
			}
		}

		if(diff > 0.0)
		{
			// Yes, move p to the new location and continue.
			for(j = 0; j < 3; j++) startpt[ j ] = bestpt[ j ];
			iter++;
			if(( opm->maxiter > 0 ) && ( iter >= opm->maxiter ))
			{
				// Maximum smoothing iterations reached.
				break;
			}
		}
		else
		{
			break;
		}

	} // while (1)

	if(iter > 0)
	{
		// The point has been smoothed.
		opm->smthiter = iter; // Remember the number of iterations. 
		// The point has been smoothed. Update it to its new position.
		for(i = 0; i < 3; i++) smtpt[ i ] = startpt[ i ];
	}

	return iter;
}


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// improvequalitysmoothing()    Improve mesh quality by smoothing.           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

long tetgenmesh::improvequalitybysmoothing( optparameters *opm )
{
	arraypool *flipqueue, *swapqueue;
	triface *parytet;
	badface *bface, *parybface;
	point *ppt;
	long totalsmtcount, smtcount;
	int smtflag;
	int iter, i, j, k;

	//assert(unflipqueue->objects > 0l);
	flipqueue = new arraypool( sizeof( badface ), 10 );

	// Swap the two flip queues.
	swapqueue = flipqueue;
	flipqueue = unflipqueue;
	unflipqueue = swapqueue;

	totalsmtcount = 0l;
	iter = 0;

	while(flipqueue->objects > 0l)
	{

		smtcount = 0l;

		if(b->verbose > 1)
		{
			myprintf( "    Improving mesh quality by smoothing [%d]#:  %ld.\n",
					iter, flipqueue->objects );
		}

		for(k = 0; k < flipqueue->objects; k++)
		{
			bface = (badface *)fastlookup( flipqueue, k );
			if(gettetrahedron( bface->forg, bface->fdest, bface->fapex,
				bface->foppo, &bface->tt ))
			{
				// Operate on it if it is not in 'unflipqueue'.
				if(!marktested( bface->tt ))
				{
					// Here we simply re-compute the quality. Since other smoothing
					//   operation may have moved the vertices of this tet.
					ppt = (point *)& ( bface->tt.tet[ 4 ] );
					tetalldihedral( ppt[ 0 ], ppt[ 1 ], ppt[ 2 ], ppt[ 3 ], bface->cent,
									&bface->key, NULL );
					if(bface->key < cossmtdihed)
					{ // if (maxdd < cosslidihed) {
						// It is a sliver. Try to smooth its vertices.
						smtflag = 0;
						opm->initval = bface->key + 1.0;
						for(i = 0; ( i < 4 ) && !smtflag; i++)
						{
							if(pointtype( ppt[ i ] ) == FREEVOLVERTEX)
							{
								getvertexstar( 1, ppt[ i ], cavetetlist, NULL, NULL );
								opm->searchstep = 0.001; // Search step size
								smtflag = smoothpoint( ppt[ i ], cavetetlist, 1, opm );
								if(smtflag)
								{
									while(opm->smthiter == opm->maxiter)
									{
										opm->searchstep *= 10.0; // Increase the step size.
										opm->initval = opm->imprval;
										opm->smthiter = 0; // reset
										smoothpoint( ppt[ i ], cavetetlist, 1, opm );
									}
									// This tet is modifed.
									smtcount++;
									if(( opm->imprval - 1.0 ) < cossmtdihed)
									{
										// There are slivers in new tets. Queue them.
										for(j = 0; j < cavetetlist->objects; j++)
										{
											parytet = (triface *)fastlookup( cavetetlist, j );
											// Operate it if it is not in 'unflipqueue'.
											if(!marktested( *parytet ))
											{
												// Evaluate its quality.
												// Re-use ppt, bface->key, bface->cent.
												ppt = (point *)& ( parytet->tet[ 4 ] );
												tetalldihedral( ppt[ 0 ], ppt[ 1 ], ppt[ 2 ], ppt[ 3 ],
																bface->cent, &bface->key, NULL );
												if(bface->key < cossmtdihed)
												{
													// A new sliver. Queue it.
													marktest( *parytet ); // It is in unflipqueue.
													unflipqueue->newindex( (void **)&parybface );
													parybface->tt = *parytet;
													parybface->forg = ppt[ 0 ];
													parybface->fdest = ppt[ 1 ];
													parybface->fapex = ppt[ 2 ];
													parybface->foppo = ppt[ 3 ];
													parybface->tt.ver = 11;
													parybface->key = 0.0;
												}
											}
										} // j
									} // if ((opm->imprval - 1.0) < cossmtdihed)
								} // if (smtflag)
								cavetetlist->restart( );
							} // if (pointtype(ppt[i]) == FREEVOLVERTEX)
						} // i
						if(!smtflag)
						{
							// Didn't smooth. Queue it again.
							marktest( bface->tt ); // It is in unflipqueue.
							unflipqueue->newindex( (void **)&parybface );
							parybface->tt = bface->tt;
							parybface->forg = ppt[ 0 ];
							parybface->fdest = ppt[ 1 ];
							parybface->fapex = ppt[ 2 ];
							parybface->foppo = ppt[ 3 ];
							parybface->tt.ver = 11;
							parybface->key = 0.0;
						}
					} // if (maxdd < cosslidihed)
				} // if (!marktested(...))
			} // if (gettetrahedron(...))
		} // k

		flipqueue->restart( );

		// Unmark the tets in unflipqueue.
		for(i = 0; i < unflipqueue->objects; i++)
		{
			bface = (badface *)fastlookup( unflipqueue, i );
			unmarktest( bface->tt );
		}

		if(b->verbose > 1)
		{
			myprintf( "    Smooth %ld points.\n", smtcount );
		}
		totalsmtcount += smtcount;

		if(smtcount == 0l)
		{
			// No point has been smoothed. 
			break;
		}
		else
		{
			iter++;
			if(iter == 2)
			{ //if (iter >= b->optpasses) {
				break;
			}
		}

		// Swap the two flip queues.
		swapqueue = flipqueue;
		flipqueue = unflipqueue;
		unflipqueue = swapqueue;
	} // while

	delete flipqueue;

	return totalsmtcount;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// splitsliver()    Split a sliver.                                          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::splitsliver( triface *slitet, REAL cosd, int chkencflag )
{
	triface *abtets;
	triface searchtet, spintet, *parytet;
	point pa, pb, steinerpt;
	optparameters opm;
	insertvertexflags ivf;
	REAL smtpt[ 3 ], midpt[ 3 ];
	int success;
	int t1ver;
	int n, i;

	// 'slitet' is [c,d,a,b], where [c,d] has a big dihedral angle. 
	// Go to the opposite edge [a,b].
	edestoppo( *slitet, searchtet ); // [a,b,c,d].

	// Do not split a segment.
	if(issubseg( searchtet ))
	{
		return 0;
	}

	// Count the number of tets shared at [a,b].
	// Do not split it if it is a hull edge.
	spintet = searchtet;
	n = 0;
	while(1)
	{
		if(ishulltet( spintet )) break;
		n++;
		fnextself( spintet );
		if(spintet.tet == searchtet.tet) break;
	}
	if(ishulltet( spintet ))
	{
		return 0; // It is a hull edge.
	}

	// Get all tets at edge [a,b].
	abtets = new triface[ n ];
	spintet = searchtet;
	for(i = 0; i < n; i++)
	{
		abtets[ i ] = spintet;
		fnextself( spintet );
	}

	// Initialize the list of 2n boundary faces.
	for(i = 0; i < n; i++)
	{
		eprev( abtets[ i ], searchtet );
		esymself( searchtet ); // [a,p_i,p_i+1].
		cavetetlist->newindex( (void **)&parytet );
		*parytet = searchtet;
		enext( abtets[ i ], searchtet );
		esymself( searchtet ); // [p_i,b,p_i+1].
		cavetetlist->newindex( (void **)&parytet );
		*parytet = searchtet;
	}

	// Init the Steiner point at the midpoint of edge [a,b].
	pa = org( abtets[ 0 ] );
	pb = dest( abtets[ 0 ] );
	for(i = 0; i < 3; i++)
	{
		smtpt[ i ] = midpt[ i ] = 0.5 * ( pa[ i ] + pb[ i ] );
	}

	// Point smooth options.
	opm.min_max_dihedangle = 1;
	opm.initval = cosd + 1.0; // Initial volume is zero.
	opm.numofsearchdirs = 20;
	opm.searchstep = 0.001;
	opm.maxiter = 100; // Limit the maximum iterations.

	success = smoothpoint( smtpt, cavetetlist, 1, &opm );

	if(success)
	{
		while(opm.smthiter == opm.maxiter)
		{
			// It was relocated and the prescribed maximum iteration reached. 
			// Try to increase the search stepsize.
			opm.searchstep *= 10.0;
			//opm.maxiter = 100; // Limit the maximum iterations.
			opm.initval = opm.imprval;
			opm.smthiter = 0; // Init.
			smoothpoint( smtpt, cavetetlist, 1, &opm );
		}
	} // if (success)

	cavetetlist->restart( );

	if(!success)
	{
		delete[ ] abtets;
		return 0;
	}


	// Insert the Steiner point.
	makepoint( &steinerpt, FREEVOLVERTEX );
	for(i = 0; i < 3; i++) steinerpt[ i ] = smtpt[ i ];
	// UV: TODO

	// Insert the created Steiner point.
	for(i = 0; i < n; i++)
	{
		infect( abtets[ i ] );
		caveoldtetlist->newindex( (void **)&parytet );
		*parytet = abtets[ i ];
	}

	searchtet = abtets[ 0 ]; // No need point location.
	if(b->metric)
	{
		locate( steinerpt, &searchtet ); // For size interpolation.
	}

	delete[ ] abtets;

	ivf.iloc = (int)INSTAR;
	ivf.chkencflag = chkencflag;
	ivf.assignmeshsize = b->metric;


	if(insertpoint( steinerpt, &searchtet, NULL, NULL, &ivf ))
	{
		// The vertex has been inserted.
		st_volref_count++;
		if(steinerleft > 0) steinerleft--;
		return 1;
	}
	else
	{
		// The Steiner point is too close to an existing vertex. Reject it.
		pointdealloc( steinerpt );
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// removeslivers()    Remove slivers by adding Steiner points.               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

long tetgenmesh::removeslivers( int chkencflag )
{
	arraypool *flipqueue, *swapqueue;
	badface *bface, *parybface;
	triface slitet, *parytet;
	point *ppt;
	REAL cosdd[ 6 ], maxcosd;
	long totalsptcount, sptcount;
	int iter, i, j, k;

	//assert(unflipqueue->objects > 0l);
	flipqueue = new arraypool( sizeof( badface ), 10 );

	// Swap the two flip queues.
	swapqueue = flipqueue;
	flipqueue = unflipqueue;
	unflipqueue = swapqueue;

	totalsptcount = 0l;
	iter = 0;

	while(( flipqueue->objects > 0l ) && ( steinerleft != 0 ))
	{

		sptcount = 0l;

		if(b->verbose > 1)
		{
			myprintf( "    Splitting bad quality tets [%d]#:  %ld.\n",
					iter, flipqueue->objects );
		}

		for(k = 0; ( k < flipqueue->objects ) && ( steinerleft != 0 ); k++)
		{
			bface = (badface *)fastlookup( flipqueue, k );
			if(gettetrahedron( bface->forg, bface->fdest, bface->fapex,
				bface->foppo, &bface->tt ))
			{
				if(( bface->key == 0 ) || ( bface->tt.ver != 11 ))
				{
					// Here we need to re-compute the quality. Since other smoothing
					//   operation may have moved the vertices of this tet.
					ppt = (point *)& ( bface->tt.tet[ 4 ] );
					tetalldihedral( ppt[ 0 ], ppt[ 1 ], ppt[ 2 ], ppt[ 3 ], bface->cent,
									&bface->key, NULL );
				}
				if(bface->key < cosslidihed)
				{
					// It is a sliver. Try to split it.
					slitet.tet = bface->tt.tet;
					//cosdd = bface->cent;
					for(j = 0; j < 6; j++)
					{
						if(bface->cent[ j ] < cosslidihed)
						{
							// Found a large dihedral angle.
							slitet.ver = edge2ver[ j ]; // Go to the edge.
							if(splitsliver( &slitet, bface->cent[ j ], chkencflag ))
							{
								sptcount++;
								break;
							}
						}
					} // j
					if(j < 6)
					{
						// A sliver is split. Queue new slivers.
						badtetrahedrons->traversalinit( );
						parytet = (triface *)badtetrahedrons->traverse( );
						while(parytet != NULL)
						{
							unmarktest2( *parytet );
							ppt = (point *)& ( parytet->tet[ 4 ] );
							tetalldihedral( ppt[ 0 ], ppt[ 1 ], ppt[ 2 ], ppt[ 3 ], cosdd,
											&maxcosd, NULL );
							if(maxcosd < cosslidihed)
							{
								// A new sliver. Queue it.
								unflipqueue->newindex( (void **)&parybface );
								parybface->forg = ppt[ 0 ];
								parybface->fdest = ppt[ 1 ];
								parybface->fapex = ppt[ 2 ];
								parybface->foppo = ppt[ 3 ];
								parybface->tt.tet = parytet->tet;
								parybface->tt.ver = 11;
								parybface->key = maxcosd;
								for(i = 0; i < 6; i++)
								{
									parybface->cent[ i ] = cosdd[ i ];
								}
							}
							parytet = (triface *)badtetrahedrons->traverse( );
						}
						badtetrahedrons->restart( );
					}
					else
					{
						// Didn't split. Queue it again.
						unflipqueue->newindex( (void **)&parybface );
						*parybface = *bface;
					} // if (j == 6)
				} // if (bface->key < cosslidihed)
			} // if (gettetrahedron(...))
		} // k

		flipqueue->restart( );

		if(b->verbose > 1)
		{
			myprintf( "    Split %ld tets.\n", sptcount );
		}
		totalsptcount += sptcount;

		if(sptcount == 0l)
		{
			// No point has been smoothed. 
			break;
		}
		else
		{
			iter++;
			if(iter == 2)
			{ //if (iter >= b->optpasses) {
				break;
			}
		}

		// Swap the two flip queues.
		swapqueue = flipqueue;
		flipqueue = unflipqueue;
		unflipqueue = swapqueue;
	} // while

	delete flipqueue;

	return totalsptcount;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// optimizemesh()    Optimize mesh for specified objective functions.        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::optimizemesh( )
{
	badface *parybface;
	triface checktet;
	point *ppt;
	int optpasses;
	optparameters opm;
	REAL ncosdd[ 6 ], maxdd;
	long totalremcount, remcount;
	long totalsmtcount, smtcount;
	long totalsptcount, sptcount;
	int chkencflag;
	int iter;
	int n;

	if(!b->quiet)
	{
		myprintf( "Optimizing mesh...\n" );
	}

	optpasses = ( ( 1 << b->optlevel ) - 1 );

	if(b->verbose)
	{
		myprintf( "  Optimization level  = %d.\n", b->optlevel );
		myprintf( "  Optimization scheme = %d.\n", b->optscheme );
		myprintf( "  Number of iteration = %d.\n", optpasses );
		myprintf( "  Min_Max dihed angle = %g.\n", b->optmaxdihedral );
	}

	totalsmtcount = totalsptcount = totalremcount = 0l;

	cosmaxdihed = cos( b->optmaxdihedral / 180.0 * PIconstant );
	cossmtdihed = cos( b->optminsmtdihed / 180.0 * PIconstant );
	cosslidihed = cos( b->optminslidihed / 180.0 * PIconstant );

	int attrnum = numelemattrib - 1;

	// Put all bad tetrahedra into array.
	tetrahedrons->traversalinit( );
	checktet.tet = tetrahedrontraverse( );
	while(checktet.tet != NULL)
	{
		if(b->convex)
		{ // -c
			// Skip this tet if it lies in the exterior.
			if(elemattribute( checktet.tet, attrnum ) == -1.0)
			{
				checktet.tet = tetrahedrontraverse( );
				continue;
			}
		}
		ppt = (point *)& ( checktet.tet[ 4 ] );
		tetalldihedral( ppt[ 0 ], ppt[ 1 ], ppt[ 2 ], ppt[ 3 ], ncosdd, &maxdd, NULL );
		if(maxdd < cosmaxdihed)
		{
			// There are bad dihedral angles in this tet.
			unflipqueue->newindex( (void **)&parybface );
			parybface->tt.tet = checktet.tet;
			parybface->tt.ver = 11;
			parybface->forg = ppt[ 0 ];
			parybface->fdest = ppt[ 1 ];
			parybface->fapex = ppt[ 2 ];
			parybface->foppo = ppt[ 3 ];
			parybface->key = maxdd;
			for(n = 0; n < 6; n++)
			{
				parybface->cent[ n ] = ncosdd[ n ];
			}
		}
		checktet.tet = tetrahedrontraverse( );
	}

	totalremcount = improvequalitybyflips( );

	if(( unflipqueue->objects > 0l ) &&
		( ( b->optscheme & 2 ) || ( b->optscheme & 4 ) ))
	{
		// The pool is only used by removeslivers().
		badtetrahedrons = new memorypool( sizeof( triface ), b->tetrahedraperblock,
										  sizeof( void * ), 0 );

		// Smoothing options.
		opm.min_max_dihedangle = 1;
		opm.numofsearchdirs = 10;
		// opm.searchstep = 0.001;  
		opm.maxiter = 30; // Limit the maximum iterations.
		//opm.checkencflag = 4; // Queue affected tets after smoothing.
		chkencflag = 4; // Queue affected tets after splitting a sliver.
		iter = 0;

		while(iter < optpasses)
		{
			smtcount = sptcount = remcount = 0l;
			if(b->optscheme & 2)
			{
				smtcount += improvequalitybysmoothing( &opm );
				totalsmtcount += smtcount;
				if(smtcount > 0l)
				{
					remcount = improvequalitybyflips( );
					totalremcount += remcount;
				}
			}
			if(unflipqueue->objects > 0l)
			{
				if(b->optscheme & 4)
				{
					sptcount += removeslivers( chkencflag );
					totalsptcount += sptcount;
					if(sptcount > 0l)
					{
						remcount = improvequalitybyflips( );
						totalremcount += remcount;
					}
				}
			}
			if(unflipqueue->objects > 0l)
			{
				if(remcount > 0l)
				{
					iter++;
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		} // while (iter)

		delete badtetrahedrons;
		badtetrahedrons = NULL;
	}

	if(unflipqueue->objects > 0l)
	{
		if(b->verbose > 1)
		{
			myprintf( "    %ld bad tets remained.\n", unflipqueue->objects );
		}
		unflipqueue->restart( );
	}

	if(b->verbose)
	{
		if(totalremcount > 0l)
		{
			myprintf( "  Removed %ld edges.\n", totalremcount );
		}
		if(totalsmtcount > 0l)
		{
			myprintf( "  Smoothed %ld points.\n", totalsmtcount );
		}
		if(totalsptcount > 0l)
		{
			myprintf( "  Split %ld slivers.\n", totalsptcount );
		}
	}
}

////                                                                       ////
////                                                                       ////
//// optimize_cxx /////////////////////////////////////////////////////////////

//// meshstat_cxx /////////////////////////////////////////////////////////////
////                                                                       ////
////                                                                       ////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// printfcomma()    Print a (large) number with the 'thousands separator'.   // 
//                                                                           //
// The following code was simply copied from "stackoverflow".                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::printfcomma( unsigned long n )
{
	unsigned long n2 = 0;
	int scale = 1;
	while(n >= 1000)
	{
		n2 = n2 + scale * ( n % 1000 );
		n /= 1000;
		scale *= 1000;
	}
	myprintf( "%ld", n );
	while(scale != 1)
	{
		scale /= 1000;
		n = n2 / scale;
		n2 = n2  % scale;
		myprintf( ",%03ld", n );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// checkmesh()    Test the mesh for topological consistency.                 //
//                                                                           //
// If 'topoflag' is set, only check the topological connection of the mesh,  //
// i.e., do not report degenerated or inverted elements.                     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::checkmesh( int topoflag )
{
	triface tetloop, neightet, symtet;
	point pa, pb, pc, pd;
	REAL ori;
	int horrors, i;

	if(!b->quiet)
	{
		myprintf( "  Checking consistency of mesh...\n" );
	}

	horrors = 0;
	tetloop.ver = 0;
	// Run through the list of tetrahedra, checking each one.
	tetrahedrons->traversalinit( );
	tetloop.tet = alltetrahedrontraverse( );
	while(tetloop.tet != (tetrahedron *)NULL)
	{
		// Check all four faces of the tetrahedron.
		for(tetloop.ver = 0; tetloop.ver < 4; tetloop.ver++)
		{
			pa = org( tetloop );
			pb = dest( tetloop );
			pc = apex( tetloop );
			pd = oppo( tetloop );
			if(tetloop.ver == 0)
			{  // Only test for inversion once.
				if(!ishulltet( tetloop ))
				{  // Only do test if it is not a hull tet.
					if(!topoflag)
					{
						ori = orient3d( pa, pb, pc, pd );
						if(ori >= 0.0)
						{
							myprintf( "  !! !! %s ", ori > 0.0 ? "Inverted" : "Degenerated" );
							myprintf( "  (%d, %d, %d, %d) (ori = %.17g)\n", pointmark( pa ),
									pointmark( pb ), pointmark( pc ), pointmark( pd ), ori );
							horrors++;
						}
					}
				}
				if(infected( tetloop ))
				{
					// This may be a bug. Report it.
					myprintf( "  !! (%d, %d, %d, %d) is infected.\n", pointmark( pa ),
							pointmark( pb ), pointmark( pc ), pointmark( pd ) );
					horrors++;
				}
				if(marktested( tetloop ))
				{
					// This may be a bug. Report it.
					myprintf( "  !! (%d, %d, %d, %d) is marked.\n", pointmark( pa ),
							pointmark( pb ), pointmark( pc ), pointmark( pd ) );
					horrors++;
				}
			}
			if(tetloop.tet[ tetloop.ver ] == NULL)
			{
				myprintf( "  !! !! No neighbor at face (%d, %d, %d).\n", pointmark( pa ),
						pointmark( pb ), pointmark( pc ) );
				horrors++;
			}
			else
			{
				// Find the neighboring tetrahedron on this face.
				fsym( tetloop, neightet );
				if(neightet.tet != NULL)
				{
					// Check that the tetrahedron's neighbor knows it's a neighbor.
					fsym( neightet, symtet );
					if(( tetloop.tet != symtet.tet ) || ( tetloop.ver != symtet.ver ))
					{
						myprintf( "  !! !! Asymmetric tetra-tetra bond:\n" );
						if(tetloop.tet == symtet.tet)
						{
							myprintf( "   (Right tetrahedron, wrong orientation)\n" );
						}
						myprintf( "    First:  (%d, %d, %d, %d)\n", pointmark( pa ),
								pointmark( pb ), pointmark( pc ), pointmark( pd ) );
						myprintf( "    Second: (%d, %d, %d, %d)\n", pointmark( org( neightet ) ),
								pointmark( dest( neightet ) ), pointmark( apex( neightet ) ),
								pointmark( oppo( neightet ) ) );
						horrors++;
					}
					// Check if they have the same edge (the bond() operation).
					if(( org( neightet ) != pb ) || ( dest( neightet ) != pa ))
					{
						myprintf( "  !! !! Wrong edge-edge bond:\n" );
						myprintf( "    First:  (%d, %d, %d, %d)\n", pointmark( pa ),
								pointmark( pb ), pointmark( pc ), pointmark( pd ) );
						myprintf( "    Second: (%d, %d, %d, %d)\n", pointmark( org( neightet ) ),
								pointmark( dest( neightet ) ), pointmark( apex( neightet ) ),
								pointmark( oppo( neightet ) ) );
						horrors++;
					}
					// Check if they have the same apex.
					if(apex( neightet ) != pc)
					{
						myprintf( "  !! !! Wrong face-face bond:\n" );
						myprintf( "    First:  (%d, %d, %d, %d)\n", pointmark( pa ),
								pointmark( pb ), pointmark( pc ), pointmark( pd ) );
						myprintf( "    Second: (%d, %d, %d, %d)\n", pointmark( org( neightet ) ),
								pointmark( dest( neightet ) ), pointmark( apex( neightet ) ),
								pointmark( oppo( neightet ) ) );
						horrors++;
					}
					// Check if they have the same opposite.
					if(oppo( neightet ) == pd)
					{
						myprintf( "  !! !! Two identical tetra:\n" );
						myprintf( "    First:  (%d, %d, %d, %d)\n", pointmark( pa ),
								pointmark( pb ), pointmark( pc ), pointmark( pd ) );
						myprintf( "    Second: (%d, %d, %d, %d)\n", pointmark( org( neightet ) ),
								pointmark( dest( neightet ) ), pointmark( apex( neightet ) ),
								pointmark( oppo( neightet ) ) );
						horrors++;
					}
				}
				else
				{
					myprintf( "  !! !! Tet-face has no neighbor (%d, %d, %d) - %d:\n",
							pointmark( pa ), pointmark( pb ), pointmark( pc ), pointmark( pd ) );
					horrors++;
				}
			}
			if(facemarked( tetloop ))
			{
				// This may be a bug. Report it.
				myprintf( "  !! tetface (%d, %d, %d) %d is marked.\n", pointmark( pa ),
						pointmark( pb ), pointmark( pc ), pointmark( pd ) );
			}
		}
		// Check the six edges of this tet.
		for(i = 0; i < 6; i++)
		{
			tetloop.ver = edge2ver[ i ];
			if(edgemarked( tetloop ))
			{
				// This may be a bug. Report it.
				myprintf( "  !! tetedge (%d, %d) %d, %d is marked.\n",
						pointmark( org( tetloop ) ), pointmark( dest( tetloop ) ),
						pointmark( apex( tetloop ) ), pointmark( oppo( tetloop ) ) );
			}
		}
		tetloop.tet = alltetrahedrontraverse( );
	}
	if(horrors == 0)
	{
		if(!b->quiet)
		{
			myprintf( "  In my studied opinion, the mesh appears to be consistent.\n" );
		}
	}
	else
	{
		myprintf( "  !! !! !! !! %d %s witnessed.\n", horrors,
				horrors > 1 ? "abnormity" : "abnormities" );
	}

	return horrors;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// checkshells()       Test the boundary mesh for topological consistency.   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::checkshells( )
{
	triface neightet, symtet;
	face shloop, spinsh, nextsh;
	face checkseg;
	point pa, pb;
	int bakcount;
	int horrors, i;

	if(!b->quiet)
	{
		myprintf( "  Checking consistency of the mesh boundary...\n" );
	}
	horrors = 0;

	void **bakpathblock = subfaces->pathblock;
	void *bakpathitem = subfaces->pathitem;
	int bakpathitemsleft = subfaces->pathitemsleft;
	int bakalignbytes = subfaces->alignbytes;

	subfaces->traversalinit( );
	shloop.sh = shellfacetraverse( subfaces );
	while(shloop.sh != NULL)
	{
		shloop.shver = 0;
		for(i = 0; i < 3; i++)
		{
			// Check the face ring at this edge.
			pa = sorg( shloop );
			pb = sdest( shloop );
			spinsh = shloop;
			spivot( spinsh, nextsh );
			bakcount = horrors;
			while(( nextsh.sh != NULL ) && ( nextsh.sh != shloop.sh ))
			{
				if(nextsh.sh[ 3 ] == NULL)
				{
					myprintf( "  !! !! Wrong subface-subface connection (Dead subface).\n" );
					myprintf( "    First: x%lx (%d, %d, %d).\n", (uintptr_t)spinsh.sh,
							pointmark( sorg( spinsh ) ), pointmark( sdest( spinsh ) ),
							pointmark( sapex( spinsh ) ) );
					myprintf( "    Second: x%lx (DEAD)\n", (uintptr_t)nextsh.sh );
					horrors++;
					break;
				}
				// check if they have the same edge.
				if(!( ( ( sorg( nextsh ) == pa ) && ( sdest( nextsh ) == pb ) ) ||
					( ( sorg( nextsh ) == pb ) && ( sdest( nextsh ) == pa ) ) ))
				{
					myprintf( "  !! !! Wrong subface-subface connection.\n" );
					myprintf( "    First: x%lx (%d, %d, %d).\n", (uintptr_t)spinsh.sh,
							pointmark( sorg( spinsh ) ), pointmark( sdest( spinsh ) ),
							pointmark( sapex( spinsh ) ) );
					myprintf( "    Scond: x%lx (%d, %d, %d).\n", (uintptr_t)nextsh.sh,
							pointmark( sorg( nextsh ) ), pointmark( sdest( nextsh ) ),
							pointmark( sapex( nextsh ) ) );
					horrors++;
					break;
				}
				// Check they should not have the same apex.
				if(sapex( nextsh ) == sapex( spinsh ))
				{
					myprintf( "  !! !! Existing two duplicated subfaces.\n" );
					myprintf( "    First: x%lx (%d, %d, %d).\n", (uintptr_t)spinsh.sh,
							pointmark( sorg( spinsh ) ), pointmark( sdest( spinsh ) ),
							pointmark( sapex( spinsh ) ) );
					myprintf( "    Scond: x%lx (%d, %d, %d).\n", (uintptr_t)nextsh.sh,
							pointmark( sorg( nextsh ) ), pointmark( sdest( nextsh ) ),
							pointmark( sapex( nextsh ) ) );
					horrors++;
					break;
				}
				spinsh = nextsh;
				spivot( spinsh, nextsh );
			}
			// Check subface-subseg bond.
			sspivot( shloop, checkseg );
			if(checkseg.sh != NULL)
			{
				if(checkseg.sh[ 3 ] == NULL)
				{
					myprintf( "  !! !! Wrong subface-subseg connection (Dead subseg).\n" );
					myprintf( "    Sub: x%lx (%d, %d, %d).\n", (uintptr_t)shloop.sh,
							pointmark( sorg( shloop ) ), pointmark( sdest( shloop ) ),
							pointmark( sapex( shloop ) ) );
					myprintf( "    Sub: x%lx (Dead)\n", (uintptr_t)checkseg.sh );
					horrors++;
				}
				else
				{
					if(!( ( ( sorg( checkseg ) == pa ) && ( sdest( checkseg ) == pb ) ) ||
						( ( sorg( checkseg ) == pb ) && ( sdest( checkseg ) == pa ) ) ))
					{
						myprintf( "  !! !! Wrong subface-subseg connection.\n" );
						myprintf( "    Sub: x%lx (%d, %d, %d).\n", (uintptr_t)shloop.sh,
								pointmark( sorg( shloop ) ), pointmark( sdest( shloop ) ),
								pointmark( sapex( shloop ) ) );
						myprintf( "    Seg: x%lx (%d, %d).\n", (uintptr_t)checkseg.sh,
								pointmark( sorg( checkseg ) ), pointmark( sdest( checkseg ) ) );
						horrors++;
					}
				}
			}
			if(horrors > bakcount) break; // An error detected. 
			senextself( shloop );
		}
		// Check tet-subface connection.
		stpivot( shloop, neightet );
		if(neightet.tet != NULL)
		{
			if(neightet.tet[ 4 ] == NULL)
			{
				myprintf( "  !! !! Wrong sub-to-tet connection (Dead tet)\n" );
				myprintf( "    Sub: x%lx (%d, %d, %d).\n", (uintptr_t)shloop.sh,
						pointmark( sorg( shloop ) ), pointmark( sdest( shloop ) ),
						pointmark( sapex( shloop ) ) );
				myprintf( "    Tet: x%lx (DEAD)\n", (uintptr_t)neightet.tet );
				horrors++;
			}
			else
			{
				if(!( ( sorg( shloop ) == org( neightet ) ) &&
					( sdest( shloop ) == dest( neightet ) ) ))
				{
					myprintf( "  !! !! Wrong sub-to-tet connection\n" );
					myprintf( "    Sub: x%lx (%d, %d, %d).\n", (uintptr_t)shloop.sh,
							pointmark( sorg( shloop ) ), pointmark( sdest( shloop ) ),
							pointmark( sapex( shloop ) ) );
					myprintf( "    Tet: x%lx (%d, %d, %d, %d).\n",
							(uintptr_t)neightet.tet, pointmark( org( neightet ) ),
							pointmark( dest( neightet ) ), pointmark( apex( neightet ) ),
							pointmark( oppo( neightet ) ) );
					horrors++;
				}
				tspivot( neightet, spinsh );
				if(!( ( sorg( spinsh ) == org( neightet ) ) &&
					( sdest( spinsh ) == dest( neightet ) ) ))
				{
					myprintf( "  !! !! Wrong tet-sub connection.\n" );
					myprintf( "    Sub: x%lx (%d, %d, %d).\n", (uintptr_t)spinsh.sh,
							pointmark( sorg( spinsh ) ), pointmark( sdest( spinsh ) ),
							pointmark( sapex( spinsh ) ) );
					myprintf( "    Tet: x%lx (%d, %d, %d, %d).\n",
							(uintptr_t)neightet.tet, pointmark( org( neightet ) ),
							pointmark( dest( neightet ) ), pointmark( apex( neightet ) ),
							pointmark( oppo( neightet ) ) );
					horrors++;
				}
				fsym( neightet, symtet );
				tspivot( symtet, spinsh );
				if(spinsh.sh != NULL)
				{
					if(!( ( sorg( spinsh ) == org( symtet ) ) &&
						( sdest( spinsh ) == dest( symtet ) ) ))
					{
						myprintf( "  !! !! Wrong tet-sub connection.\n" );
						myprintf( "    Sub: x%lx (%d, %d, %d).\n", (uintptr_t)spinsh.sh,
								pointmark( sorg( spinsh ) ), pointmark( sdest( spinsh ) ),
								pointmark( sapex( spinsh ) ) );
						myprintf( "    Tet: x%lx (%d, %d, %d, %d).\n",
								(uintptr_t)symtet.tet, pointmark( org( symtet ) ),
								pointmark( dest( symtet ) ), pointmark( apex( symtet ) ),
								pointmark( oppo( symtet ) ) );
						horrors++;
					}
				}
				else
				{
					myprintf( "  Warning: Broken tet-sub-tet connection.\n" );
				}
			}
		}
		if(sinfected( shloop ))
		{
			// This may be a bug. report it.
			myprintf( "  !! A infected subface: (%d, %d, %d).\n",
					pointmark( sorg( shloop ) ), pointmark( sdest( shloop ) ),
					pointmark( sapex( shloop ) ) );
		}
		if(smarktested( shloop ))
		{
			// This may be a bug. report it.
			myprintf( "  !! A marked subface: (%d, %d, %d).\n", pointmark( sorg( shloop ) ),
					pointmark( sdest( shloop ) ), pointmark( sapex( shloop ) ) );
		}
		shloop.sh = shellfacetraverse( subfaces );
	}

	if(horrors == 0)
	{
		if(!b->quiet)
		{
			myprintf( "  Mesh boundaries connected correctly.\n" );
		}
	}
	else
	{
		myprintf( "  !! !! !! !! %d boundary connection viewed with horror.\n",
				horrors );
	}

	subfaces->pathblock = bakpathblock;
	subfaces->pathitem = bakpathitem;
	subfaces->pathitemsleft = bakpathitemsleft;
	subfaces->alignbytes = bakalignbytes;

	return horrors;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// checksegments()    Check the connections between tetrahedra and segments. //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::checksegments( )
{
	triface tetloop, neightet, spintet;
	shellface *segs;
	face neighsh, spinsh, checksh;
	face sseg, checkseg;
	point pa, pb;
	int miscount;
	int t1ver;
	int horrors, i;


	if(!b->quiet)
	{
		myprintf( "  Checking tet->seg connections...\n" );
	}

	horrors = 0;
	tetrahedrons->traversalinit( );
	tetloop.tet = tetrahedrontraverse( );
	while(tetloop.tet != NULL)
	{
		// Loop the six edges of the tet.
		if(tetloop.tet[ 8 ] != NULL)
		{
			segs = (shellface *)tetloop.tet[ 8 ];
			for(i = 0; i < 6; i++)
			{
				sdecode( segs[ i ], sseg );
				if(sseg.sh != NULL)
				{
					// Get the edge of the tet.
					tetloop.ver = edge2ver[ i ];
					// Check if they are the same edge.
					pa = (point)sseg.sh[ 3 ];
					pb = (point)sseg.sh[ 4 ];
					if(!( ( ( org( tetloop ) == pa ) && ( dest( tetloop ) == pb ) ) ||
						( ( org( tetloop ) == pb ) && ( dest( tetloop ) == pa ) ) ))
					{
						myprintf( "  !! Wrong tet-seg connection.\n" );
						myprintf( "    Tet: x%lx (%d, %d, %d, %d) - Seg: x%lx (%d, %d).\n",
								(uintptr_t)tetloop.tet, pointmark( org( tetloop ) ),
								pointmark( dest( tetloop ) ), pointmark( apex( tetloop ) ),
								pointmark( oppo( tetloop ) ), (uintptr_t)sseg.sh,
								pointmark( pa ), pointmark( pb ) );
						horrors++;
					}
					else
					{
						// Loop all tets sharing at this edge.
						neightet = tetloop;
						do
						{
							tsspivot1( neightet, checkseg );
							if(checkseg.sh != sseg.sh)
							{
								myprintf( "  !! Wrong tet->seg connection.\n" );
								myprintf( "    Tet: x%lx (%d, %d, %d, %d) - ",
										(uintptr_t)neightet.tet, pointmark( org( neightet ) ),
										pointmark( dest( neightet ) ), pointmark( apex( neightet ) ),
										pointmark( oppo( neightet ) ) );
								if(checkseg.sh != NULL)
								{
									myprintf( "Seg x%lx (%d, %d).\n", (uintptr_t)checkseg.sh,
											pointmark( sorg( checkseg ) ), pointmark( sdest( checkseg ) ) );
								}
								else
								{
									myprintf( "Seg: NULL.\n" );
								}
								horrors++;
							}
							fnextself( neightet );
						}
						while(neightet.tet != tetloop.tet);
					}
					// Check the seg->tet pointer.
					sstpivot1( sseg, neightet );
					if(neightet.tet == NULL)
					{
						myprintf( "  !! Wrong seg->tet connection (A NULL tet).\n" );
						horrors++;
					}
					else
					{
						if(!( ( ( org( neightet ) == pa ) && ( dest( neightet ) == pb ) ) ||
							( ( org( neightet ) == pb ) && ( dest( neightet ) == pa ) ) ))
						{
							myprintf( "  !! Wrong seg->tet connection (Wrong edge).\n" );
							myprintf( "    Tet: x%lx (%d, %d, %d, %d) - Seg: x%lx (%d, %d).\n",
									(uintptr_t)neightet.tet, pointmark( org( neightet ) ),
									pointmark( dest( neightet ) ), pointmark( apex( neightet ) ),
									pointmark( oppo( neightet ) ), (uintptr_t)sseg.sh,
									pointmark( pa ), pointmark( pb ) );
							horrors++;
						}
					}
				}
			}
		}
		// Loop the six edge of this tet.
		neightet.tet = tetloop.tet;
		for(i = 0; i < 6; i++)
		{
			neightet.ver = edge2ver[ i ];
			if(edgemarked( neightet ))
			{
				// A possible bug. Report it.
				myprintf( "  !! A marked edge: (%d, %d, %d, %d) -- x%lx %d.\n",
						pointmark( org( neightet ) ), pointmark( dest( neightet ) ),
						pointmark( apex( neightet ) ), pointmark( oppo( neightet ) ),
						(uintptr_t)neightet.tet, neightet.ver );
				// Check if all tets at the edge are marked.
				spintet = neightet;
				while(1)
				{
					fnextself( spintet );
					if(!edgemarked( spintet ))
					{
						myprintf( "  !! !! An unmarked edge (%d, %d, %d, %d) -- x%lx %d.\n",
								pointmark( org( spintet ) ), pointmark( dest( spintet ) ),
								pointmark( apex( spintet ) ), pointmark( oppo( spintet ) ),
								(uintptr_t)spintet.tet, spintet.ver );
						horrors++;
					}
					if(spintet.tet == neightet.tet) break;
				}
			}
		}
		tetloop.tet = tetrahedrontraverse( );
	}

	if(!b->quiet)
	{
		myprintf( "  Checking seg->tet connections...\n" );
	}

	miscount = 0; // Count the number of unrecovered segments.
	subsegs->traversalinit( );
	sseg.shver = 0;
	sseg.sh = shellfacetraverse( subsegs );
	while(sseg.sh != NULL)
	{
		pa = sorg( sseg );
		pb = sdest( sseg );
		spivot( sseg, neighsh );
		if(neighsh.sh != NULL)
		{
			spinsh = neighsh;
			while(1)
			{
				// Check seg-subface bond.
				if(( ( sorg( spinsh ) == pa ) && ( sdest( spinsh ) == pb ) ) ||
					( ( sorg( spinsh ) == pb ) && ( sdest( spinsh ) == pa ) ))
				{
					// Keep the same rotate direction.
					//if (sorg(spinsh) != pa) {          
					//  sesymself(spinsh);
					//  myprintf("  !! Wrong ori at subface (%d, %d, %d) -- x%lx %d\n",
					//         pointmark(sorg(spinsh)), pointmark(sdest(spinsh)),
					//         pointmark(sapex(spinsh)), (uintptr_t) spinsh.sh,
					//         spinsh.shver);
					//  horrors++;
					//}
					stpivot( spinsh, spintet );
					if(spintet.tet != NULL)
					{
						// Check if all tets at this segment.
						while(1)
						{
							tsspivot1( spintet, checkseg );
							if(checkseg.sh == NULL)
							{
								myprintf( "  !! !! No seg at tet (%d, %d, %d, %d) -- x%lx %d\n",
										pointmark( org( spintet ) ), pointmark( dest( spintet ) ),
										pointmark( apex( spintet ) ), pointmark( oppo( spintet ) ),
										(uintptr_t)spintet.tet, spintet.ver );
								horrors++;
							}
							if(checkseg.sh != sseg.sh)
							{
								myprintf( "  !! !! Wrong seg (%d, %d) at tet (%d, %d, %d, %d)\n",
										pointmark( sorg( checkseg ) ), pointmark( sdest( checkseg ) ),
										pointmark( org( spintet ) ), pointmark( dest( spintet ) ),
										pointmark( apex( spintet ) ), pointmark( oppo( spintet ) ) );
								horrors++;
							}
							fnextself( spintet );
							// Stop at the next subface.
							tspivot( spintet, checksh );
							if(checksh.sh != NULL) break;
						} // while (1)
					}
				}
				else
				{
					myprintf( "  !! Wrong seg-subface (%d, %d, %d) -- x%lx %d connect\n",
							pointmark( sorg( spinsh ) ), pointmark( sdest( spinsh ) ),
							pointmark( sapex( spinsh ) ), (uintptr_t)spinsh.sh,
							spinsh.shver );
					horrors++;
					break;
				} // if pa, pb
				spivotself( spinsh );
				if(spinsh.sh == NULL) break; // A dangling segment.
				if(spinsh.sh == neighsh.sh) break;
			} // while (1)
		} // if (neighsh.sh != NULL)
		// Count the number of "un-recovered" segments.
		sstpivot1( sseg, neightet );
		if(neightet.tet == NULL)
		{
			miscount++;
		}
		sseg.sh = shellfacetraverse( subsegs );
	}

	if(!b->quiet)
	{
		myprintf( "  Checking seg->seg connections...\n" );
	}

	points->traversalinit( );
	pa = pointtraverse( );
	while(pa != NULL)
	{
		if(pointtype( pa ) == FREESEGVERTEX)
		{
			// There should be two subsegments connected at 'pa'.
			// Get a subsegment containing 'pa'.
			sdecode( point2sh( pa ), sseg );
			if(( sseg.sh == NULL ) || sseg.sh[ 3 ] == NULL)
			{
				myprintf( "  !! Dead point-to-seg pointer at point %d.\n",
						pointmark( pa ) );
				horrors++;
			}
			else
			{
				sseg.shver = 0;
				if(sorg( sseg ) != pa)
				{
					if(sdest( sseg ) != pa)
					{
						myprintf( "  !! Wrong point-to-seg pointer at point %d.\n",
								pointmark( pa ) );
						horrors++;
					}
					else
					{
						// Find the next subsegment at 'pa'.
						senext( sseg, checkseg );
						if(( checkseg.sh == NULL ) || ( checkseg.sh[ 3 ] == NULL ))
						{
							myprintf( "  !! Dead seg-seg connection at point %d.\n",
									pointmark( pa ) );
							horrors++;
						}
						else
						{
							spivotself( checkseg );
							checkseg.shver = 0;
							if(sorg( checkseg ) != pa)
							{
								myprintf( "  !! Wrong seg-seg connection at point %d.\n",
										pointmark( pa ) );
								horrors++;
							}
						}
					}
				}
				else
				{
					// Find the previous subsegment at 'pa'.
					senext2( sseg, checkseg );
					if(( checkseg.sh == NULL ) || ( checkseg.sh[ 3 ] == NULL ))
					{
						myprintf( "  !! Dead seg-seg connection at point %d.\n",
								pointmark( pa ) );
						horrors++;
					}
					else
					{
						spivotself( checkseg );
						checkseg.shver = 0;
						if(sdest( checkseg ) != pa)
						{
							myprintf( "  !! Wrong seg-seg connection at point %d.\n",
									pointmark( pa ) );
							horrors++;
						}
					}
				}
			}
		}
		pa = pointtraverse( );
	}

	if(horrors == 0)
	{
		myprintf( "  Segments are connected properly.\n" );
	}
	else
	{
		myprintf( "  !! !! !! !! Found %d missing connections.\n", horrors );
	}
	if(miscount > 0)
	{
		myprintf( "  !! !! Found %d missing segments.\n", miscount );
	}

	return horrors;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// checkdelaunay()    Ensure that the mesh is (constrained) Delaunay.        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::checkdelaunay( int perturb )
{
	triface tetloop;
	triface symtet;
	face checksh;
	point pa, pb, pc, pd, pe;
	REAL sign;
	int ndcount; // Count the non-locally Delaunay faces.
	int horrors;

	if(!b->quiet)
	{
		myprintf( "  Checking Delaunay property of the mesh...\n" );
	}

	ndcount = 0;
	horrors = 0;
	tetloop.ver = 0;
	// Run through the list of triangles, checking each one.
	tetrahedrons->traversalinit( );
	tetloop.tet = tetrahedrontraverse( );
	while(tetloop.tet != (tetrahedron *)NULL)
	{
		// Check all four faces of the tetrahedron.
		for(tetloop.ver = 0; tetloop.ver < 4; tetloop.ver++)
		{
			fsym( tetloop, symtet );
			// Only do test if its adjoining tet is not a hull tet or its pointer
			//   is larger (to ensure that each pair isn't tested twice).
			if(( (point)symtet.tet[ 7 ] != dummypoint ) && ( tetloop.tet < symtet.tet ))
			{
				pa = org( tetloop );
				pb = dest( tetloop );
				pc = apex( tetloop );
				pd = oppo( tetloop );
				pe = oppo( symtet );
				if(perturb)
				{
					sign = insphere_s( pa, pb, pc, pd, pe );
				}
				else
				{
					sign = insphere( pa, pb, pc, pd, pe );
				}
				if(sign < 0.0)
				{
					ndcount++;
					if(checksubfaceflag)
					{
						tspivot( tetloop, checksh );
					}
					if(checksh.sh == NULL)
					{
						myprintf( "  !! Non-locally Delaunay (%d, %d, %d) - %d, %d\n",
								pointmark( pa ), pointmark( pb ), pointmark( pc ), pointmark( pd ),
								pointmark( pe ) );
						horrors++;
					}
				}
			}
		}
		tetloop.tet = tetrahedrontraverse( );
	}

	if(horrors == 0)
	{
		if(!b->quiet)
		{
			if(ndcount > 0)
			{
				myprintf( "  The mesh is constrained Delaunay.\n" );
			}
			else
			{
				myprintf( "  The mesh is Delaunay.\n" );
			}
		}
	}
	else
	{
		myprintf( "  !! !! !! !! Found %d non-Delaunay faces.\n", horrors );
	}

	return horrors;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Check if the current tetrahedralization is (constrained) regular.         //
//                                                                           //
// The parameter 'type' determines which regularity should be checked:       //
//   - 0:  check the Delaunay property.                                      //
//   - 1:  check the Delaunay property with symbolic perturbation.           //
//   - 2:  check the regular property, the weights are stored in p[3].       //
//   - 3:  check the regular property with symbolic perturbation.            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::checkregular( int type )
{
	triface tetloop;
	triface symtet;
	face checksh;
	point p[ 5 ];
	REAL sign;
	int ndcount; // Count the non-locally Delaunay faces.
	int horrors;

	if(!b->quiet)
	{
		myprintf( "  Checking %s %s property of the mesh...\n",
				( type & 2 ) == 0 ? "Delaunay" : "regular",
				( type & 1 ) == 0 ? " " : "(s)" );
	}

	// Make sure orient3d(p[1], p[0], p[2], p[3]) > 0;
	//   Hence if (insphere(p[1], p[0], p[2], p[3], p[4]) > 0) means that
	//     p[4] lies inside the circumsphere of p[1], p[0], p[2], p[3].
	//   The same if orient4d(p[1], p[0], p[2], p[3], p[4]) > 0 means that
	//     p[4] lies below the oriented hyperplane passing through 
	//     p[1], p[0], p[2], p[3].

	ndcount = 0;
	horrors = 0;
	tetloop.ver = 0;
	// Run through the list of triangles, checking each one.
	tetrahedrons->traversalinit( );
	tetloop.tet = tetrahedrontraverse( );
	while(tetloop.tet != (tetrahedron *)NULL)
	{
		// Check all four faces of the tetrahedron.
		for(tetloop.ver = 0; tetloop.ver < 4; tetloop.ver++)
		{
			fsym( tetloop, symtet );
			// Only do test if its adjoining tet is not a hull tet or its pointer
			//   is larger (to ensure that each pair isn't tested twice).
			if(( (point)symtet.tet[ 7 ] != dummypoint ) && ( tetloop.tet < symtet.tet ))
			{
				p[ 0 ] = org( tetloop );   // pa
				p[ 1 ] = dest( tetloop );  // pb
				p[ 2 ] = apex( tetloop );  // pc
				p[ 3 ] = oppo( tetloop );  // pd
				p[ 4 ] = oppo( symtet );   // pe

				if(type == 0)
				{
					sign = insphere( p[ 1 ], p[ 0 ], p[ 2 ], p[ 3 ], p[ 4 ] );
				}
				else if(type == 1)
				{
					sign = insphere_s( p[ 1 ], p[ 0 ], p[ 2 ], p[ 3 ], p[ 4 ] );
				}
				else if(type == 2)
				{
					sign = orient4d( p[ 1 ], p[ 0 ], p[ 2 ], p[ 3 ], p[ 4 ],
									 p[ 1 ][ 3 ], p[ 0 ][ 3 ], p[ 2 ][ 3 ], p[ 3 ][ 3 ], p[ 4 ][ 3 ] );
				}
				else
				{ // type == 3
					sign = orient4d_s( p[ 1 ], p[ 0 ], p[ 2 ], p[ 3 ], p[ 4 ],
									   p[ 1 ][ 3 ], p[ 0 ][ 3 ], p[ 2 ][ 3 ], p[ 3 ][ 3 ], p[ 4 ][ 3 ] );
				}

				if(sign > 0.0)
				{
					ndcount++;
					if(checksubfaceflag)
					{
						tspivot( tetloop, checksh );
					}
					if(checksh.sh == NULL)
					{
						myprintf( "  !! Non-locally %s (%d, %d, %d) - %d, %d\n",
								( type & 2 ) == 0 ? "Delaunay" : "regular",
								pointmark( p[ 0 ] ), pointmark( p[ 1 ] ), pointmark( p[ 2 ] ),
								pointmark( p[ 3 ] ), pointmark( p[ 4 ] ) );
						horrors++;
					}
				}
			}
		}
		tetloop.tet = tetrahedrontraverse( );
	}

	if(horrors == 0)
	{
		if(!b->quiet)
		{
			if(ndcount > 0)
			{
				myprintf( "  The mesh is constrained %s.\n",
						( type & 2 ) == 0 ? "Delaunay" : "regular" );
			}
			else
			{
				myprintf( "  The mesh is %s.\n", ( type & 2 ) == 0 ? "Delaunay" : "regular" );
			}
		}
	}
	else
	{
		myprintf( "  !! !! !! !! Found %d non-%s faces.\n", horrors,
				( type & 2 ) == 0 ? "Delaunay" : "regular" );
	}

	return horrors;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// checkconforming()    Ensure that the mesh is conforming Delaunay.         //
//                                                                           //
// If 'flag' is 1, only check subsegments. If 'flag' is 2, check subfaces.   //
// If 'flag' is 3, check both subsegments and subfaces.                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int tetgenmesh::checkconforming( int flag )
{
	triface searchtet, neightet, spintet;
	face shloop;
	face segloop;
	point eorg, edest, eapex, pa, pb, pc;
	REAL cent[ 3 ], radius, dist, diff, rd, len;
	bool enq;
	int encsubsegs, encsubfaces;
	int t1ver;
	int i;

	REAL A[ 4 ][ 4 ], rhs[ 4 ], D;
	int indx[ 4 ];
	REAL elen[ 3 ];

	encsubsegs = 0;

	if(flag & 1)
	{
		if(!b->quiet)
		{
			myprintf( "  Checking conforming property of segments...\n" );
		}
		encsubsegs = 0;

		// Run through the list of subsegments, check each one.
		subsegs->traversalinit( );
		segloop.sh = shellfacetraverse( subsegs );
		while(segloop.sh != (shellface *)NULL)
		{
			eorg = (point)segloop.sh[ 3 ];
			edest = (point)segloop.sh[ 4 ];
			radius = 0.5 * distance( eorg, edest );
			for(i = 0; i < 3; i++) cent[ i ] = 0.5 * ( eorg[ i ] + edest[ i ] );

			enq = false;
			sstpivot1( segloop, neightet );
			if(neightet.tet != NULL)
			{
				spintet = neightet;
				while(1)
				{
					eapex = apex( spintet );
					if(eapex != dummypoint)
					{
						dist = distance( eapex, cent );
						diff = dist - radius;
						if(fabs( diff ) / radius <= b->epsilon) diff = 0.0; // Rounding.
						if(diff < 0)
						{
							enq = true; break;
						}
					}
					fnextself( spintet );
					if(spintet.tet == neightet.tet) break;
				}
			}
			if(enq)
			{
				myprintf( "  !! !! Non-conforming segment: (%d, %d)\n",
						pointmark( eorg ), pointmark( edest ) );
				encsubsegs++;
			}
			segloop.sh = shellfacetraverse( subsegs );
		}

		if(encsubsegs == 0)
		{
			if(!b->quiet)
			{
				myprintf( "  The segments are conforming Delaunay.\n" );
			}
		}
		else
		{
			myprintf( "  !! !! %d subsegments are non-conforming.\n", encsubsegs );
		}
	} // if (flag & 1)

	encsubfaces = 0;

	if(flag & 2)
	{
		if(!b->quiet)
		{
			myprintf( "  Checking conforming property of subfaces...\n" );
		}

		// Run through the list of subfaces, check each one.
		subfaces->traversalinit( );
		shloop.sh = shellfacetraverse( subfaces );
		while(shloop.sh != (shellface *)NULL)
		{
			pa = (point)shloop.sh[ 3 ];
			pb = (point)shloop.sh[ 4 ];
			pc = (point)shloop.sh[ 5 ];

			// Compute the coefficient matrix A (3x3).
			A[ 0 ][ 0 ] = pb[ 0 ] - pa[ 0 ];
			A[ 0 ][ 1 ] = pb[ 1 ] - pa[ 1 ];
			A[ 0 ][ 2 ] = pb[ 2 ] - pa[ 2 ]; // vector V1 (pa->pb)
			A[ 1 ][ 0 ] = pc[ 0 ] - pa[ 0 ];
			A[ 1 ][ 1 ] = pc[ 1 ] - pa[ 1 ];
			A[ 1 ][ 2 ] = pc[ 2 ] - pa[ 2 ]; // vector V2 (pa->pc)
			cross( A[ 0 ], A[ 1 ], A[ 2 ] ); // vector V3 (V1 X V2)

			// Compute the right hand side vector b (3x1).
			elen[ 0 ] = dot( A[ 0 ], A[ 0 ] );
			elen[ 1 ] = dot( A[ 1 ], A[ 1 ] );
			rhs[ 0 ] = 0.5 * elen[ 0 ];
			rhs[ 1 ] = 0.5 * elen[ 1 ];
			rhs[ 2 ] = 0.0;

			if(lu_decmp( A, 3, indx, &D, 0 ))
			{
				lu_solve( A, 3, indx, rhs, 0 );
				cent[ 0 ] = pa[ 0 ] + rhs[ 0 ];
				cent[ 1 ] = pa[ 1 ] + rhs[ 1 ];
				cent[ 2 ] = pa[ 2 ] + rhs[ 2 ];
				rd = sqrt( rhs[ 0 ] * rhs[ 0 ] + rhs[ 1 ] * rhs[ 1 ] + rhs[ 2 ] * rhs[ 2 ] );

				// Check if this subface is encroached.
				for(i = 0; i < 2; i++)
				{
					stpivot( shloop, searchtet );
					if(!ishulltet( searchtet ))
					{
						len = distance( oppo( searchtet ), cent );
						if(( fabs( len - rd ) / rd ) < b->epsilon) len = rd; // Rounding.
						if(len < rd)
						{
							myprintf( "  !! !! Non-conforming subface: (%d, %d, %d)\n",
									pointmark( pa ), pointmark( pb ), pointmark( pc ) );
							encsubfaces++;
							enq = true; break;
						}
					}
					sesymself( shloop );
				}
			}
			shloop.sh = shellfacetraverse( subfaces );
		}

		if(encsubfaces == 0)
		{
			if(!b->quiet)
			{
				myprintf( "  The subfaces are conforming Delaunay.\n" );
			}
		}
		else
		{
			myprintf( "  !! !! %d subfaces are non-conforming.\n", encsubfaces );
		}
	} // if (flag & 2)

	return encsubsegs + encsubfaces;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// qualitystatistics()    Print statistics about the quality of the mesh.    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::qualitystatistics( )
{
	triface tetloop, neightet;
	point p[ 4 ];
	char sbuf[ 128 ];
	REAL radiusratiotable[ 12 ];
	REAL aspectratiotable[ 12 ];
	REAL A[ 4 ][ 4 ], rhs[ 4 ], D;
	REAL V[ 6 ][ 3 ], N[ 4 ][ 3 ], H[ 4 ]; // edge-vectors, face-normals, face-heights.
	REAL edgelength[ 6 ], alldihed[ 6 ], faceangle[ 3 ];
	REAL shortest, longest;
	REAL smallestvolume, biggestvolume;
	REAL smallestratio, biggestratio;
	REAL smallestdiangle, biggestdiangle;
	REAL smallestfaangle, biggestfaangle;
	REAL total_tet_vol, total_tetprism_vol;
	REAL tetvol, minaltitude;
	REAL cirradius, minheightinv; // insradius;
	REAL shortlen, longlen;
	REAL tetaspect, tetradius;
	REAL smalldiangle, bigdiangle;
	REAL smallfaangle, bigfaangle;
	unsigned long radiustable[ 12 ];
	unsigned long aspecttable[ 16 ];
	unsigned long dihedangletable[ 18 ];
	unsigned long faceangletable[ 18 ];
	int indx[ 4 ];
	int radiusindex;
	int aspectindex;
	int tendegree;
	int i, j;

	myprintf( "Mesh quality statistics:\n\n" );

	shortlen = longlen = 0.0;
	smalldiangle = bigdiangle = 0.0;
	total_tet_vol = 0.0;
	total_tetprism_vol = 0.0;

	radiusratiotable[ 0 ] = 0.707;    radiusratiotable[ 1 ] = 1.0;
	radiusratiotable[ 2 ] = 1.1;    radiusratiotable[ 3 ] = 1.2;
	radiusratiotable[ 4 ] = 1.4;    radiusratiotable[ 5 ] = 1.6;
	radiusratiotable[ 6 ] = 1.8;    radiusratiotable[ 7 ] = 2.0;
	radiusratiotable[ 8 ] = 2.5;    radiusratiotable[ 9 ] = 3.0;
	radiusratiotable[ 10 ] = 10.0;    radiusratiotable[ 11 ] = 0.0;

	aspectratiotable[ 0 ] = 1.5;    aspectratiotable[ 1 ] = 2.0;
	aspectratiotable[ 2 ] = 2.5;    aspectratiotable[ 3 ] = 3.0;
	aspectratiotable[ 4 ] = 4.0;    aspectratiotable[ 5 ] = 6.0;
	aspectratiotable[ 6 ] = 10.0;    aspectratiotable[ 7 ] = 15.0;
	aspectratiotable[ 8 ] = 25.0;    aspectratiotable[ 9 ] = 50.0;
	aspectratiotable[ 10 ] = 100.0;    aspectratiotable[ 11 ] = 0.0;

	for(i = 0; i < 12; i++) radiustable[ i ] = 0l;
	for(i = 0; i < 12; i++) aspecttable[ i ] = 0l;
	for(i = 0; i < 18; i++) dihedangletable[ i ] = 0l;
	for(i = 0; i < 18; i++) faceangletable[ i ] = 0l;

	minaltitude = xmax - xmin + ymax - ymin + zmax - zmin;
	minaltitude = minaltitude * minaltitude;
	shortest = minaltitude;
	longest = 0.0;
	smallestvolume = minaltitude;
	biggestvolume = 0.0;
	smallestratio = 1e+16; // minaltitude;
	biggestratio = 0.0;
	smallestdiangle = smallestfaangle = 180.0;
	biggestdiangle = biggestfaangle = 0.0;


	int attrnum = numelemattrib - 1;

	// Loop all elements, calculate quality parameters for each element.
	tetrahedrons->traversalinit( );
	tetloop.tet = tetrahedrontraverse( );
	while(tetloop.tet != (tetrahedron *)NULL)
	{

		if(b->convex)
		{
			// Skip tets in the exterior.
			if(elemattribute( tetloop.tet, attrnum ) == -1.0)
			{
				tetloop.tet = tetrahedrontraverse( );
				continue;
			}
		}

		// Get four vertices: p0, p1, p2, p3.
		for(i = 0; i < 4; i++) p[ i ] = (point)tetloop.tet[ 4 + i ];

		// Get the tet volume.
		tetvol = orient3dfast( p[ 1 ], p[ 0 ], p[ 2 ], p[ 3 ] ) / 6.0;
		total_tet_vol += tetvol;
		total_tetprism_vol += tetprismvol( p[ 0 ], p[ 1 ], p[ 2 ], p[ 3 ] );

		// Calculate the largest and smallest volume.
		if(tetvol < smallestvolume)
		{
			smallestvolume = tetvol;
		}
		if(tetvol > biggestvolume)
		{
			biggestvolume = tetvol;
		}

		// Set the edge vectors: V[0], ..., V[5]
		for(i = 0; i < 3; i++) V[ 0 ][ i ] = p[ 0 ][ i ] - p[ 3 ][ i ]; // V[0]: p3->p0.
		for(i = 0; i < 3; i++) V[ 1 ][ i ] = p[ 1 ][ i ] - p[ 3 ][ i ]; // V[1]: p3->p1.
		for(i = 0; i < 3; i++) V[ 2 ][ i ] = p[ 2 ][ i ] - p[ 3 ][ i ]; // V[2]: p3->p2.
		for(i = 0; i < 3; i++) V[ 3 ][ i ] = p[ 1 ][ i ] - p[ 0 ][ i ]; // V[3]: p0->p1.
		for(i = 0; i < 3; i++) V[ 4 ][ i ] = p[ 2 ][ i ] - p[ 1 ][ i ]; // V[4]: p1->p2.
		for(i = 0; i < 3; i++) V[ 5 ][ i ] = p[ 0 ][ i ] - p[ 2 ][ i ]; // V[5]: p2->p0.

		// Get the squares of the edge lengths.
		for(i = 0; i < 6; i++) edgelength[ i ] = dot( V[ i ], V[ i ] );

		// Calculate the longest and shortest edge length.
		for(i = 0; i < 6; i++)
		{
			if(i == 0)
			{
				shortlen = longlen = edgelength[ i ];
			}
			else
			{
				shortlen = edgelength[ i ] < shortlen ? edgelength[ i ] : shortlen;
				longlen = edgelength[ i ] > longlen ? edgelength[ i ] : longlen;
			}
			if(edgelength[ i ] > longest)
			{
				longest = edgelength[ i ];
			}
			if(edgelength[ i ] < shortest)
			{
				shortest = edgelength[ i ];
			}
		}

		// Set the matrix A = [V[0], V[1], V[2]]^T.
		for(j = 0; j < 3; j++)
		{
			for(i = 0; i < 3; i++) A[ j ][ i ] = V[ j ][ i ];
		}

		// Decompose A just once.
		if(lu_decmp( A, 3, indx, &D, 0 ))
		{
			// Get the three faces normals.
			for(j = 0; j < 3; j++)
			{
				for(i = 0; i < 3; i++) rhs[ i ] = 0.0;
				rhs[ j ] = 1.0;  // Positive means the inside direction
				lu_solve( A, 3, indx, rhs, 0 );
				for(i = 0; i < 3; i++) N[ j ][ i ] = rhs[ i ];
			}
			// Get the fourth face normal by summing up the first three.
			for(i = 0; i < 3; i++) N[ 3 ][ i ] = -N[ 0 ][ i ] - N[ 1 ][ i ] - N[ 2 ][ i ];
			// Get the radius of the circumsphere.
			for(i = 0; i < 3; i++) rhs[ i ] = 0.5 * dot( V[ i ], V[ i ] );
			lu_solve( A, 3, indx, rhs, 0 );
			cirradius = sqrt( dot( rhs, rhs ) );
			// Normalize the face normals.
			for(i = 0; i < 4; i++)
			{
				// H[i] is the inverse of height of its corresponding face.
				H[ i ] = sqrt( dot( N[ i ], N[ i ] ) );
				for(j = 0; j < 3; j++) N[ i ][ j ] /= H[ i ];
			}
			// Get the radius of the inscribed sphere.
			// insradius = 1.0 / (H[0] + H[1] + H[2] + H[3]);
			// Get the biggest H[i] (corresponding to the smallest height).
			minheightinv = H[ 0 ];
			for(i = 1; i < 4; i++)
			{
				if(H[ i ] > minheightinv) minheightinv = H[ i ];
			}
		}
		else
		{
			// A nearly degenerated tet.
			if(tetvol <= 0.0)
			{
				myprintf( "  !! Warning:  A %s tet (%d,%d,%d,%d).\n",
						tetvol < 0 ? "inverted" : "degenerated", pointmark( p[ 0 ] ),
						pointmark( p[ 1 ] ), pointmark( p[ 2 ] ), pointmark( p[ 3 ] ) );
				// Skip it.        
				tetloop.tet = tetrahedrontraverse( );
				continue;
			}
			// Calculate the four face normals.
			facenormal( p[ 2 ], p[ 1 ], p[ 3 ], N[ 0 ], 1, NULL );
			facenormal( p[ 0 ], p[ 2 ], p[ 3 ], N[ 1 ], 1, NULL );
			facenormal( p[ 1 ], p[ 0 ], p[ 3 ], N[ 2 ], 1, NULL );
			facenormal( p[ 0 ], p[ 1 ], p[ 2 ], N[ 3 ], 1, NULL );
			// Normalize the face normals.
			for(i = 0; i < 4; i++)
			{
				// H[i] is the twice of the area of the face.
				H[ i ] = sqrt( dot( N[ i ], N[ i ] ) );
				for(j = 0; j < 3; j++) N[ i ][ j ] /= H[ i ];
			}
			// Get the biggest H[i] / tetvol (corresponding to the smallest height).
			minheightinv = ( H[ 0 ] / tetvol );
			for(i = 1; i < 4; i++)
			{
				if(( H[ i ] / tetvol ) > minheightinv) minheightinv = ( H[ i ] / tetvol );
			}
			// Let the circumradius to be the half of its longest edge length.
			cirradius = 0.5 * sqrt( longlen );
		}

		// Get the dihedrals (in degree) at each edges.
		j = 0;
		for(i = 1; i < 4; i++)
		{
			alldihed[ j ] = -dot( N[ 0 ], N[ i ] ); // Edge cd, bd, bc.
			if(alldihed[ j ] < -1.0) alldihed[ j ] = -1; // Rounding.
			else if(alldihed[ j ] > 1.0) alldihed[ j ] = 1;
			alldihed[ j ] = acos( alldihed[ j ] ) / PIconstant * 180.0;
			j++;
		}
		for(i = 2; i < 4; i++)
		{
			alldihed[ j ] = -dot( N[ 1 ], N[ i ] ); // Edge ad, ac.
			if(alldihed[ j ] < -1.0) alldihed[ j ] = -1; // Rounding.
			else if(alldihed[ j ] > 1.0) alldihed[ j ] = 1;
			alldihed[ j ] = acos( alldihed[ j ] ) / PIconstant * 180.0;
			j++;
		}
		alldihed[ j ] = -dot( N[ 2 ], N[ 3 ] ); // Edge ab.
		if(alldihed[ j ] < -1.0) alldihed[ j ] = -1; // Rounding.
		else if(alldihed[ j ] > 1.0) alldihed[ j ] = 1;
		alldihed[ j ] = acos( alldihed[ j ] ) / PIconstant * 180.0;

		// Calculate the largest and smallest dihedral angles.
		for(i = 0; i < 6; i++)
		{
			if(i == 0)
			{
				smalldiangle = bigdiangle = alldihed[ i ];
			}
			else
			{
				smalldiangle = alldihed[ i ] < smalldiangle ? alldihed[ i ] : smalldiangle;
				bigdiangle = alldihed[ i ] > bigdiangle ? alldihed[ i ] : bigdiangle;
			}
			if(alldihed[ i ] < smallestdiangle)
			{
				smallestdiangle = alldihed[ i ];
			}
			if(alldihed[ i ] > biggestdiangle)
			{
				biggestdiangle = alldihed[ i ];
			}
			// Accumulate the corresponding number in the dihedral angle histogram.
			if(alldihed[ i ] < 5.0)
			{
				tendegree = 0;
			}
			else if(alldihed[ i ] >= 5.0 && alldihed[ i ] < 10.0)
			{
				tendegree = 1;
			}
			else if(alldihed[ i ] >= 80.0 && alldihed[ i ] < 110.0)
			{
				tendegree = 9; // Angles between 80 to 110 degree are in one entry.
			}
			else if(alldihed[ i ] >= 170.0 && alldihed[ i ] < 175.0)
			{
				tendegree = 16;
			}
			else if(alldihed[ i ] >= 175.0)
			{
				tendegree = 17;
			}
			else
			{
				tendegree = (int)( alldihed[ i ] / 10. );
				if(alldihed[ i ] < 80.0)
				{
					tendegree++;  // In the left column.
				}
				else
				{
					tendegree--;  // In the right column.
				}
			}
			dihedangletable[ tendegree ]++;
		}



		// Calculate the largest and smallest face angles.
		for(tetloop.ver = 0; tetloop.ver < 4; tetloop.ver++)
		{
			fsym( tetloop, neightet );
			// Only do the calulation once for a face.
			if(( (point)neightet.tet[ 7 ] == dummypoint ) ||
				( tetloop.tet < neightet.tet ))
			{
				p[ 0 ] = org( tetloop );
				p[ 1 ] = dest( tetloop );
				p[ 2 ] = apex( tetloop );
				faceangle[ 0 ] = interiorangle( p[ 0 ], p[ 1 ], p[ 2 ], NULL );
				faceangle[ 1 ] = interiorangle( p[ 1 ], p[ 2 ], p[ 0 ], NULL );
				faceangle[ 2 ] = PIconstant - ( faceangle[ 0 ] + faceangle[ 1 ] );
				// Translate angles into degrees.
				for(i = 0; i < 3; i++)
				{
					faceangle[ i ] = ( faceangle[ i ] * 180.0 ) / PIconstant;
				}
				// Calculate the largest and smallest face angles.
				for(i = 0; i < 3; i++)
				{
					if(i == 0)
					{
						smallfaangle = bigfaangle = faceangle[ i ];
					}
					else
					{
						smallfaangle = faceangle[ i ] < smallfaangle ?
							faceangle[ i ] : smallfaangle;
						bigfaangle = faceangle[ i ] > bigfaangle ? faceangle[ i ] : bigfaangle;
					}
					if(faceangle[ i ] < smallestfaangle)
					{
						smallestfaangle = faceangle[ i ];
					}
					if(faceangle[ i ] > biggestfaangle)
					{
						biggestfaangle = faceangle[ i ];
					}
					tendegree = (int)( faceangle[ i ] / 10. );
					faceangletable[ tendegree ]++;
				}
			}
		}

		// Calculate aspect ratio and radius-edge ratio for this element.
		tetradius = cirradius / sqrt( shortlen );
		// tetaspect = sqrt(longlen) / (2.0 * insradius);
		tetaspect = sqrt( longlen ) * minheightinv;
		// Remember the largest and smallest aspect ratio.
		if(tetaspect < smallestratio)
		{
			smallestratio = tetaspect;
		}
		if(tetaspect > biggestratio)
		{
			biggestratio = tetaspect;
		}
		// Accumulate the corresponding number in the aspect ratio histogram.
		aspectindex = 0;
		while(( tetaspect > aspectratiotable[ aspectindex ] ) && ( aspectindex < 11 ))
		{
			aspectindex++;
		}
		aspecttable[ aspectindex ]++;
		radiusindex = 0;
		while(( tetradius > radiusratiotable[ radiusindex ] ) && ( radiusindex < 11 ))
		{
			radiusindex++;
		}
		radiustable[ radiusindex ]++;

		tetloop.tet = tetrahedrontraverse( );
	}

	shortest = sqrt( shortest );
	longest = sqrt( longest );
	minaltitude = sqrt( minaltitude );

	myprintf( "  Smallest volume: %16.5g   |  Largest volume: %16.5g\n",
			smallestvolume, biggestvolume );
	myprintf( "  Shortest edge:   %16.5g   |  Longest edge:   %16.5g\n",
			shortest, longest );
	myprintf( "  Smallest asp.ratio: %13.5g   |  Largest asp.ratio: %13.5g\n",
			smallestratio, biggestratio );
	sprintf( sbuf, "%.17g", biggestfaangle );
	if(strlen( sbuf ) > 8)
	{
		sbuf[ 8 ] = '\0';
	}
	myprintf( "  Smallest facangle: %14.5g   |  Largest facangle:       %s\n",
			smallestfaangle, sbuf );
	sprintf( sbuf, "%.17g", biggestdiangle );
	if(strlen( sbuf ) > 8)
	{
		sbuf[ 8 ] = '\0';
	}
	myprintf( "  Smallest dihedral: %14.5g   |  Largest dihedral:       %s\n\n",
			smallestdiangle, sbuf );

	myprintf( "  Aspect ratio histogram:\n" );
	myprintf( "         < %-6.6g    :  %8ld      | %6.6g - %-6.6g     :  %8ld\n",
			aspectratiotable[ 0 ], aspecttable[ 0 ], aspectratiotable[ 5 ],
			aspectratiotable[ 6 ], aspecttable[ 6 ] );
	for(i = 1; i < 5; i++)
	{
		myprintf( "  %6.6g - %-6.6g    :  %8ld      | %6.6g - %-6.6g     :  %8ld\n",
				aspectratiotable[ i - 1 ], aspectratiotable[ i ], aspecttable[ i ],
				aspectratiotable[ i + 5 ], aspectratiotable[ i + 6 ],
				aspecttable[ i + 6 ] );
	}
	myprintf( "  %6.6g - %-6.6g    :  %8ld      | %6.6g -            :  %8ld\n",
			aspectratiotable[ 4 ], aspectratiotable[ 5 ], aspecttable[ 5 ],
			aspectratiotable[ 10 ], aspecttable[ 11 ] );
	myprintf( "  (A tetrahedron's aspect ratio is its longest edge length" );
	myprintf( " divided by its\n" );
	myprintf( "    smallest side height)\n\n" );

	myprintf( "  Face angle histogram:\n" );
	for(i = 0; i < 9; i++)
	{
		myprintf( "    %3d - %3d degrees:  %8ld      |    %3d - %3d degrees:  %8ld\n",
				i * 10, i * 10 + 10, faceangletable[ i ],
				i * 10 + 90, i * 10 + 100, faceangletable[ i + 9 ] );
	}
	if(minfaceang != PIconstant)
	{
		myprintf( "  Minimum input face angle is %g (degree).\n",
				minfaceang / PIconstant * 180.0 );
	}
	myprintf( "\n" );

	myprintf( "  Dihedral angle histogram:\n" );
	// Print the three two rows:
	myprintf( "     %3d - %2d degrees:  %8ld      |    %3d - %3d degrees:  %8ld\n",
			0, 5, dihedangletable[ 0 ], 80, 110, dihedangletable[ 9 ] );
	myprintf( "     %3d - %2d degrees:  %8ld      |    %3d - %3d degrees:  %8ld\n",
			5, 10, dihedangletable[ 1 ], 110, 120, dihedangletable[ 10 ] );
	// Print the third to seventh rows.
	for(i = 2; i < 7; i++)
	{
		myprintf( "     %3d - %2d degrees:  %8ld      |    %3d - %3d degrees:  %8ld\n",
				( i - 1 ) * 10, ( i - 1 ) * 10 + 10, dihedangletable[ i ],
				( i - 1 ) * 10 + 110, ( i - 1 ) * 10 + 120, dihedangletable[ i + 9 ] );
	}
	// Print the last two rows.
	myprintf( "     %3d - %2d degrees:  %8ld      |    %3d - %3d degrees:  %8ld\n",
			60, 70, dihedangletable[ 7 ], 170, 175, dihedangletable[ 16 ] );
	myprintf( "     %3d - %2d degrees:  %8ld      |    %3d - %3d degrees:  %8ld\n",
			70, 80, dihedangletable[ 8 ], 175, 180, dihedangletable[ 17 ] );
	if(minfacetdihed != PIconstant)
	{
		myprintf( "  Minimum input dihedral angle is %g (degree).\n",
				minfacetdihed / PIconstant * 180.0 );
	}
	myprintf( "\n" );

	myprintf( "\n" );
}


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// memorystatistics()    Report the memory usage.                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::memorystatistics( )
{
	myprintf( "Memory usage statistics:\n\n" );

	// Count the number of blocks of tetrahedra. 
	int tetblocks = 0;
	tetrahedrons->pathblock = tetrahedrons->firstblock;
	while(tetrahedrons->pathblock != NULL)
	{
		tetblocks++;
		tetrahedrons->pathblock = (void **)*( tetrahedrons->pathblock );
	}

	// Calculate the total memory (in bytes) used by storing meshes.
	unsigned long totalmeshmemory = 0l, totalt2shmemory = 0l;
	totalmeshmemory = points->maxitems * points->itembytes +
		tetrahedrons->maxitems * tetrahedrons->itembytes;
	if(b->plc || b->refine)
	{
		totalmeshmemory += ( subfaces->maxitems * subfaces->itembytes +
							 subsegs->maxitems * subsegs->itembytes );
		totalt2shmemory = ( tet2subpool->maxitems * tet2subpool->itembytes +
							tet2segpool->maxitems * tet2segpool->itembytes );
	}

	unsigned long totalalgomemory = 0l;
	totalalgomemory = cavetetlist->totalmemory + cavebdrylist->totalmemory +
		caveoldtetlist->totalmemory +
		flippool->maxitems * flippool->itembytes;
	if(b->plc || b->refine)
	{
		totalalgomemory += ( subsegstack->totalmemory + subfacstack->totalmemory +
							 subvertstack->totalmemory +
							 caveshlist->totalmemory + caveshbdlist->totalmemory +
							 cavesegshlist->totalmemory +
							 cavetetshlist->totalmemory +
							 cavetetseglist->totalmemory +
							 caveencshlist->totalmemory +
							 caveencseglist->totalmemory +
							 cavetetvertlist->totalmemory +
							 unflipqueue->totalmemory );
	}

	myprintf( "  Maximum number of tetrahedra:  %ld\n", tetrahedrons->maxitems );
	myprintf( "  Maximum number of tet blocks (blocksize = %d):  %d\n",
			b->tetrahedraperblock, tetblocks );
	/*
	if (b->plc || b->refine) {
	myprintf("  Approximate memory for tetrahedral mesh (bytes):  %ld\n",
	totalmeshmemory);

	myprintf("  Approximate memory for extra pointers (bytes):  %ld\n",
	totalt2shmemory);
	} else {
	myprintf("  Approximate memory for tetrahedralization (bytes):  %ld\n",
	totalmeshmemory);
	}
	myprintf("  Approximate memory for algorithms (bytes):  %ld\n",
	totalalgomemory);
	myprintf("  Approximate memory for working arrays (bytes):  %ld\n",
	totalworkmemory);
	myprintf("  Approximate total used memory (bytes):  %ld\n",
	totalmeshmemory + totalt2shmemory + totalalgomemory +
	totalworkmemory);
	*/
	if(b->plc || b->refine)
	{
		myprintf( "  Approximate memory for tetrahedral mesh (bytes):  " );
		printfcomma( totalmeshmemory ); myprintf( "\n" );

		myprintf( "  Approximate memory for extra pointers (bytes):  " );
		printfcomma( totalt2shmemory ); myprintf( "\n" );
	}
	else
	{
		myprintf( "  Approximate memory for tetrahedralization (bytes):  " );
		printfcomma( totalmeshmemory ); myprintf( "\n" );
	}
	myprintf( "  Approximate memory for algorithms (bytes):  " );
	printfcomma( totalalgomemory ); myprintf( "\n" );
	myprintf( "  Approximate memory for working arrays (bytes):  " );
	printfcomma( totalworkmemory ); myprintf( "\n" );
	myprintf( "  Approximate total used memory (bytes):  " );
	printfcomma( totalmeshmemory + totalt2shmemory + totalalgomemory +
				 totalworkmemory );
	myprintf( "\n" );

	myprintf( "\n" );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// statistics()    Print all sorts of cool facts.                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::statistics( )
{
	long tetnumber, facenumber;

	myprintf( "\nStatistics:\n\n" );
	myprintf( "  Input points: %d\n", in->numberofpoints );
	if(b->refine)
	{
		myprintf( "  Input tetrahedra: %d\n", in->numberoftetrahedra );
		if(in->numberoftrifaces > 0)
		{
			myprintf( "  Input triangles: %d\n", in->numberoftrifaces );
		}
		if(in->numberofedges > 0)
		{
			myprintf( "  Input edges: %d\n", in->numberofedges );
		}
	}
	else if(b->plc)
	{
		myprintf( "  Input facets: %d\n", in->numberoffacets );
		myprintf( "  Input segments: %ld\n", insegments );
		if(in->numberofedges > 0)
		{
			myprintf( "  Input edges: %d\n", in->numberofedges );
		}
		myprintf( "  Input holes: %d\n", in->numberofholes );
		myprintf( "  Input regions: %d\n", in->numberofregions );
	}

	tetnumber = tetrahedrons->items - hullsize;
	facenumber = ( tetnumber * 4l + hullsize ) / 2l;

	if(b->weighted)
	{ // -w option
		myprintf( "\n  Mesh points: %ld\n", points->items - nonregularcount );
	}
	else
	{
		myprintf( "\n  Mesh points: %ld\n", points->items );
	}
	myprintf( "  Mesh tetrahedra: %ld\n", tetnumber );
	myprintf( "  Mesh faces: %ld\n", facenumber );
	if(meshedges > 0l)
	{
		myprintf( "  Mesh edges: %ld\n", meshedges );
	}
	else
	{
		if(!nonconvex)
		{
			long vsize = points->items - dupverts - unuverts;
			if(b->weighted) vsize -= nonregularcount;
			meshedges = vsize + facenumber - tetnumber - 1;
			myprintf( "  Mesh edges: %ld\n", meshedges );
		}
	}

	if(b->plc || b->refine)
	{
		myprintf( "  Mesh faces on exterior boundary: %ld\n", hullsize );
		if(meshhulledges > 0l)
		{
			myprintf( "  Mesh edges on exterior boundary: %ld\n", meshhulledges );
		}
		myprintf( "  Mesh faces on input facets: %ld\n", subfaces->items );
		myprintf( "  Mesh edges on input segments: %ld\n", subsegs->items );
		if(st_facref_count > 0l)
		{
			myprintf( "  Steiner points on input facets:  %ld\n", st_facref_count );
		}
		if(st_segref_count > 0l)
		{
			myprintf( "  Steiner points on input segments:  %ld\n", st_segref_count );
		}
		if(st_volref_count > 0l)
		{
			myprintf( "  Steiner points inside domain: %ld\n", st_volref_count );
		}
	}
	else
	{
		myprintf( "  Convex hull faces: %ld\n", hullsize );
		if(meshhulledges > 0l)
		{
			myprintf( "  Convex hull edges: %ld\n", meshhulledges );
		}
	}
	if(b->weighted)
	{ // -w option
		myprintf( "  Skipped non-regular points: %ld\n", nonregularcount );
	}
	myprintf( "\n" );


	if(b->verbose > 0)
	{
		if(b->plc || b->refine)
		{ // -p or -r
			if(tetrahedrons->items > 0l)
			{
				qualitystatistics( );
			}
		}
		if(tetrahedrons->items > 0l)
		{
			memorystatistics( );
		}
	}
}

////                                                                       ////
////                                                                       ////
//// meshstat_cxx /////////////////////////////////////////////////////////////

//// output_cxx ///////////////////////////////////////////////////////////////
////                                                                       ////
////                                                                       ////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// jettisonnodes()    Jettison unused or duplicated vertices.                //
//                                                                           //
// Unused points are those input points which are outside the mesh domain or //
// have no connection (isolated) to the mesh.  Duplicated points exist for   //
// example if the input PLC is read from a .stl mesh file (marked during the //
// Delaunay tetrahedralization step. This routine remove these points from   //
// points list. All existing points are reindexed.                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::jettisonnodes( )
{
	point pointloop;
	bool jetflag;
	int oldidx, newidx;
	int remcount;

	if(!b->quiet)
	{
		myprintf( "Jettisoning redundant points.\n" );
	}

	points->traversalinit( );
	pointloop = pointtraverse( );
	oldidx = newidx = 0; // in->firstnumber;
	remcount = 0;
	while(pointloop != (point)NULL)
	{
		jetflag = ( pointtype( pointloop ) == DUPLICATEDVERTEX ) ||
			( pointtype( pointloop ) == UNUSEDVERTEX );
		if(jetflag)
		{
			// It is a duplicated or unused point, delete it.
			pointdealloc( pointloop );
			remcount++;
		}
		else
		{
			// Re-index it.
			setpointmark( pointloop, newidx + in->firstnumber );
			if(in->pointmarkerlist != (int *)NULL)
			{
				if(oldidx < in->numberofpoints)
				{
					// Re-index the point marker as well.
					in->pointmarkerlist[ newidx ] = in->pointmarkerlist[ oldidx ];
				}
			}
			newidx++;
		}
		oldidx++;
		pointloop = pointtraverse( );
	}
	if(b->verbose)
	{
		myprintf( "  %ld duplicated vertices are removed.\n", dupverts );
		myprintf( "  %ld unused vertices are removed.\n", unuverts );
	}
	dupverts = 0l;
	unuverts = 0l;

	// The following line ensures that dead items in the pool of nodes cannot
	//   be allocated for the new created nodes. This ensures that the input
	//   nodes will occur earlier in the output files, and have lower indices.
	points->deaditemstack = (void *)NULL;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// highorder()   Create extra nodes for quadratic subparametric elements.    //
//                                                                           //
// 'highordertable' is an array (size = numberoftetrahedra * 6) for storing  //
// high-order nodes of each tetrahedron.  This routine is used only when -o2 //
// switch is used.                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::highorder( )
{
	triface tetloop, worktet, spintet;
	point *extralist, *adjextralist;
	point torg, tdest, newpoint;
	int highorderindex;
	int t1ver;
	int i, j;

	if(!b->quiet)
	{
		myprintf( "Adding vertices for second-order tetrahedra.\n" );
	}

	// Initialize the 'highordertable'.
	highordertable = new point[ tetrahedrons->items * 6 ];
	if(highordertable == (point *)NULL)
	{
		terminatetetgen( this, 1 );
	}

	// This will overwrite the slot for element markers.
	highorderindex = 11;

	// The following line ensures that dead items in the pool of nodes cannot
	//   be allocated for the extra nodes associated with high order elements.
	//   This ensures that the primary nodes (at the corners of elements) will
	//   occur earlier in the output files, and have lower indices, than the
	//   extra nodes.
	points->deaditemstack = (void *)NULL;

	// Assign an entry for each tetrahedron to find its extra nodes. At the
	//   mean while, initialize all extra nodes be NULL.
	i = 0;
	tetrahedrons->traversalinit( );
	tetloop.tet = tetrahedrontraverse( );
	while(tetloop.tet != (tetrahedron *)NULL)
	{
		tetloop.tet[ highorderindex ] = (tetrahedron)&highordertable[ i ];
		for(j = 0; j < 6; j++)
		{
			highordertable[ i + j ] = (point)NULL;
		}
		i += 6;
		tetloop.tet = tetrahedrontraverse( );
	}

	// To create a unique node on each edge. Loop over all tetrahedra, and
	//   look at the six edges of each tetrahedron.  If the extra node in
	//   the tetrahedron corresponding to this edge is NULL, create a node
	//   for this edge, at the same time, set the new node into the extra
	//   node lists of all other tetrahedra sharing this edge.  
	tetrahedrons->traversalinit( );
	tetloop.tet = tetrahedrontraverse( );
	while(tetloop.tet != (tetrahedron *)NULL)
	{
		// Get the list of extra nodes.
		extralist = (point *)tetloop.tet[ highorderindex ];
		worktet.tet = tetloop.tet;
		for(i = 0; i < 6; i++)
		{
			if(extralist[ i ] == (point)NULL)
			{
				// Go to the ith-edge.
				worktet.ver = edge2ver[ i ];
				// Create a new point in the middle of this edge.
				torg = org( worktet );
				tdest = dest( worktet );
				makepoint( &newpoint, FREEVOLVERTEX );
				// UV: TODO
				for(j = 0; j < 3 + numpointattrib; j++)
				{
					newpoint[ j ] = 0.5 * ( torg[ j ] + tdest[ j ] );
				}
				// Interpolate its metrics.
				for(j = 0; j < in->numberofpointmtrs; j++)
				{
					newpoint[ pointmtrindex + j ] =
						0.5 * ( torg[ pointmtrindex + j ] + tdest[ pointmtrindex + j ] );
				}
				// Set this point into all extra node lists at this edge.
				spintet = worktet;
				while(1)
				{
					if(!ishulltet( spintet ))
					{
						adjextralist = (point *)spintet.tet[ highorderindex ];
						adjextralist[ ver2edge[ spintet.ver ] ] = newpoint;
					}
					fnextself( spintet );
					if(spintet.tet == worktet.tet) break;
				}
			} // if (!extralist[i])
		} // i
		tetloop.tet = tetrahedrontraverse( );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// indexelements()    Index all tetrahedra.                                  //
//                                                                           //
// Many output functions require that the tetrahedra are indexed.  This      //
// routine is called when -E option is used.                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::indexelements( )
{
	triface worktet;
	int eindex = b->zeroindex ? 0 : in->firstnumber; // firstindex;
	tetrahedrons->traversalinit( );
	worktet.tet = tetrahedrontraverse( );
	while(worktet.tet != NULL)
	{
		setelemindex( worktet.tet, eindex );
		eindex++;
		if(b->metric)
		{ // -m option
			// Update the point-to-tet map, so that every point is pointing
			//   to a real tet, not a fictious one. Used by .p2t file.
			tetrahedron tptr = encode( worktet );
			for(int i = 0; i < 4; i++)
			{
				setpoint2tet( (point)( worktet.tet[ 4 + i ] ), tptr );
			}
		}
		worktet.tet = tetrahedrontraverse( );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// numberedges()    Count the number of edges, save in "meshedges".          //
//                                                                           //
// This routine is called when '-p' or '-r', and '-E' options are used.  The //
// total number of edges depends on the genus of the input surface mesh.     //
//                                                                           //
// NOTE:  This routine must be called after outelements().  So all elements  //
// have been indexed.                                                        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::numberedges( )
{
	triface worktet, spintet;
	int ishulledge;
	int t1ver;
	int i;

	meshedges = meshhulledges = 0l;

	tetrahedrons->traversalinit( );
	worktet.tet = tetrahedrontraverse( );
	while(worktet.tet != NULL)
	{
		for(i = 0; i < 6; i++)
		{
			worktet.ver = edge2ver[ i ];
			ishulledge = 0;
			fnext( worktet, spintet );
			do
			{
				if(!ishulltet( spintet ))
				{
					if(elemindex( spintet.tet ) < elemindex( worktet.tet )) break;
				}
				else
				{
					ishulledge = 1;
				}
				fnextself( spintet );
			}
			while(spintet.tet != worktet.tet);
			if(spintet.tet == worktet.tet)
			{
				meshedges++;
				if(ishulledge) meshhulledges++;
			}
		}
		infect( worktet );
		worktet.tet = tetrahedrontraverse( );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// outnodes()    Output the points to a .node file or a tetgenio structure.  //
//                                                                           //
// Note: each point has already been numbered on input (the first index is   //
// 'in->firstnumber').                                                       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::outnodes( tetgenio* out )
{
	FILE *outfile = NULL;
	char outnodefilename[ FILENAMESIZE ];
	face parentsh;
	point pointloop;
	int nextras, bmark, marker = 0, weightDT = 0;
	int coordindex, attribindex;
	int pointnumber, firstindex;
	int index, i;

	if(out == (tetgenio *)NULL)
	{
		strcpy( outnodefilename, b->outfilename );
		strcat( outnodefilename, ".node" );
	}

	if(!b->quiet)
	{
		if(out == (tetgenio *)NULL)
		{
			myprintf( "Writing %s.\n", outnodefilename );
		}
		else
		{
			myprintf( "Writing nodes.\n" );
		}
	}

	nextras = numpointattrib;
	if(b->weighted)
	{ // -w
		if(b->weighted_param == 0) weightDT = 1; // Weighted DT.
	}

	bmark = !b->nobound && in->pointmarkerlist;

	if(out == (tetgenio *)NULL)
	{
		outfile = fopen( outnodefilename, "w" );
		if(outfile == (FILE *)NULL)
		{
			myprintf( "File I/O Error:  Cannot create file %s.\n", outnodefilename );
			terminatetetgen( this, 1 );
		}
		// Number of points, number of dimensions, number of point attributes,
		//   and number of boundary markers (zero or one).
		fprintf( outfile, "%ld  %d  %d  %d\n", points->items, 3, nextras, bmark );
	}
	else
	{
		// Allocate space for 'pointlist';
		out->pointlist = new REAL[ points->items * 3 ];
		if(out->pointlist == (REAL *)NULL)
		{
			myprintf( "Error:  Out of memory.\n" );
			terminatetetgen( this, 1 );
		}
		// Allocate space for 'pointattributelist' if necessary;
		if(nextras > 0)
		{
			out->pointattributelist = new REAL[ points->items * nextras ];
			if(out->pointattributelist == (REAL *)NULL)
			{
				myprintf( "Error:  Out of memory.\n" );
				terminatetetgen( this, 1 );
			}
		}
		// Allocate space for 'pointmarkerlist' if necessary;
		if(bmark)
		{
			out->pointmarkerlist = new int[ points->items ];
			if(out->pointmarkerlist == (int *)NULL)
			{
				myprintf( "Error:  Out of memory.\n" );
				terminatetetgen( this, 1 );
			}
		}
		if(b->psc)
		{
			out->pointparamlist = new tetgenio::pointparam[ points->items ];
			if(out->pointparamlist == NULL)
			{
				myprintf( "Error:  Out of memory.\n" );
				terminatetetgen( this, 1 );
			}
		}
		out->numberofpoints = points->items;
		out->numberofpointattributes = nextras;
		coordindex = 0;
		attribindex = 0;
	}

	// Determine the first index (0 or 1).
	firstindex = b->zeroindex ? 0 : in->firstnumber;

	points->traversalinit( );
	pointloop = pointtraverse( );
	pointnumber = firstindex; // in->firstnumber;
	index = 0;
	while(pointloop != (point)NULL)
	{
		if(bmark)
		{
			// Default the vertex has a zero marker.
			marker = 0;
			// Is it an input vertex?
			if(index < in->numberofpoints)
			{
				// Input point's marker is directly copied to output.
				marker = in->pointmarkerlist[ index ];
			}
			else
			{
				if(( pointtype( pointloop ) == FREESEGVERTEX ) ||
					( pointtype( pointloop ) == FREEFACETVERTEX ))
				{
					sdecode( point2sh( pointloop ), parentsh );
					if(parentsh.sh != NULL)
					{
						marker = shellmark( parentsh );
					}
				} // if (pointtype(...))
			}
		}
		if(out == (tetgenio *)NULL)
		{
			// Point number, x, y and z coordinates.
			fprintf( outfile, "%4d    %.17g  %.17g  %.17g", pointnumber,
					 pointloop[ 0 ], pointloop[ 1 ], pointloop[ 2 ] );
			for(i = 0; i < nextras; i++)
			{
				// Write an attribute.
				if(( i == 0 ) && weightDT)
				{
					fprintf( outfile, "  %.17g", pointloop[ 0 ] * pointloop[ 0 ] +
							 pointloop[ 1 ] * pointloop[ 1 ] + pointloop[ 2 ] * pointloop[ 2 ]
							 - pointloop[ 3 + i ] );
				}
				else
				{
					fprintf( outfile, "  %.17g", pointloop[ 3 + i ] );
				}
			}
			if(bmark)
			{
				// Write the boundary marker.
				fprintf( outfile, "    %d", marker );
			}
			if(b->psc)
			{
				fprintf( outfile, "  %.8g  %.8g  %d", pointgeomuv( pointloop, 0 ),
						 pointgeomuv( pointloop, 1 ), pointgeomtag( pointloop ) );
				if(pointtype( pointloop ) == RIDGEVERTEX)
				{
					fprintf( outfile, "  0" );
				}
				else if(pointtype( pointloop ) == ACUTEVERTEX)
				{
					fprintf( outfile, "  0" );
				}
				else if(pointtype( pointloop ) == FREESEGVERTEX)
				{
					fprintf( outfile, "  1" );
				}
				else if(pointtype( pointloop ) == FREEFACETVERTEX)
				{
					fprintf( outfile, "  2" );
				}
				else if(pointtype( pointloop ) == FREEVOLVERTEX)
				{
					fprintf( outfile, "  3" );
				}
				else
				{
					fprintf( outfile, "  -1" ); // Unknown type.
				}
			}
			fprintf( outfile, "\n" );
		}
		else
		{
			// X, y, and z coordinates.
			out->pointlist[ coordindex++ ] = pointloop[ 0 ];
			out->pointlist[ coordindex++ ] = pointloop[ 1 ];
			out->pointlist[ coordindex++ ] = pointloop[ 2 ];
			// Point attributes.
			for(i = 0; i < nextras; i++)
			{
				// Output an attribute.
				if(( i == 0 ) && weightDT)
				{
					out->pointattributelist[ attribindex++ ] =
						pointloop[ 0 ] * pointloop[ 0 ] + pointloop[ 1 ] * pointloop[ 1 ] +
						pointloop[ 2 ] * pointloop[ 2 ] - pointloop[ 3 + i ];
				}
				else
				{
					out->pointattributelist[ attribindex++ ] = pointloop[ 3 + i ];
				}
			}
			if(bmark)
			{
				// Output the boundary marker.  
				out->pointmarkerlist[ index ] = marker;
			}
			if(b->psc)
			{
				for(int tt = 0; tt < tetgenio::pointparam::maxuvs; ++tt)
				{
					out->pointparamlist[ index ].uv[ tt ] = pointgeomuv( pointloop, tt );
				}
				out->pointparamlist[ index ].tag = pointgeomtag( pointloop );
				if(pointtype( pointloop ) == RIDGEVERTEX)
				{
					out->pointparamlist[ index ].type = 0;
				}
				else if(pointtype( pointloop ) == ACUTEVERTEX)
				{
					out->pointparamlist[ index ].type = 0;
				}
				else if(pointtype( pointloop ) == FREESEGVERTEX)
				{
					out->pointparamlist[ index ].type = 1;
				}
				else if(pointtype( pointloop ) == FREEFACETVERTEX)
				{
					out->pointparamlist[ index ].type = 2;
				}
				else if(pointtype( pointloop ) == FREEVOLVERTEX)
				{
					out->pointparamlist[ index ].type = 3;
				}
				else
				{
					out->pointparamlist[ index ].type = -1; // Unknown type.
				}
			}
		}
		pointloop = pointtraverse( );
		pointnumber++;
		index++;
	}

	if(out == (tetgenio *)NULL)
	{
		fprintf( outfile, "# Generated by %s\n", b->commandline );
		fclose( outfile );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// outmetrics()    Output the metric to a file (*.mtr) or a tetgenio obj.    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::outmetrics( tetgenio* out )
{
	FILE *outfile = NULL;
	char outmtrfilename[ FILENAMESIZE ];
	point ptloop;
	int mtrindex = 0;
	int i;
	int msize = ( sizeoftensor - useinsertradius );
	if(msize == 0)
	{
		return;
	}

	if(out == (tetgenio *)NULL)
	{
		strcpy( outmtrfilename, b->outfilename );
		strcat( outmtrfilename, ".mtr" );
	}

	if(!b->quiet)
	{
		if(out == (tetgenio *)NULL)
		{
			myprintf( "Writing %s.\n", outmtrfilename );
		}
		else
		{
			myprintf( "Writing metrics.\n" );
		}
	}

	if(out == (tetgenio *)NULL)
	{
		outfile = fopen( outmtrfilename, "w" );
		if(outfile == (FILE *)NULL)
		{
			myprintf( "File I/O Error:  Cannot create file %s.\n", outmtrfilename );
			terminatetetgen( this, 3 );
		}
		// Number of points, number of point metrices,
		fprintf( outfile, "%ld  %d\n", points->items, msize );
	}
	else
	{
		// Allocate space for 'pointmtrlist'.
		out->numberofpointmtrs = msize;
		out->pointmtrlist = new REAL[ points->items * msize ];
		if(out->pointmtrlist == (REAL *)NULL)
		{
			terminatetetgen( this, 1 );
		}
	}

	points->traversalinit( );
	ptloop = pointtraverse( );
	while(ptloop != (point)NULL)
	{
		if(out == (tetgenio *)NULL)
		{
			for(i = 0; i < msize; i++)
			{
				fprintf( outfile, " %-16.8e", ptloop[ pointmtrindex + i ] );
			}
			fprintf( outfile, "\n" );
		}
		else
		{
			for(i = 0; i < msize; i++)
			{
				out->pointmtrlist[ mtrindex++ ] = ptloop[ pointmtrindex + i ];
			}
		}
		ptloop = pointtraverse( );
	}

	// Output the point-to-tet map.
	if(out == (tetgenio *)NULL)
	{
		strcpy( outmtrfilename, b->outfilename );
		strcat( outmtrfilename, ".p2t" );
	}

	if(!b->quiet)
	{
		if(out == (tetgenio *)NULL)
		{
			myprintf( "Writing %s.\n", outmtrfilename );
		}
		else
		{
			myprintf( "Writing point-to-tet map.\n" );
		}
	}

	if(out == (tetgenio *)NULL)
	{
		outfile = fopen( outmtrfilename, "w" );
		if(outfile == (FILE *)NULL)
		{
			myprintf( "File I/O Error:  Cannot create file %s.\n", outmtrfilename );
			terminatetetgen( this, 3 );
		}
		// Number of points,
		//fprintf(outfile, "%ld\n", points->items);
	}
	else
	{
		// Allocate space for 'point2tetlist'.
		out->point2tetlist = new int[ points->items ];
		if(out->point2tetlist == (int *)NULL)
		{
			terminatetetgen( this, 1 );
		}
	}

	// The list of tetrahedra must be indexed.
	if(bgm != NULL)
	{
		bgm->indexelements( );
	}
	// Determine the first index (0 or 1).
	int firstindex = b->zeroindex ? 0 : in->firstnumber;
	int pointindex = firstindex;
	i = 0;

	triface parenttet;
	points->traversalinit( );
	ptloop = pointtraverse( );
	while(ptloop != (point)NULL)
	{
		if(bgm != NULL)
		{
			bgm->decode( point2bgmtet( ptloop ), parenttet );
		}
		else
		{
			decode( point2tet( ptloop ), parenttet );
		}
		if(out == (tetgenio *)NULL)
		{
			fprintf( outfile, "%d  %d\n", pointindex, elemindex( parenttet.tet ) );
		}
		else
		{
			out->point2tetlist[ i ] = elemindex( parenttet.tet );
		}
		pointindex++;
		i++;
		ptloop = pointtraverse( );
	}

	if(out == (tetgenio *)NULL)
	{
		fprintf( outfile, "# Generated by %s\n", b->commandline );
		fclose( outfile );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// outelements()    Output the tetrahedra to an .ele file or a tetgenio      //
//                  structure.                                               //
//                                                                           //
// This routine also indexes all tetrahedra (exclusing hull tets) (from in-> //
// firstnumber). The total number of mesh edges is counted in 'meshedges'.   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::outelements( tetgenio* out )
{
	FILE *outfile = NULL;
	char outelefilename[ FILENAMESIZE ];
	tetrahedron* tptr;
	point p1, p2, p3, p4;
	point *extralist;
	REAL *talist = NULL;
	int *tlist = NULL;
	long ntets;
	int firstindex, shift;
	int pointindex, attribindex;
	int highorderindex = 11;
	int elementnumber;
	int eextras;
	int i;

	if(out == (tetgenio *)NULL)
	{
		strcpy( outelefilename, b->outfilename );
		strcat( outelefilename, ".ele" );
	}

	if(!b->quiet)
	{
		if(out == (tetgenio *)NULL)
		{
			myprintf( "Writing %s.\n", outelefilename );
		}
		else
		{
			myprintf( "Writing elements.\n" );
		}
	}

	// The number of tets excluding hull tets.
	ntets = tetrahedrons->items - hullsize;

	eextras = numelemattrib;
	if(out == (tetgenio *)NULL)
	{
		outfile = fopen( outelefilename, "w" );
		if(outfile == (FILE *)NULL)
		{
			myprintf( "File I/O Error:  Cannot create file %s.\n", outelefilename );
			terminatetetgen( this, 1 );
		}
		// Number of tetras, points per tetra, attributes per tetra.
		fprintf( outfile, "%ld  %d  %d\n", ntets, b->order == 1 ? 4 : 10, eextras );
	}
	else
	{
		// Allocate memory for output tetrahedra.
		out->tetrahedronlist = new int[ ntets * ( b->order == 1 ? 4 : 10 ) ];
		if(out->tetrahedronlist == (int *)NULL)
		{
			myprintf( "Error:  Out of memory.\n" );
			terminatetetgen( this, 1 );
		}
		// Allocate memory for output tetrahedron attributes if necessary.
		if(eextras > 0)
		{
			out->tetrahedronattributelist = new REAL[ ntets * eextras ];
			if(out->tetrahedronattributelist == (REAL *)NULL)
			{
				myprintf( "Error:  Out of memory.\n" );
				terminatetetgen( this, 1 );
			}
		}
		out->numberoftetrahedra = ntets;
		out->numberofcorners = b->order == 1 ? 4 : 10;
		out->numberoftetrahedronattributes = eextras;
		tlist = out->tetrahedronlist;
		talist = out->tetrahedronattributelist;
		pointindex = 0;
		attribindex = 0;
	}

	// Determine the first index (0 or 1).
	firstindex = b->zeroindex ? 0 : in->firstnumber;
	shift = 0; // Default no shift.
	if(( in->firstnumber == 1 ) && ( firstindex == 0 ))
	{
		shift = 1; // Shift the output indices by 1.
	}

	tetrahedrons->traversalinit( );
	tptr = tetrahedrontraverse( );
	elementnumber = firstindex; // in->firstnumber;
	while(tptr != (tetrahedron *)NULL)
	{
		if(!b->reversetetori)
		{
			p1 = (point)tptr[ 4 ];
			p2 = (point)tptr[ 5 ];
		}
		else
		{
			p1 = (point)tptr[ 5 ];
			p2 = (point)tptr[ 4 ];
		}
		p3 = (point)tptr[ 6 ];
		p4 = (point)tptr[ 7 ];
		if(out == (tetgenio *)NULL)
		{
			// Tetrahedron number, indices for four points.
			fprintf( outfile, "%5d   %5d %5d %5d %5d", elementnumber,
					 pointmark( p1 ) - shift, pointmark( p2 ) - shift,
					 pointmark( p3 ) - shift, pointmark( p4 ) - shift );
			if(b->order == 2)
			{
				extralist = (point *)tptr[ highorderindex ];
				// indices for six extra points.
				fprintf( outfile, "  %5d %5d %5d %5d %5d %5d",
						 pointmark( extralist[ 0 ] ) - shift, pointmark( extralist[ 1 ] ) - shift,
						 pointmark( extralist[ 2 ] ) - shift, pointmark( extralist[ 3 ] ) - shift,
						 pointmark( extralist[ 4 ] ) - shift, pointmark( extralist[ 5 ] ) - shift );
			}
			for(i = 0; i < eextras; i++)
			{
				fprintf( outfile, "    %.17g", elemattribute( tptr, i ) );
			}
			fprintf( outfile, "\n" );
		}
		else
		{
			tlist[ pointindex++ ] = pointmark( p1 ) - shift;
			tlist[ pointindex++ ] = pointmark( p2 ) - shift;
			tlist[ pointindex++ ] = pointmark( p3 ) - shift;
			tlist[ pointindex++ ] = pointmark( p4 ) - shift;
			if(b->order == 2)
			{
				extralist = (point *)tptr[ highorderindex ];
				tlist[ pointindex++ ] = pointmark( extralist[ 0 ] ) - shift;
				tlist[ pointindex++ ] = pointmark( extralist[ 1 ] ) - shift;
				tlist[ pointindex++ ] = pointmark( extralist[ 2 ] ) - shift;
				tlist[ pointindex++ ] = pointmark( extralist[ 3 ] ) - shift;
				tlist[ pointindex++ ] = pointmark( extralist[ 4 ] ) - shift;
				tlist[ pointindex++ ] = pointmark( extralist[ 5 ] ) - shift;
			}
			for(i = 0; i < eextras; i++)
			{
				talist[ attribindex++ ] = elemattribute( tptr, i );
			}
		}
		// Remember the index of this element (for counting edges).
		setelemindex( tptr, elementnumber );
		if(b->metric)
		{ // -m option
			// Update the point-to-tet map, so that every point is pointing
			//   to a real tet, not a fictious one. Used by .p2t file.
			for(int i = 0; i < 4; i++)
			{
				setpoint2tet( (point)( tptr[ 4 + i ] ), (tetrahedron)tptr );
			}
		}
		tptr = tetrahedrontraverse( );
		elementnumber++;
	}


	if(out == (tetgenio *)NULL)
	{
		fprintf( outfile, "# Generated by %s\n", b->commandline );
		fclose( outfile );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// outfaces()    Output all faces to a .face file or a tetgenio object.      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::outfaces( tetgenio* out )
{
	FILE *outfile = NULL;
	char facefilename[ FILENAMESIZE ];
	triface tface, tsymface;
	face checkmark;
	point torg, tdest, tapex;
	long ntets, faces;
	int *elist = NULL, *emlist = NULL;
	int neigh1 = 0, neigh2 = 0;
	int marker = 0;
	int firstindex, shift;
	int facenumber;
	int index = 0;

	// For -o2 option.
	triface workface;
	point *extralist, pp[ 3 ] = { 0, 0, 0 };
	int highorderindex = 11;
	int o2index = 0, i;

	// For -nn option.
	int *tet2facelist = NULL;
	int tidx;

	if(out == (tetgenio *)NULL)
	{
		strcpy( facefilename, b->outfilename );
		strcat( facefilename, ".face" );
	}

	if(!b->quiet)
	{
		if(out == (tetgenio *)NULL)
		{
			myprintf( "Writing %s.\n", facefilename );
		}
		else
		{
			myprintf( "Writing faces.\n" );
		}
	}

	ntets = tetrahedrons->items - hullsize;
	faces = ( ntets * 4l + hullsize ) / 2l;

	if(out == (tetgenio *)NULL)
	{
		outfile = fopen( facefilename, "w" );
		if(outfile == (FILE *)NULL)
		{
			myprintf( "File I/O Error:  Cannot create file %s.\n", facefilename );
			terminatetetgen( this, 1 );
		}
		fprintf( outfile, "%ld  %d\n", faces, !b->nobound );
	}
	else
	{
		// Allocate memory for 'trifacelist'.
		out->trifacelist = new int[ faces * 3 ];
		if(out->trifacelist == (int *)NULL)
		{
			myprintf( "Error:  Out of memory.\n" );
			terminatetetgen( this, 1 );
		}
		if(b->order == 2)
		{
			out->o2facelist = new int[ faces * 3 ];
		}
		// Allocate memory for 'trifacemarkerlist' if necessary.
		if(!b->nobound)
		{
			out->trifacemarkerlist = new int[ faces ];
			if(out->trifacemarkerlist == (int *)NULL)
			{
				myprintf( "Error:  Out of memory.\n" );
				terminatetetgen( this, 1 );
			}
		}
		if(b->neighout > 1)
		{
			// '-nn' switch.
			out->face2tetlist = new int[ faces * 2 ];
			if(out->face2tetlist == (int *)NULL)
			{
				myprintf( "Error:  Out of memory.\n" );
				terminatetetgen( this, 1 );
			}
		}
		out->numberoftrifaces = faces;
		elist = out->trifacelist;
		emlist = out->trifacemarkerlist;
	}

	if(b->neighout > 1)
	{ // -nn option
		// Output the tetrahedron-to-face map.
		tet2facelist = new int[ ntets * 4 ];
	}

	// Determine the first index (0 or 1).
	firstindex = b->zeroindex ? 0 : in->firstnumber;
	shift = 0; // Default no shiftment.
	if(( in->firstnumber == 1 ) && ( firstindex == 0 ))
	{
		shift = 1; // Shift the output indices by 1.
	}

	tetrahedrons->traversalinit( );
	tface.tet = tetrahedrontraverse( );
	facenumber = firstindex; // in->firstnumber;
	// To loop over the set of faces, loop over all tetrahedra, and look at
	//   the four faces of each one. If its adjacent tet is a hull tet,
	//   operate on the face, otherwise, operate on the face only if the
	//   current tet has a smaller index than its neighbor.
	while(tface.tet != (tetrahedron *)NULL)
	{
		for(tface.ver = 0; tface.ver < 4; tface.ver++)
		{
			fsym( tface, tsymface );
			if(ishulltet( tsymface ) ||
				( elemindex( tface.tet ) < elemindex( tsymface.tet ) ))
			{
				torg = org( tface );
				tdest = dest( tface );
				tapex = apex( tface );
				if(b->order == 2)
				{ // -o2
					// Get the three extra vertices on edges.
					extralist = (point *)( tface.tet[ highorderindex ] );
					// The extra vertices are on edges opposite the corners.
					enext( tface, workface );
					for(i = 0; i < 3; i++)
					{
						pp[ i ] = extralist[ ver2edge[ workface.ver ] ];
						enextself( workface );
					}
				}
				if(!b->nobound)
				{
					// Get the boundary marker of this face.
					if(b->plc || b->refine)
					{
						// Shell face is used.
						tspivot( tface, checkmark );
						if(checkmark.sh == NULL)
						{
							marker = 0;  // It is an inner face. It's marker is 0.
						}
						else
						{
							marker = shellmark( checkmark );
						}
					}
					else
					{
						// Shell face is not used, only distinguish outer and inner face.
						marker = (int)ishulltet( tsymface );
					}
				}
				if(b->neighout > 1)
				{
					// '-nn' switch. Output adjacent tets indices.
					if(!ishulltet( tface ))
					{
						neigh1 = elemindex( tface.tet );
					}
					else
					{
						neigh1 = -1;
					}
					if(!ishulltet( tsymface ))
					{
						neigh2 = elemindex( tsymface.tet );
					}
					else
					{
						neigh2 = -1;
					}
					// Fill the tetrahedron-to-face map.
					tidx = elemindex( tface.tet ) - firstindex;
					tet2facelist[ tidx * 4 + tface.ver ] = facenumber;
					if(!ishulltet( tsymface ))
					{
						tidx = elemindex( tsymface.tet ) - firstindex;
						tet2facelist[ tidx * 4 + ( tsymface.ver & 3 ) ] = facenumber;
					}
				}
				if(out == (tetgenio *)NULL)
				{
					// Face number, indices of three vertices.
					fprintf( outfile, "%5d   %4d  %4d  %4d", facenumber,
							 pointmark( torg ) - shift, pointmark( tdest ) - shift,
							 pointmark( tapex ) - shift );
					if(b->order == 2)
					{ // -o2
						fprintf( outfile, "  %4d  %4d  %4d", pointmark( pp[ 0 ] ) - shift,
								 pointmark( pp[ 1 ] ) - shift, pointmark( pp[ 2 ] ) - shift );
					}
					if(!b->nobound)
					{
						// Output a boundary marker.
						fprintf( outfile, "  %d", marker );
					}
					if(b->neighout > 1)
					{
						fprintf( outfile, "    %5d  %5d", neigh1, neigh2 );
					}
					fprintf( outfile, "\n" );
				}
				else
				{
					// Output indices of three vertices.
					elist[ index++ ] = pointmark( torg ) - shift;
					elist[ index++ ] = pointmark( tdest ) - shift;
					elist[ index++ ] = pointmark( tapex ) - shift;
					if(b->order == 2)
					{ // -o2
						out->o2facelist[ o2index++ ] = pointmark( pp[ 0 ] ) - shift;
						out->o2facelist[ o2index++ ] = pointmark( pp[ 1 ] ) - shift;
						out->o2facelist[ o2index++ ] = pointmark( pp[ 2 ] ) - shift;
					}
					if(!b->nobound)
					{
						emlist[ facenumber - in->firstnumber ] = marker;
					}
					if(b->neighout > 1)
					{
						out->face2tetlist[ ( facenumber - in->firstnumber ) * 2 ] = neigh1;
						out->face2tetlist[ ( facenumber - in->firstnumber ) * 2 + 1 ] = neigh2;
					}
				}
				facenumber++;
			}
		}
		tface.tet = tetrahedrontraverse( );
	}

	if(out == (tetgenio *)NULL)
	{
		fprintf( outfile, "# Generated by %s\n", b->commandline );
		fclose( outfile );
	}

	if(b->neighout > 1)
	{ // -nn option
		// Output the tetrahedron-to-face map.
		if(out == (tetgenio *)NULL)
		{
			strcpy( facefilename, b->outfilename );
			strcat( facefilename, ".t2f" );
		}
		if(!b->quiet)
		{
			if(out == (tetgenio *)NULL)
			{
				myprintf( "Writing %s.\n", facefilename );
			}
			else
			{
				myprintf( "Writing tetrahedron-to-face map.\n" );
			}
		}
		if(out == (tetgenio *)NULL)
		{
			outfile = fopen( facefilename, "w" );
			for(tidx = 0; tidx < ntets; tidx++)
			{
				index = tidx * 4;
				fprintf( outfile, "%4d  %d %d %d %d\n", tidx + in->firstnumber,
						 tet2facelist[ index ], tet2facelist[ index + 1 ],
						 tet2facelist[ index + 2 ], tet2facelist[ index + 3 ] );
			}
			fclose( outfile );
			delete[ ] tet2facelist;
		}
		else
		{
			// Simply copy the address of the list to the output.
			out->tet2facelist = tet2facelist;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// outhullfaces()    Output hull faces to a .face file or a tetgenio object. //
//                                                                           //
// The normal of each face is pointing to the outside of the domain.         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::outhullfaces( tetgenio* out )
{
	FILE *outfile = NULL;
	char facefilename[ FILENAMESIZE ];
	triface hulltet;
	point torg, tdest, tapex;
	int *elist = NULL;
	int firstindex, shift;
	int facenumber;
	int index;

	if(out == (tetgenio *)NULL)
	{
		strcpy( facefilename, b->outfilename );
		strcat( facefilename, ".face" );
	}

	if(!b->quiet)
	{
		if(out == (tetgenio *)NULL)
		{
			myprintf( "Writing %s.\n", facefilename );
		}
		else
		{
			myprintf( "Writing faces.\n" );
		}
	}

	if(out == (tetgenio *)NULL)
	{
		outfile = fopen( facefilename, "w" );
		if(outfile == (FILE *)NULL)
		{
			myprintf( "File I/O Error:  Cannot create file %s.\n", facefilename );
			terminatetetgen( this, 1 );
		}
		fprintf( outfile, "%ld  0\n", hullsize );
	}
	else
	{
		// Allocate memory for 'trifacelist'.
		out->trifacelist = new int[ hullsize * 3 ];
		if(out->trifacelist == (int *)NULL)
		{
			myprintf( "Error:  Out of memory.\n" );
			terminatetetgen( this, 1 );
		}
		out->numberoftrifaces = hullsize;
		elist = out->trifacelist;
		index = 0;
	}

	// Determine the first index (0 or 1).
	firstindex = b->zeroindex ? 0 : in->firstnumber;
	shift = 0; // Default no shiftment.
	if(( in->firstnumber == 1 ) && ( firstindex == 0 ))
	{
		shift = 1; // Shift the output indices by 1.
	}

	tetrahedrons->traversalinit( );
	hulltet.tet = alltetrahedrontraverse( );
	facenumber = firstindex;
	while(hulltet.tet != (tetrahedron *)NULL)
	{
		if(ishulltet( hulltet ))
		{
			torg = (point)hulltet.tet[ 4 ];
			tdest = (point)hulltet.tet[ 5 ];
			tapex = (point)hulltet.tet[ 6 ];
			if(out == (tetgenio *)NULL)
			{
				// Face number, indices of three vertices.
				fprintf( outfile, "%5d   %4d  %4d  %4d", facenumber,
						 pointmark( torg ) - shift, pointmark( tdest ) - shift,
						 pointmark( tapex ) - shift );
				fprintf( outfile, "\n" );
			}
			else
			{
				// Output indices of three vertices.
				elist[ index++ ] = pointmark( torg ) - shift;
				elist[ index++ ] = pointmark( tdest ) - shift;
				elist[ index++ ] = pointmark( tapex ) - shift;
			}
			facenumber++;
		}
		hulltet.tet = alltetrahedrontraverse( );
	}

	if(out == (tetgenio *)NULL)
	{
		fprintf( outfile, "# Generated by %s\n", b->commandline );
		fclose( outfile );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// outsubfaces()    Output subfaces (i.e. boundary faces) to a .face file or //
//                  a tetgenio structure.                                    //
//                                                                           //
// The boundary faces are found in 'subfaces'. For listing triangle vertices //
// in the same sense for all triangles in the mesh, the direction determined //
// by right-hand rule is pointer to the inside of the volume.                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::outsubfaces( tetgenio* out )
{
	FILE *outfile = NULL;
	char facefilename[ FILENAMESIZE ];
	int *elist = NULL;
	int *emlist = NULL;
	int index = 0, index1 = 0, index2 = 0;
	triface abuttingtet;
	face faceloop;
	point torg, tdest, tapex;
	int marker = 0;
	int firstindex, shift;
	int neigh1 = 0, neigh2 = 0;
	int facenumber;

	// For -o2 option.
	triface workface;
	point *extralist, pp[ 3 ] = { 0, 0, 0 };
	int highorderindex = 11;
	int o2index = 0, i;

	int t1ver; // used by fsymself()

	if(out == (tetgenio *)NULL)
	{
		strcpy( facefilename, b->outfilename );
		strcat( facefilename, ".face" );
	}

	if(!b->quiet)
	{
		if(out == (tetgenio *)NULL)
		{
			myprintf( "Writing %s.\n", facefilename );
		}
		else
		{
			myprintf( "Writing faces.\n" );
		}
	}

	if(out == (tetgenio *)NULL)
	{
		outfile = fopen( facefilename, "w" );
		if(outfile == (FILE *)NULL)
		{
			myprintf( "File I/O Error:  Cannot create file %s.\n", facefilename );
			terminatetetgen( this, 3 );
		}
		// Number of subfaces.
		fprintf( outfile, "%ld  %d\n", subfaces->items, !b->nobound );
	}
	else
	{
		// Allocate memory for 'trifacelist'.
		out->trifacelist = new int[ subfaces->items * 3 ];
		if(out->trifacelist == (int *)NULL)
		{
			terminatetetgen( this, 1 );
		}
		if(b->order == 2)
		{
			out->o2facelist = new int[ subfaces->items * 3 ];
		}
		if(!b->nobound)
		{
			// Allocate memory for 'trifacemarkerlist'.
			out->trifacemarkerlist = new int[ subfaces->items ];
			if(out->trifacemarkerlist == (int *)NULL)
			{
				terminatetetgen( this, 1 );
			}
		}
		if(b->neighout > 1)
		{
			// '-nn' switch.
			out->face2tetlist = new int[ subfaces->items * 2 ];
			if(out->face2tetlist == (int *)NULL)
			{
				terminatetetgen( this, 1 );
			}
		}
		out->numberoftrifaces = subfaces->items;
		elist = out->trifacelist;
		emlist = out->trifacemarkerlist;
	}

	// Determine the first index (0 or 1).
	firstindex = b->zeroindex ? 0 : in->firstnumber;
	shift = 0; // Default no shiftment.
	if(( in->firstnumber == 1 ) && ( firstindex == 0 ))
	{
		shift = 1; // Shift the output indices by 1.
	}

	subfaces->traversalinit( );
	faceloop.sh = shellfacetraverse( subfaces );
	facenumber = firstindex; // in->firstnumber;
	while(faceloop.sh != (shellface *)NULL)
	{
		stpivot( faceloop, abuttingtet );
		// If there is a tetrahedron containing this subface, orient it so
		//   that the normal of this face points to inside of the volume by
		//   right-hand rule.
		if(abuttingtet.tet != NULL)
		{
			if(ishulltet( abuttingtet ))
			{
				fsymself( abuttingtet );
			}
		}
		if(abuttingtet.tet != NULL)
		{
			torg = org( abuttingtet );
			tdest = dest( abuttingtet );
			tapex = apex( abuttingtet );
			if(b->order == 2)
			{ // -o2
				// Get the three extra vertices on edges.
				extralist = (point *)( abuttingtet.tet[ highorderindex ] );
				workface = abuttingtet;
				for(i = 0; i < 3; i++)
				{
					pp[ i ] = extralist[ ver2edge[ workface.ver ] ];
					enextself( workface );
				}
			}
		}
		else
		{
			// This may happen when only a surface mesh be generated.
			torg = sorg( faceloop );
			tdest = sdest( faceloop );
			tapex = sapex( faceloop );
			if(b->order == 2)
			{ // -o2
				// There is no extra node list available.
				pp[ 0 ] = torg;
				pp[ 1 ] = tdest;
				pp[ 2 ] = tapex;
			}
		}
		if(!b->nobound)
		{
			marker = shellmark( faceloop );
		}
		if(b->neighout > 1)
		{
			// '-nn' switch. Output adjacent tets indices.
			neigh1 = -1;
			neigh2 = -1;
			stpivot( faceloop, abuttingtet );
			if(abuttingtet.tet != NULL)
			{
				if(!ishulltet( abuttingtet ))
				{
					neigh1 = elemindex( abuttingtet.tet );
				}
				fsymself( abuttingtet );
				if(!ishulltet( abuttingtet ))
				{
					neigh2 = elemindex( abuttingtet.tet );
				}
			}
		}
		if(out == (tetgenio *)NULL)
		{
			fprintf( outfile, "%5d   %4d  %4d  %4d", facenumber,
					 pointmark( torg ) - shift, pointmark( tdest ) - shift,
					 pointmark( tapex ) - shift );
			if(b->order == 2)
			{ // -o2
				fprintf( outfile, "  %4d  %4d  %4d", pointmark( pp[ 0 ] ) - shift,
						 pointmark( pp[ 1 ] ) - shift, pointmark( pp[ 2 ] ) - shift );
			}
			if(!b->nobound)
			{
				fprintf( outfile, "    %d", marker );
			}
			if(b->neighout > 1)
			{
				fprintf( outfile, "    %5d  %5d", neigh1, neigh2 );
			}
			fprintf( outfile, "\n" );
		}
		else
		{
			// Output three vertices of this face;
			elist[ index++ ] = pointmark( torg ) - shift;
			elist[ index++ ] = pointmark( tdest ) - shift;
			elist[ index++ ] = pointmark( tapex ) - shift;
			if(b->order == 2)
			{ // -o2
				out->o2facelist[ o2index++ ] = pointmark( pp[ 0 ] ) - shift;
				out->o2facelist[ o2index++ ] = pointmark( pp[ 1 ] ) - shift;
				out->o2facelist[ o2index++ ] = pointmark( pp[ 2 ] ) - shift;
			}
			if(!b->nobound)
			{
				emlist[ index1++ ] = marker;
			}
			if(b->neighout > 1)
			{
				out->face2tetlist[ index2++ ] = neigh1;
				out->face2tetlist[ index2++ ] = neigh2;
			}
		}
		facenumber++;
		faceloop.sh = shellfacetraverse( subfaces );
	}

	if(out == (tetgenio *)NULL)
	{
		fprintf( outfile, "# Generated by %s\n", b->commandline );
		fclose( outfile );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// outedges()    Output all edges to a .edge file or a tetgenio object.      //
//                                                                           //
// Note: This routine must be called after outelements(),  so that the total //
// number of edges 'meshedges' has been counted.                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::outedges( tetgenio* out )
{
	FILE *outfile = NULL;
	char edgefilename[ FILENAMESIZE ];
	triface tetloop, worktet, spintet;
	face checkseg;
	point torg, tdest;
	int ishulledge;
	int firstindex, shift;
	int edgenumber, marker;
	int index = 0, index1 = 0, index2 = 0;
	int t1ver;
	int i;

	// For -o2 option.
	point *extralist, pp = NULL;
	int highorderindex = 11;
	int o2index = 0;

	// For -nn option.
	int *tet2edgelist = NULL;
	int tidx;

	if(out == (tetgenio *)NULL)
	{
		strcpy( edgefilename, b->outfilename );
		strcat( edgefilename, ".edge" );
	}

	if(!b->quiet)
	{
		if(out == (tetgenio *)NULL)
		{
			myprintf( "Writing %s.\n", edgefilename );
		}
		else
		{
			myprintf( "Writing edges.\n" );
		}
	}

	if(meshedges == 0l)
	{
		if(nonconvex)
		{
			numberedges( );  // Count the edges.
		}
		else
		{
			// Use Euler's characteristic to get the numbe of edges.
			// It states V - E + F - C = 1, hence E = V + F - C - 1.
			long tsize = tetrahedrons->items - hullsize;
			long fsize = ( tsize * 4l + hullsize ) / 2l;
			long vsize = points->items - dupverts - unuverts;
			if(b->weighted) vsize -= nonregularcount;
			meshedges = vsize + fsize - tsize - 1;
		}
	}
	meshhulledges = 0l; // It will be counted.

	if(out == (tetgenio *)NULL)
	{
		outfile = fopen( edgefilename, "w" );
		if(outfile == (FILE *)NULL)
		{
			myprintf( "File I/O Error:  Cannot create file %s.\n", edgefilename );
			terminatetetgen( this, 1 );
		}
		// Write the number of edges, boundary markers (0 or 1).
		fprintf( outfile, "%ld  %d\n", meshedges, !b->nobound );
	}
	else
	{
		// Allocate memory for 'edgelist'.
		out->numberofedges = meshedges;
		out->edgelist = new int[ meshedges * 2 ];
		if(out->edgelist == (int *)NULL)
		{
			myprintf( "Error:  Out of memory.\n" );
			terminatetetgen( this, 1 );
		}
		if(b->order == 2)
		{ // -o2 switch
			out->o2edgelist = new int[ meshedges ];
		}
		if(!b->nobound)
		{
			out->edgemarkerlist = new int[ meshedges ];
		}
		if(b->neighout > 1)
		{ // '-nn' switch.
			out->edge2tetlist = new int[ meshedges ];
		}
	}

	if(b->neighout > 1)
	{ // -nn option
		// Output the tetrahedron-to-edge map.
		long tsize = tetrahedrons->items - hullsize;
		tet2edgelist = new int[ tsize * 6 ];
	}

	// Determine the first index (0 or 1).
	firstindex = b->zeroindex ? 0 : in->firstnumber;
	shift = 0; // Default no shiftment.
	if(( in->firstnumber == 1 ) && ( firstindex == 0 ))
	{
		shift = 1; // Shift (reduce) the output indices by 1.
	}

	tetrahedrons->traversalinit( );
	tetloop.tet = tetrahedrontraverse( );
	edgenumber = firstindex; // in->firstnumber;
	while(tetloop.tet != (tetrahedron *)NULL)
	{
		// Count the number of Voronoi faces. 
		worktet.tet = tetloop.tet;
		for(i = 0; i < 6; i++)
		{
			worktet.ver = edge2ver[ i ];
			ishulledge = 0;
			fnext( worktet, spintet );
			do
			{
				if(!ishulltet( spintet ))
				{
					if(elemindex( spintet.tet ) < elemindex( worktet.tet )) break;
				}
				else
				{
					ishulledge = 1;
				}
				fnextself( spintet );
			}
			while(spintet.tet != worktet.tet);
			if(spintet.tet == worktet.tet)
			{
				// Found a new edge.
				if(ishulledge) meshhulledges++;
				torg = org( worktet );
				tdest = dest( worktet );
				if(b->order == 2)
				{ // -o2
					// Get the extra vertex on this edge.
					extralist = (point *)worktet.tet[ highorderindex ];
					pp = extralist[ ver2edge[ worktet.ver ] ];
				}
				if(out == (tetgenio *)NULL)
				{
					fprintf( outfile, "%5d   %4d  %4d", edgenumber,
							 pointmark( torg ) - shift, pointmark( tdest ) - shift );
					if(b->order == 2)
					{ // -o2
						fprintf( outfile, "  %4d", pointmark( pp ) - shift );
					}
				}
				else
				{
					// Output three vertices of this face;
					out->edgelist[ index++ ] = pointmark( torg ) - shift;
					out->edgelist[ index++ ] = pointmark( tdest ) - shift;
					if(b->order == 2)
					{ // -o2
						out->o2edgelist[ o2index++ ] = pointmark( pp ) - shift;
					}
				}
				if(!b->nobound)
				{
					if(b->plc || b->refine)
					{
						// Check if the edge is a segment.
						tsspivot1( worktet, checkseg );
						if(checkseg.sh != NULL)
						{
							marker = shellmark( checkseg );
						}
						else
						{
							marker = 0;  // It's not a segment.
						}
					}
					else
					{
						// Mark it if it is a hull edge.
						marker = ishulledge ? 1 : 0;
					}
					if(out == (tetgenio *)NULL)
					{
						fprintf( outfile, "  %d", marker );
					}
					else
					{
						out->edgemarkerlist[ index1++ ] = marker;
					}
				}
				if(b->neighout > 1)
				{ // '-nn' switch.
					if(out == (tetgenio *)NULL)
					{
						fprintf( outfile, "  %d", elemindex( tetloop.tet ) );
					}
					else
					{
						out->edge2tetlist[ index2++ ] = elemindex( tetloop.tet );
					}
					// Fill the tetrahedron-to-edge map.
					spintet = worktet;
					while(1)
					{
						if(!ishulltet( spintet ))
						{
							tidx = elemindex( spintet.tet ) - firstindex;
							tet2edgelist[ tidx * 6 + ver2edge[ spintet.ver ] ] = edgenumber;
						}
						fnextself( spintet );
						if(spintet.tet == worktet.tet) break;
					}
				}
				if(out == (tetgenio *)NULL)
				{
					fprintf( outfile, "\n" );
				}
				edgenumber++;
			}
		}
		tetloop.tet = tetrahedrontraverse( );
	}

	if(out == (tetgenio *)NULL)
	{
		fprintf( outfile, "# Generated by %s\n", b->commandline );
		fclose( outfile );
	}

	if(b->neighout > 1)
	{ // -nn option
		long tsize = tetrahedrons->items - hullsize;

		if(b->facesout)
		{ // -f option
			// Build the face-to-edge map (use the tet-to-edge map).
			long fsize = ( tsize * 4l + hullsize ) / 2l;
			int *face2edgelist = new int[ fsize * 3 ];

			tetrahedrons->traversalinit( );
			tetloop.tet = tetrahedrontraverse( );
			int facenumber = 0; // firstindex; // in->firstnumber;
			while(tetloop.tet != (tetrahedron *)NULL)
			{
				for(tetloop.ver = 0; tetloop.ver < 4; tetloop.ver++)
				{
					fsym( tetloop, spintet );
					if(ishulltet( spintet ) ||
						( elemindex( tetloop.tet ) < elemindex( spintet.tet ) ))
					{
						// The three edges of this face are ordered such that the
						//    first edge is opposite to the first vertex of this face
						//    that appears in the .face file, and so on.
						tidx = elemindex( tetloop.tet ) - firstindex;
						worktet = tetloop;
						for(i = 0; i < 3; i++)
						{
							enextself( worktet ); // The edge opposite to vertex i.
							int eidx = tet2edgelist[ tidx * 6 + ver2edge[ worktet.ver ] ];
							face2edgelist[ facenumber * 3 + i ] = eidx;
						}
						facenumber++;
					}
				}
				tetloop.tet = tetrahedrontraverse( );
			}

			// Output the face-to-edge map.
			if(out == (tetgenio *)NULL)
			{
				strcpy( edgefilename, b->outfilename );
				strcat( edgefilename, ".f2e" );
			}
			if(!b->quiet)
			{
				if(out == (tetgenio *)NULL)
				{
					myprintf( "Writing %s.\n", edgefilename );
				}
				else
				{
					myprintf( "Writing face-to-edge map.\n" );
				}
			}
			if(out == (tetgenio *)NULL)
			{
				outfile = fopen( edgefilename, "w" );
				for(tidx = 0; tidx < fsize; tidx++)
				{ // Re-use `tidx'
					i = tidx * 3;
					fprintf( outfile, "%4d  %d %d %d\n", tidx + in->firstnumber,
							 face2edgelist[ i ], face2edgelist[ i + 1 ], face2edgelist[ i + 2 ] );
				}
				fclose( outfile );
				delete[ ] face2edgelist;
			}
			else
			{
				// Simply copy the address of the list to the output.
				out->face2edgelist = face2edgelist;
			}
		} // if (b->facesout)

		// Output the tetrahedron-to-edge map.
		if(out == (tetgenio *)NULL)
		{
			strcpy( edgefilename, b->outfilename );
			strcat( edgefilename, ".t2e" );
		}
		if(!b->quiet)
		{
			if(out == (tetgenio *)NULL)
			{
				myprintf( "Writing %s.\n", edgefilename );
			}
			else
			{
				myprintf( "Writing tetrahedron-to-edge map.\n" );
			}
		}
		if(out == (tetgenio *)NULL)
		{
			outfile = fopen( edgefilename, "w" );
			for(tidx = 0; tidx < tsize; tidx++)
			{
				i = tidx * 6;
				fprintf( outfile, "%4d  %d %d %d %d %d %d\n", tidx + in->firstnumber,
						 tet2edgelist[ i ], tet2edgelist[ i + 1 ], tet2edgelist[ i + 2 ],
						 tet2edgelist[ i + 3 ], tet2edgelist[ i + 4 ], tet2edgelist[ i + 5 ] );
			}
			fclose( outfile );
			delete[ ] tet2edgelist;
		}
		else
		{
			// Simply copy the address of the list to the output.
			out->tet2edgelist = tet2edgelist;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// outsubsegments()    Output segments to a .edge file or a structure.       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::outsubsegments( tetgenio* out )
{
	FILE *outfile = NULL;
	char edgefilename[ FILENAMESIZE ];
	int *elist = NULL;
	int index, i;
	face edgeloop;
	point torg, tdest;
	int firstindex, shift;
	int marker;
	int edgenumber;

	// For -o2 option.
	triface workface, spintet;
	point *extralist, pp = NULL;
	int highorderindex = 11;
	int o2index = 0;

	// For -nn option.
	int neigh = -1;
	int index2 = 0;

	int t1ver; // used by fsymself()

	if(out == (tetgenio *)NULL)
	{
		strcpy( edgefilename, b->outfilename );
		strcat( edgefilename, ".edge" );
	}

	if(!b->quiet)
	{
		if(out == (tetgenio *)NULL)
		{
			myprintf( "Writing %s.\n", edgefilename );
		}
		else
		{
			myprintf( "Writing edges.\n" );
		}
	}

	if(out == (tetgenio *)NULL)
	{
		outfile = fopen( edgefilename, "w" );
		if(outfile == (FILE *)NULL)
		{
			myprintf( "File I/O Error:  Cannot create file %s.\n", edgefilename );
			terminatetetgen( this, 3 );
		}
		// Number of subsegments.
		fprintf( outfile, "%ld  1\n", subsegs->items );
	}
	else
	{
		// Allocate memory for 'edgelist'.
		out->edgelist = new int[ subsegs->items * ( b->order == 1 ? 2 : 3 ) ];
		if(out->edgelist == (int *)NULL)
		{
			terminatetetgen( this, 1 );
		}
		if(b->order == 2)
		{
			out->o2edgelist = new int[ subsegs->items ];
		}
		out->edgemarkerlist = new int[ subsegs->items ];
		if(out->edgemarkerlist == (int *)NULL)
		{
			terminatetetgen( this, 1 );
		}
		if(b->neighout > 1)
		{
			out->edge2tetlist = new int[ subsegs->items ];
		}
		out->numberofedges = subsegs->items;
		elist = out->edgelist;
	}

	// Determine the first index (0 or 1).
	firstindex = b->zeroindex ? 0 : in->firstnumber;
	shift = 0; // Default no shiftment.
	if(( in->firstnumber == 1 ) && ( firstindex == 0 ))
	{
		shift = 1; // Shift the output indices by 1.
	}
	index = 0;
	i = 0;

	subsegs->traversalinit( );
	edgeloop.sh = shellfacetraverse( subsegs );
	edgenumber = firstindex; // in->firstnumber;
	while(edgeloop.sh != (shellface *)NULL)
	{
		torg = sorg( edgeloop );
		tdest = sdest( edgeloop );
		if(( b->order == 2 ) || ( b->neighout > 1 ))
		{
			sstpivot1( edgeloop, workface );
			if(workface.tet != NULL)
			{
				// We must find a non-hull tet.
				if(ishulltet( workface ))
				{
					spintet = workface;
					while(1)
					{
						fnextself( spintet );
						if(!ishulltet( spintet )) break;
						if(spintet.tet == workface.tet) break;
					}
					workface = spintet;
				}
			}
		}
		if(b->order == 2)
		{ // -o2
			// Get the extra vertex on this edge.
			if(workface.tet != NULL)
			{
				extralist = (point *)workface.tet[ highorderindex ];
				pp = extralist[ ver2edge[ workface.ver ] ];
			}
			else
			{
				pp = torg; // There is no extra node available.
			}
		}
		if(b->neighout > 1)
		{ // -nn
			if(workface.tet != NULL)
			{
				neigh = elemindex( workface.tet );
			}
			else
			{
				neigh = -1;
			}
		}
		marker = shellmark( edgeloop );
		if(marker == 0)
		{
			marker = 1; // Default marker of a boundary edge is 1. 
		}
		if(out == (tetgenio *)NULL)
		{
			fprintf( outfile, "%5d   %4d  %4d", edgenumber,
					 pointmark( torg ) - shift, pointmark( tdest ) - shift );
			if(b->order == 2)
			{ // -o2
				fprintf( outfile, "  %4d", pointmark( pp ) - shift );
			}
			fprintf( outfile, "  %d", marker );
			if(b->neighout > 1)
			{ // -nn
				fprintf( outfile, "  %4d", neigh );
			}
			fprintf( outfile, "\n" );
		}
		else
		{
			// Output three vertices of this face;
			elist[ index++ ] = pointmark( torg ) - shift;
			elist[ index++ ] = pointmark( tdest ) - shift;
			if(b->order == 2)
			{ // -o2
				out->o2edgelist[ o2index++ ] = pointmark( pp ) - shift;
			}
			out->edgemarkerlist[ i++ ] = marker;
			if(b->neighout > 1)
			{ // -nn
				out->edge2tetlist[ index2++ ] = neigh;
			}
		}
		edgenumber++;
		edgeloop.sh = shellfacetraverse( subsegs );
	}

	if(out == (tetgenio *)NULL)
	{
		fprintf( outfile, "# Generated by %s\n", b->commandline );
		fclose( outfile );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// outneighbors()    Output tet neighbors to a .neigh file or a structure.   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::outneighbors( tetgenio* out )
{
	FILE *outfile = NULL;
	char neighborfilename[ FILENAMESIZE ];
	int *nlist = NULL;
	int index = 0;
	triface tetloop, tetsym;
	int neighbori[ 4 ];
	int firstindex;
	int elementnumber;
	long ntets;

	if(out == (tetgenio *)NULL)
	{
		strcpy( neighborfilename, b->outfilename );
		strcat( neighborfilename, ".neigh" );
	}

	if(!b->quiet)
	{
		if(out == (tetgenio *)NULL)
		{
			myprintf( "Writing %s.\n", neighborfilename );
		}
		else
		{
			myprintf( "Writing neighbors.\n" );
		}
	}

	ntets = tetrahedrons->items - hullsize;

	if(out == (tetgenio *)NULL)
	{
		outfile = fopen( neighborfilename, "w" );
		if(outfile == (FILE *)NULL)
		{
			myprintf( "File I/O Error:  Cannot create file %s.\n", neighborfilename );
			terminatetetgen( this, 1 );
		}
		// Number of tetrahedra, four faces per tetrahedron.
		fprintf( outfile, "%ld  %d\n", ntets, 4 );
	}
	else
	{
		// Allocate memory for 'neighborlist'.
		out->neighborlist = new int[ ntets * 4 ];
		if(out->neighborlist == (int *)NULL)
		{
			myprintf( "Error:  Out of memory.\n" );
			terminatetetgen( this, 1 );
		}
		nlist = out->neighborlist;
	}

	// Determine the first index (0 or 1).
	firstindex = b->zeroindex ? 0 : in->firstnumber;

	tetrahedrons->traversalinit( );
	tetloop.tet = tetrahedrontraverse( );
	elementnumber = firstindex; // in->firstnumber;
	while(tetloop.tet != (tetrahedron *)NULL)
	{
		for(tetloop.ver = 0; tetloop.ver < 4; tetloop.ver++)
		{
			fsym( tetloop, tetsym );
			if(!ishulltet( tetsym ))
			{
				neighbori[ tetloop.ver ] = elemindex( tetsym.tet );
			}
			else
			{
				neighbori[ tetloop.ver ] = -1;
			}
		}
		if(out == (tetgenio *)NULL)
		{
			// Tetrahedra number, neighboring tetrahedron numbers.
			fprintf( outfile, "%4d    %4d  %4d  %4d  %4d\n", elementnumber,
					 neighbori[ 0 ], neighbori[ 1 ], neighbori[ 2 ], neighbori[ 3 ] );
		}
		else
		{
			nlist[ index++ ] = neighbori[ 0 ];
			nlist[ index++ ] = neighbori[ 1 ];
			nlist[ index++ ] = neighbori[ 2 ];
			nlist[ index++ ] = neighbori[ 3 ];
		}
		tetloop.tet = tetrahedrontraverse( );
		elementnumber++;
	}

	if(out == (tetgenio *)NULL)
	{
		fprintf( outfile, "# Generated by %s\n", b->commandline );
		fclose( outfile );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// outvoronoi()    Output the Voronoi diagram to .v.node, .v.edge, v.face,   //
//                 and .v.cell.                                              //
//                                                                           //
// The Voronoi diagram is the geometric dual of the Delaunay triangulation.  //
// The Voronoi vertices are the circumcenters of Delaunay tetrahedra.  Each  //
// Voronoi edge connects two Voronoi vertices at two sides of a common Dela- //
// unay face. At a face of convex hull, it becomes a ray (goto the infinity).//
// A Voronoi face is the convex hull of all Voronoi vertices around a common //
// Delaunay edge. It is a closed polygon for any internal Delaunay edge. At a//
// ridge, it is unbounded.  Each Voronoi cell is the convex hull of all Vor- //
// onoi vertices around a common Delaunay vertex. It is a polytope for any   //
// internal Delaunay vertex. It is an unbounded polyhedron for a Delaunay    //
// vertex belonging to the convex hull.                                      //
//                                                                           //
// NOTE: This routine is only used when the input is only a set of point.    //
// Comment: Special thanks to Victor Liu for finding and fixing few bugs.    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::outvoronoi( tetgenio* out )
{
	FILE *outfile = NULL;
	char outfilename[ FILENAMESIZE ];
	tetgenio::voroedge *vedge = NULL;
	tetgenio::vorofacet *vfacet = NULL;
	arraypool *tetlist, *ptlist;
	triface tetloop, worktet, spintet, firsttet;
	point pt[ 4 ], ploop, neipt;
	REAL ccent[ 3 ], infvec[ 3 ], vec1[ 3 ], vec2[ 3 ], L;
	long ntets, faces, edges;
	int *indexarray, *fidxs, *eidxs;
	int arraysize, *vertarray = NULL;
	int vpointcount, vedgecount, vfacecount, tcount;
	int ishullvert, ishullface;
	int index, shift, end1, end2;
	int i, j;

	int t1ver; // used by fsymself()

	// Output Voronoi vertices to .v.node file.
	if(out == (tetgenio *)NULL)
	{
		strcpy( outfilename, b->outfilename );
		strcat( outfilename, ".v.node" );
	}

	if(!b->quiet)
	{
		if(out == (tetgenio *)NULL)
		{
			myprintf( "Writing %s.\n", outfilename );
		}
		else
		{
			myprintf( "Writing Voronoi vertices.\n" );
		}
	}

	// Determine the first index (0 or 1).
	shift = ( b->zeroindex ? 0 : in->firstnumber );

	// Each face and edge of the tetrahedral mesh will be indexed for indexing
	//   the Voronoi edges and facets. Indices of faces and edges are saved in
	//   each tetrahedron (including hull tets).

	// Allocate the total space once.
	indexarray = new int[ tetrahedrons->items * 10 ];

	// Allocate space (10 integers) into each tetrahedron. It re-uses the slot
	//   for element markers, flags.
	i = 0;
	tetrahedrons->traversalinit( );
	tetloop.tet = alltetrahedrontraverse( );
	while(tetloop.tet != NULL)
	{
		tetloop.tet[ 11 ] = (tetrahedron)&( indexarray[ i * 10 ] );
		i++;
		tetloop.tet = alltetrahedrontraverse( );
	}

	// The number of tetrahedra (excluding hull tets) (Voronoi vertices).
	ntets = tetrahedrons->items - hullsize;
	// The number of Delaunay faces (Voronoi edges).
	faces = ( 4l * ntets + hullsize ) / 2l;
	// The number of Delaunay edges (Voronoi faces).
	long vsize = points->items - dupverts - unuverts;
	if(b->weighted) vsize -= nonregularcount;
	if(!nonconvex)
	{
		edges = vsize + faces - ntets - 1;
	}
	else
	{
		if(meshedges == 0l)
		{
			numberedges( ); // Count edges.
		}
		edges = meshedges;
	}

	if(out == (tetgenio *)NULL)
	{
		outfile = fopen( outfilename, "w" );
		if(outfile == (FILE *)NULL)
		{
			myprintf( "File I/O Error:  Cannot create file %s.\n", outfilename );
			terminatetetgen( this, 3 );
		}
		// Number of voronoi points, 3 dim, no attributes, no marker.
		fprintf( outfile, "%ld  3  0  0\n", ntets );
	}
	else
	{
		// Allocate space for 'vpointlist'.
		out->numberofvpoints = (int)ntets;
		out->vpointlist = new REAL[ out->numberofvpoints * 3 ];
		if(out->vpointlist == (REAL *)NULL)
		{
			terminatetetgen( this, 1 );
		}
	}

	// Output Voronoi vertices (the circumcenters of tetrahedra). 
	tetrahedrons->traversalinit( );
	tetloop.tet = tetrahedrontraverse( );
	vpointcount = 0; // The (internal) v-index always starts from 0. 
	index = 0;
	while(tetloop.tet != (tetrahedron *)NULL)
	{
		for(i = 0; i < 4; i++)
		{
			pt[ i ] = (point)tetloop.tet[ 4 + i ];
			setpoint2tet( pt[ i ], encode( tetloop ) );
		}
		if(b->weighted)
		{
			orthosphere( pt[ 0 ], pt[ 1 ], pt[ 2 ], pt[ 3 ], pt[ 0 ][ 3 ], pt[ 1 ][ 3 ], pt[ 2 ][ 3 ],
						 pt[ 3 ][ 3 ], ccent, NULL );
		}
		else
		{
			circumsphere( pt[ 0 ], pt[ 1 ], pt[ 2 ], pt[ 3 ], ccent, NULL );
		}
		if(out == (tetgenio *)NULL)
		{
			fprintf( outfile, "%4d  %16.8e %16.8e %16.8e\n", vpointcount + shift,
					 ccent[ 0 ], ccent[ 1 ], ccent[ 2 ] );
		}
		else
		{
			out->vpointlist[ index++ ] = ccent[ 0 ];
			out->vpointlist[ index++ ] = ccent[ 1 ];
			out->vpointlist[ index++ ] = ccent[ 2 ];
		}
		setelemindex( tetloop.tet, vpointcount );
		vpointcount++;
		tetloop.tet = tetrahedrontraverse( );
	}

	if(out == (tetgenio *)NULL)
	{
		fprintf( outfile, "# Generated by %s\n", b->commandline );
		fclose( outfile );
	}

	// Output Voronoi edges to .v.edge file.
	if(out == (tetgenio *)NULL)
	{
		strcpy( outfilename, b->outfilename );
		strcat( outfilename, ".v.edge" );
	}

	if(!b->quiet)
	{
		if(out == (tetgenio *)NULL)
		{
			myprintf( "Writing %s.\n", outfilename );
		}
		else
		{
			myprintf( "Writing Voronoi edges.\n" );
		}
	}

	if(out == (tetgenio *)NULL)
	{
		outfile = fopen( outfilename, "w" );
		if(outfile == (FILE *)NULL)
		{
			myprintf( "File I/O Error:  Cannot create file %s.\n", outfilename );
			terminatetetgen( this, 3 );
		}
		// Number of Voronoi edges, no marker.
		fprintf( outfile, "%ld  0\n", faces );
	}
	else
	{
		// Allocate space for 'vpointlist'.
		out->numberofvedges = (int)faces;
		out->vedgelist = new tetgenio::voroedge[ out->numberofvedges ];
	}

	// Output the Voronoi edges. 
	tetrahedrons->traversalinit( );
	tetloop.tet = tetrahedrontraverse( );
	vedgecount = 0; // D-Face (V-edge) index (from zero).
	index = 0; // The Delaunay-face index.
	while(tetloop.tet != (tetrahedron *)NULL)
	{
		// Count the number of Voronoi edges. Look at the four faces of each
		//   tetrahedron. Count the face if the tetrahedron's index is
		//   smaller than its neighbor's or the neighbor is outside.
		end1 = elemindex( tetloop.tet );
		for(tetloop.ver = 0; tetloop.ver < 4; tetloop.ver++)
		{
			fsym( tetloop, worktet );
			if(ishulltet( worktet ) ||
				( elemindex( tetloop.tet ) < elemindex( worktet.tet ) ))
			{
				// Found a Voronoi edge. Operate on it.
				if(out == (tetgenio *)NULL)
				{
					fprintf( outfile, "%4d  %4d", vedgecount + shift, end1 + shift );
				}
				else
				{
					vedge = &( out->vedgelist[ index++ ] );
					vedge->v1 = end1 + shift;
				}
				if(!ishulltet( worktet ))
				{
					end2 = elemindex( worktet.tet );
				}
				else
				{
					end2 = -1;
				}
				// Note that end2 may be -1 (worktet.tet is outside).
				if(end2 == -1)
				{
					// Calculate the out normal of this hull face.
					pt[ 0 ] = dest( worktet );
					pt[ 1 ] = org( worktet );
					pt[ 2 ] = apex( worktet );
					for(j = 0; j < 3; j++) vec1[ j ] = pt[ 1 ][ j ] - pt[ 0 ][ j ];
					for(j = 0; j < 3; j++) vec2[ j ] = pt[ 2 ][ j ] - pt[ 0 ][ j ];
					cross( vec1, vec2, infvec );
					// Normalize it.
					L = sqrt( infvec[ 0 ] * infvec[ 0 ] + infvec[ 1 ] * infvec[ 1 ]
							  + infvec[ 2 ] * infvec[ 2 ] );
					if(L > 0) for(j = 0; j < 3; j++) infvec[ j ] /= L;
					if(out == (tetgenio *)NULL)
					{
						fprintf( outfile, " -1" );
						fprintf( outfile, " %g %g %g\n", infvec[ 0 ], infvec[ 1 ], infvec[ 2 ] );
					}
					else
					{
						vedge->v2 = -1;
						vedge->vnormal[ 0 ] = infvec[ 0 ];
						vedge->vnormal[ 1 ] = infvec[ 1 ];
						vedge->vnormal[ 2 ] = infvec[ 2 ];
					}
				}
				else
				{
					if(out == (tetgenio *)NULL)
					{
						fprintf( outfile, " %4d\n", end2 + shift );
					}
					else
					{
						vedge->v2 = end2 + shift;
						vedge->vnormal[ 0 ] = 0.0;
						vedge->vnormal[ 1 ] = 0.0;
						vedge->vnormal[ 2 ] = 0.0;
					}
				}
				// Save the V-edge index in this tet and its neighbor.
				fidxs = (int *)( tetloop.tet[ 11 ] );
				fidxs[ tetloop.ver ] = vedgecount;
				fidxs = (int *)( worktet.tet[ 11 ] );
				fidxs[ worktet.ver & 3 ] = vedgecount;
				vedgecount++;
			}
		} // tetloop.ver
		tetloop.tet = tetrahedrontraverse( );
	}

	if(out == (tetgenio *)NULL)
	{
		fprintf( outfile, "# Generated by %s\n", b->commandline );
		fclose( outfile );
	}

	// Output Voronoi faces to .v.face file.
	if(out == (tetgenio *)NULL)
	{
		strcpy( outfilename, b->outfilename );
		strcat( outfilename, ".v.face" );
	}

	if(!b->quiet)
	{
		if(out == (tetgenio *)NULL)
		{
			myprintf( "Writing %s.\n", outfilename );
		}
		else
		{
			myprintf( "Writing Voronoi faces.\n" );
		}
	}

	if(out == (tetgenio *)NULL)
	{
		outfile = fopen( outfilename, "w" );
		if(outfile == (FILE *)NULL)
		{
			myprintf( "File I/O Error:  Cannot create file %s.\n", outfilename );
			terminatetetgen( this, 3 );
		}
		// Number of Voronoi faces.
		fprintf( outfile, "%ld  0\n", edges );
	}
	else
	{
		out->numberofvfacets = edges;
		out->vfacetlist = new tetgenio::vorofacet[ out->numberofvfacets ];
		if(out->vfacetlist == ( tetgenio::vorofacet * ) NULL)
		{
			terminatetetgen( this, 1 );
		}
	}

	// Output the Voronoi facets.
	tetrahedrons->traversalinit( );
	tetloop.tet = tetrahedrontraverse( );
	vfacecount = 0; // D-edge (V-facet) index (from zero).
	while(tetloop.tet != (tetrahedron *)NULL)
	{
		// Count the number of Voronoi faces. Look at the six edges of each
		//   tetrahedron. Count the edge only if the tetrahedron's index is
		//   smaller than those of all other tetrahedra that share the edge.
		worktet.tet = tetloop.tet;
		for(i = 0; i < 6; i++)
		{
			worktet.ver = edge2ver[ i ];
			// Count the number of faces at this edge. If the edge is a hull edge,
			//   the face containing dummypoint is also counted.
			//ishulledge = 0; // Is it a hull edge.
			tcount = 0;
			firsttet = worktet;
			spintet = worktet;
			while(1)
			{
				tcount++;
				fnextself( spintet );
				if(spintet.tet == worktet.tet) break;
				if(!ishulltet( spintet ))
				{
					if(elemindex( spintet.tet ) < elemindex( worktet.tet )) break;
				}
				else
				{
					//ishulledge = 1;
					if(apex( spintet ) == dummypoint)
					{
						// We make this V-edge appear in the end of the edge list.
						fnext( spintet, firsttet );
					}
				}
			} // while (1)
			if(spintet.tet == worktet.tet)
			{
				// Found a Voronoi facet. Operate on it.
				pt[ 0 ] = org( worktet );
				pt[ 1 ] = dest( worktet );
				end1 = pointmark( pt[ 0 ] ) - in->firstnumber; // V-cell index
				end2 = pointmark( pt[ 1 ] ) - in->firstnumber;
				if(out == (tetgenio *)NULL)
				{
					fprintf( outfile, "%4d  %4d %4d  %-2d ", vfacecount + shift,
							 end1 + shift, end2 + shift, tcount );
				}
				else
				{
					vfacet = &( out->vfacetlist[ vfacecount ] );
					vfacet->c1 = end1 + shift;
					vfacet->c2 = end2 + shift;
					vfacet->elist = new int[ tcount + 1 ];
					vfacet->elist[ 0 ] = tcount;
					index = 1;
				}
				// Output V-edges of this V-facet.
				spintet = firsttet; //worktet;
				while(1)
				{
					fidxs = (int *)( spintet.tet[ 11 ] );
					if(apex( spintet ) != dummypoint)
					{
						vedgecount = fidxs[ spintet.ver & 3 ];
						ishullface = 0;
					}
					else
					{
						ishullface = 1; // It's not a real face.
					}
					if(out == (tetgenio *)NULL)
					{
						fprintf( outfile, " %d", !ishullface ? ( vedgecount + shift ) : -1 );
					}
					else
					{
						vfacet->elist[ index++ ] = !ishullface ? ( vedgecount + shift ) : -1;
					}
					// Save the V-facet index in this tet at this edge.
					eidxs = &( fidxs[ 4 ] );
					eidxs[ ver2edge[ spintet.ver ] ] = vfacecount;
					// Go to the next face.
					fnextself( spintet );
					if(spintet.tet == firsttet.tet) break;
				} // while (1)
				if(out == (tetgenio *)NULL)
				{
					fprintf( outfile, "\n" );
				}
				vfacecount++;
			} // if (spintet.tet == worktet.tet)
		} // if (i = 0; i < 6; i++)
		tetloop.tet = tetrahedrontraverse( );
	}

	if(out == (tetgenio *)NULL)
	{
		fprintf( outfile, "# Generated by %s\n", b->commandline );
		fclose( outfile );
	}

	// Output Voronoi cells to .v.cell file.
	if(out == (tetgenio *)NULL)
	{
		strcpy( outfilename, b->outfilename );
		strcat( outfilename, ".v.cell" );
	}

	if(!b->quiet)
	{
		if(out == (tetgenio *)NULL)
		{
			myprintf( "Writing %s.\n", outfilename );
		}
		else
		{
			myprintf( "Writing Voronoi cells.\n" );
		}
	}

	if(out == (tetgenio *)NULL)
	{
		outfile = fopen( outfilename, "w" );
		if(outfile == (FILE *)NULL)
		{
			myprintf( "File I/O Error:  Cannot create file %s.\n", outfilename );
			terminatetetgen( this, 3 );
		}
		// Number of Voronoi cells.
		fprintf( outfile, "%ld\n", points->items - unuverts - dupverts );
	}
	else
	{
		out->numberofvcells = points->items - unuverts - dupverts;
		out->vcelllist = new int*[ out->numberofvcells ];
		if(out->vcelllist == (int **)NULL)
		{
			terminatetetgen( this, 1 );
		}
	}

	// Output Voronoi cells.
	tetlist = cavetetlist;
	ptlist = cavetetvertlist;
	points->traversalinit( );
	ploop = pointtraverse( );
	vpointcount = 0;
	while(ploop != (point)NULL)
	{
		if(( pointtype( ploop ) != UNUSEDVERTEX ) &&
			( pointtype( ploop ) != DUPLICATEDVERTEX ) &&
			( pointtype( ploop ) != NREGULARVERTEX ))
		{
			getvertexstar( 1, ploop, tetlist, ptlist, NULL );
			// Mark all vertices. Check if it is a hull vertex.
			ishullvert = 0;
			for(i = 0; i < ptlist->objects; i++)
			{
				neipt = *(point *)fastlookup( ptlist, i );
				if(neipt != dummypoint)
				{
					pinfect( neipt );
				}
				else
				{
					ishullvert = 1;
				}
			}
			tcount = (int)ptlist->objects;
			if(out == (tetgenio *)NULL)
			{
				fprintf( outfile, "%4d  %-2d ", vpointcount + shift, tcount );
			}
			else
			{
				arraysize = tcount;
				vertarray = new int[ arraysize + 1 ];
				out->vcelllist[ vpointcount ] = vertarray;
				vertarray[ 0 ] = tcount;
				index = 1;
			}
			// List Voronoi facets bounding this cell.
			for(i = 0; i < tetlist->objects; i++)
			{
				worktet = *(triface *)fastlookup( tetlist, i );
				// Let 'worktet' be [a,b,c,d] where d = ploop.
				for(j = 0; j < 3; j++)
				{
					neipt = org( worktet ); // neipt is a, or b, or c
					// Skip the dummypoint.
					if(neipt != dummypoint)
					{
						if(pinfected( neipt ))
						{
							// It's not processed yet.
							puninfect( neipt );
							// Go to the DT edge [a,d], or [b,d], or [c,d]. 
							esym( worktet, spintet );
							enextself( spintet );
							// Get the V-face dual to this edge.
							eidxs = (int *)spintet.tet[ 11 ];
							vfacecount = eidxs[ 4 + ver2edge[ spintet.ver ] ];
							if(out == (tetgenio *)NULL)
							{
								fprintf( outfile, " %d", vfacecount + shift );
							}
							else
							{
								vertarray[ index++ ] = vfacecount + shift;
							}
						}
					}
					enextself( worktet );
				} // j
			} // i
			if(ishullvert)
			{
				// Add a hull facet (-1) to the facet list.
				if(out == (tetgenio *)NULL)
				{
					fprintf( outfile, " -1" );
				}
				else
				{
					vertarray[ index++ ] = -1;
				}
			}
			if(out == (tetgenio *)NULL)
			{
				fprintf( outfile, "\n" );
			}
			tetlist->restart( );
			ptlist->restart( );
			vpointcount++;
		}
		ploop = pointtraverse( );
	}

	// Delete the space for face/edge indices.
	delete[ ] indexarray;

	if(out == (tetgenio *)NULL)
	{
		fprintf( outfile, "# Generated by %s\n", b->commandline );
		fclose( outfile );
	}
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// outsmesh()    Write surface mesh to a .smesh file, which can be read and  //
//               tetrahedralized by TetGen.                                  //
//                                                                           //
// You can specify a filename (without suffix) in 'smfilename'. If you don't //
// supply a filename (let smfilename be NULL), the default name stored in    //
// 'tetgenbehavior' will be used.                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::outsmesh( char* smfilename )
{
	FILE *outfile;
	char nodfilename[ FILENAMESIZE ];
	char smefilename[ FILENAMESIZE ];
	face faceloop;
	point p1, p2, p3;
	int firstindex, shift;
	int bmark;
	int marker;
	int i;

	if(smfilename != (char *)NULL && smfilename[ 0 ] != '\0')
	{
		strcpy( smefilename, smfilename );
	}
	else if(b->outfilename[ 0 ] != '\0')
	{
		strcpy( smefilename, b->outfilename );
	}
	else
	{
		strcpy( smefilename, "unnamed" );
	}
	strcpy( nodfilename, smefilename );
	strcat( smefilename, ".smesh" );
	strcat( nodfilename, ".node" );

	if(!b->quiet)
	{
		myprintf( "Writing %s.\n", smefilename );
	}
	outfile = fopen( smefilename, "w" );
	if(outfile == (FILE *)NULL)
	{
		myprintf( "File I/O Error:  Cannot create file %s.\n", smefilename );
		return;
	}

	// Determine the first index (0 or 1).
	firstindex = b->zeroindex ? 0 : in->firstnumber;
	shift = 0; // Default no shiftment.
	if(( in->firstnumber == 1 ) && ( firstindex == 0 ))
	{
		shift = 1; // Shift the output indices by 1.
	}

	fprintf( outfile, "# %s.  TetGen's input file.\n", smefilename );
	fprintf( outfile, "\n# part 1: node list.\n" );
	fprintf( outfile, "0  3  0  0  # nodes are found in %s.\n", nodfilename );

	marker = 0; // avoid compile warning.
	bmark = !b->nobound && ( in->facetmarkerlist || in->trifacemarkerlist );

	fprintf( outfile, "\n# part 2: facet list.\n" );
	// Number of facets, boundary marker.
	fprintf( outfile, "%ld  %d\n", subfaces->items, bmark );

	subfaces->traversalinit( );
	faceloop.sh = shellfacetraverse( subfaces );
	while(faceloop.sh != (shellface *)NULL)
	{
		p1 = sorg( faceloop );
		p2 = sdest( faceloop );
		p3 = sapex( faceloop );
		if(bmark)
		{
			marker = shellmark( faceloop );
		}
		fprintf( outfile, "3    %4d  %4d  %4d", pointmark( p1 ) - shift,
				 pointmark( p2 ) - shift, pointmark( p3 ) - shift );
		if(bmark)
		{
			fprintf( outfile, "    %d", marker );
		}
		fprintf( outfile, "\n" );
		faceloop.sh = shellfacetraverse( subfaces );
	}

	// Copy input holelist.
	fprintf( outfile, "\n# part 3: hole list.\n" );
	fprintf( outfile, "%d\n", in->numberofholes );
	for(i = 0; i < in->numberofholes; i++)
	{
		fprintf( outfile, "%d  %g  %g  %g\n", i + in->firstnumber,
				 in->holelist[ i * 3 ], in->holelist[ i * 3 + 1 ],
				 in->holelist[ i * 3 + 2 ] );
	}

	// Copy input regionlist.
	fprintf( outfile, "\n# part 4: region list.\n" );
	fprintf( outfile, "%d\n", in->numberofregions );
	for(i = 0; i < in->numberofregions; i++)
	{
		fprintf( outfile, "%d  %g  %g  %g  %d  %g\n", i + in->firstnumber,
				 in->regionlist[ i * 5 ], in->regionlist[ i * 5 + 1 ],
				 in->regionlist[ i * 5 + 2 ], (int)in->regionlist[ i * 5 + 3 ],
				 in->regionlist[ i * 5 + 4 ] );
	}

	fprintf( outfile, "# Generated by %s\n", b->commandline );
	fclose( outfile );
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// outmesh2medit()    Write mesh to a .mesh file, which can be read and      //
//                    rendered by Medit (a free mesh viewer from INRIA).     //
//                                                                           //
// You can specify a filename (without suffix) in 'mfilename'.  If you don't //
// supply a filename (let mfilename be NULL), the default name stored in     //
// 'tetgenbehavior' will be used. The output file will have the suffix .mesh.//
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::outmesh2medit( char* mfilename )
{
	FILE *outfile;
	char mefilename[ FILENAMESIZE ];
	tetrahedron* tetptr;
	triface tface, tsymface;
	face segloop, checkmark;
	point ptloop, p1, p2, p3, p4;
	long ntets, faces;
	int pointnumber;
	int marker;
	int i;

	if(mfilename != (char *)NULL && mfilename[ 0 ] != '\0')
	{
		strcpy( mefilename, mfilename );
	}
	else if(b->outfilename[ 0 ] != '\0')
	{
		strcpy( mefilename, b->outfilename );
	}
	else
	{
		strcpy( mefilename, "unnamed" );
	}
	strcat( mefilename, ".mesh" );

	if(!b->quiet)
	{
		myprintf( "Writing %s.\n", mefilename );
	}
	outfile = fopen( mefilename, "w" );
	if(outfile == (FILE *)NULL)
	{
		myprintf( "File I/O Error:  Cannot create file %s.\n", mefilename );
		return;
	}

	fprintf( outfile, "MeshVersionFormatted 1\n" );
	fprintf( outfile, "\n" );
	fprintf( outfile, "Dimension\n" );
	fprintf( outfile, "3\n" );
	fprintf( outfile, "\n" );

	fprintf( outfile, "\n# Set of mesh vertices\n" );
	fprintf( outfile, "Vertices\n" );
	fprintf( outfile, "%ld\n", points->items );

	points->traversalinit( );
	ptloop = pointtraverse( );
	pointnumber = 1;                        // Medit need start number form 1.
	while(ptloop != (point)NULL)
	{
		// Point coordinates.
		fprintf( outfile, "%.17g  %.17g  %.17g", ptloop[ 0 ], ptloop[ 1 ], ptloop[ 2 ] );
		if(in->numberofpointattributes > 0)
		{
			// Write an attribute, ignore others if more than one.
			fprintf( outfile, "  %.17g\n", ptloop[ 3 ] );
		}
		else
		{
			fprintf( outfile, "    0\n" );
		}
		setpointmark( ptloop, pointnumber );
		ptloop = pointtraverse( );
		pointnumber++;
	}

	// Compute the number of faces.
	ntets = tetrahedrons->items - hullsize;
	faces = ( ntets * 4l + hullsize ) / 2l;

	fprintf( outfile, "\n# Set of Triangles\n" );
	fprintf( outfile, "Triangles\n" );
	fprintf( outfile, "%ld\n", faces );

	tetrahedrons->traversalinit( );
	tface.tet = tetrahedrontraverse( );
	while(tface.tet != (tetrahedron *)NULL)
	{
		for(tface.ver = 0; tface.ver < 4; tface.ver++)
		{
			fsym( tface, tsymface );
			if(ishulltet( tsymface ) ||
				( elemindex( tface.tet ) < elemindex( tsymface.tet ) ))
			{
				p1 = org( tface );
				p2 = dest( tface );
				p3 = apex( tface );
				fprintf( outfile, "%5d  %5d  %5d",
						 pointmark( p1 ), pointmark( p2 ), pointmark( p3 ) );
				// Check if it is a subface.
				tspivot( tface, checkmark );
				if(checkmark.sh == NULL)
				{
					marker = 0;  // It is an inner face. It's marker is 0.
				}
				else
				{
					marker = shellmark( checkmark );
				}
				fprintf( outfile, "    %d\n", marker );
			}
		}
		tface.tet = tetrahedrontraverse( );
	}

	fprintf( outfile, "\n# Set of Tetrahedra\n" );
	fprintf( outfile, "Tetrahedra\n" );
	fprintf( outfile, "%ld\n", ntets );

	tetrahedrons->traversalinit( );
	tetptr = tetrahedrontraverse( );
	while(tetptr != (tetrahedron *)NULL)
	{
		if(!b->reversetetori)
		{
			p1 = (point)tetptr[ 4 ];
			p2 = (point)tetptr[ 5 ];
		}
		else
		{
			p1 = (point)tetptr[ 5 ];
			p2 = (point)tetptr[ 4 ];
		}
		p3 = (point)tetptr[ 6 ];
		p4 = (point)tetptr[ 7 ];
		fprintf( outfile, "%5d  %5d  %5d  %5d",
				 pointmark( p1 ), pointmark( p2 ), pointmark( p3 ), pointmark( p4 ) );
		if(numelemattrib > 0)
		{
			fprintf( outfile, "  %.17g", elemattribute( tetptr, 0 ) );
		}
		else
		{
			fprintf( outfile, "  0" );
		}
		fprintf( outfile, "\n" );
		tetptr = tetrahedrontraverse( );
	}

	fprintf( outfile, "\nCorners\n" );
	fprintf( outfile, "%d\n", in->numberofpoints );

	for(i = 0; i < in->numberofpoints; i++)
	{
		fprintf( outfile, "%4d\n", i + 1 );
	}

	if(b->plc || b->refine)
	{
		fprintf( outfile, "\nEdges\n" );
		fprintf( outfile, "%ld\n", subsegs->items );

		subsegs->traversalinit( );
		segloop.sh = shellfacetraverse( subsegs );
		while(segloop.sh != (shellface *)NULL)
		{
			p1 = sorg( segloop );
			p2 = sdest( segloop );
			fprintf( outfile, "%5d  %5d", pointmark( p1 ), pointmark( p2 ) );
			marker = shellmark( segloop );
			fprintf( outfile, "    %d\n", marker );
			segloop.sh = shellfacetraverse( subsegs );
		}
	}

	fprintf( outfile, "\nEnd\n" );
	fclose( outfile );
}





///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// outmesh2vtk()    Save mesh to file in VTK Legacy format.                  //
//                                                                           //
// This function was contributed by Bryn Llyod from ETH, 2007.               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetgenmesh::outmesh2vtk( char* ofilename )
{
	FILE *outfile;
	char vtkfilename[ FILENAMESIZE ];
	point pointloop, p1, p2, p3, p4;
	tetrahedron* tptr;
	double x, y, z;
	int n1, n2, n3, n4;
	int nnodes = 4;
	int celltype = 10;

	if(b->order == 2)
	{
		myprintf( "  Write VTK not implemented for order 2 elements \n" );
		return;
	}

	int NEL = tetrahedrons->items - hullsize;
	int NN = points->items;

	if(ofilename != (char *)NULL && ofilename[ 0 ] != '\0')
	{
		strcpy( vtkfilename, ofilename );
	}
	else if(b->outfilename[ 0 ] != '\0')
	{
		strcpy( vtkfilename, b->outfilename );
	}
	else
	{
		strcpy( vtkfilename, "unnamed" );
	}
	strcat( vtkfilename, ".vtk" );

	if(!b->quiet)
	{
		myprintf( "Writing %s.\n", vtkfilename );
	}
	outfile = fopen( vtkfilename, "w" );
	if(outfile == (FILE *)NULL)
	{
		myprintf( "File I/O Error:  Cannot create file %s.\n", vtkfilename );
		return;
	}

	//always write big endian
	//bool ImALittleEndian = !testIsBigEndian();

	fprintf( outfile, "# vtk DataFile Version 2.0\n" );
	fprintf( outfile, "Unstructured Grid\n" );
	fprintf( outfile, "ASCII\n" ); // BINARY
	fprintf( outfile, "DATASET UNSTRUCTURED_GRID\n" );
	fprintf( outfile, "POINTS %d double\n", NN );

	points->traversalinit( );
	pointloop = pointtraverse( );
	for(int id = 0; id<NN && pointloop != (point)NULL; id++)
	{
		x = pointloop[ 0 ];
		y = pointloop[ 1 ];
		z = pointloop[ 2 ];
		fprintf( outfile, "%.17g %.17g %.17g\n", x, y, z );
		pointloop = pointtraverse( );
	}
	fprintf( outfile, "\n" );

	fprintf( outfile, "CELLS %d %d\n", NEL, NEL*( 4 + 1 ) );
	//NEL rows, each has 1 type id + 4 node id's

	tetrahedrons->traversalinit( );
	tptr = tetrahedrontraverse( );
	//elementnumber = firstindex; // in->firstnumber;
	while(tptr != (tetrahedron *)NULL)
	{
		if(!b->reversetetori)
		{
			p1 = (point)tptr[ 4 ];
			p2 = (point)tptr[ 5 ];
		}
		else
		{
			p1 = (point)tptr[ 5 ];
			p2 = (point)tptr[ 4 ];
		}
		p3 = (point)tptr[ 6 ];
		p4 = (point)tptr[ 7 ];
		n1 = pointmark( p1 ) - in->firstnumber;
		n2 = pointmark( p2 ) - in->firstnumber;
		n3 = pointmark( p3 ) - in->firstnumber;
		n4 = pointmark( p4 ) - in->firstnumber;
		fprintf( outfile, "%d  %4d %4d %4d %4d\n", nnodes, n1, n2, n3, n4 );
		tptr = tetrahedrontraverse( );
	}
	fprintf( outfile, "\n" );

	fprintf( outfile, "CELL_TYPES %d\n", NEL );
	for(int tid = 0; tid<NEL; tid++)
	{
		fprintf( outfile, "%d\n", celltype );
	}
	fprintf( outfile, "\n" );

	if(numelemattrib > 0)
	{
		// Output tetrahedra region attributes.
		fprintf( outfile, "CELL_DATA %d\n", NEL );
		fprintf( outfile, "SCALARS cell_scalars int 1\n" );
		fprintf( outfile, "LOOKUP_TABLE default\n" );
		tetrahedrons->traversalinit( );
		tptr = tetrahedrontraverse( );
		while(tptr != (tetrahedron *)NULL)
		{
			fprintf( outfile, "%d\n", (int)elemattribute( tptr, numelemattrib - 1 ) );
			tptr = tetrahedrontraverse( );
		}
		fprintf( outfile, "\n" );
	}

	fclose( outfile );
}

////                                                                       ////
////                                                                       ////
//// output_cxx ///////////////////////////////////////////////////////////////

//// main_cxx /////////////////////////////////////////////////////////////////
////                                                                       ////
////                                                                       ////

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// tetrahedralize()    The interface for users using TetGen library to       //
//                     generate tetrahedral meshes with all features.        //
//                                                                           //
// The sequence is roughly as follows.  Many of these steps can be skipped,  //
// depending on the command line switches.                                   //
//                                                                           //
// - Initialize constants and parse the command line.                        //
// - Read the vertices from a file and either                                //
//   - tetrahedralize them (no -r), or                                       //
//   - read an old mesh from files and reconstruct it (-r).                  //
// - Insert the boundary segments and facets (-p or -Y).                     //
// - Read the holes (-p), regional attributes (-pA), and regional volume     //
//   constraints (-pa).  Carve the holes and concavities, and spread the     //
//   regional attributes and volume constraints.                             //
// - Enforce the constraints on minimum quality bound (-q) and maximum       //
//   volume (-a), and a mesh size function (-m).                             //
// - Optimize the mesh wrt. specified quality measures (-O and -o).          //
// - Write the output files and print the statistics.                        //
// - Check the consistency of the mesh (-C).                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetrahedralize( tetgenbehavior *b, tetgenio *in, tetgenio *out,
					 tetgenio *addin, tetgenio *bgmin )
{
	tetgenmesh m;
	clock_t tv[ 12 ], ts[ 5 ]; // Timing informations (defined in time.h)
	REAL cps = (REAL)CLOCKS_PER_SEC;

	tv[ 0 ] = clock( );

	m.b = b;
	m.in = in;
	m.addin = addin;

	if(b->metric && bgmin && ( bgmin->numberofpoints > 0 ))
	{
		m.bgm = new tetgenmesh( ); // Create an empty background mesh.
		m.bgm->b = b;
		m.bgm->in = bgmin;
	}

	m.initializepools( );
	m.transfernodes( );

	exactinit( b->verbose, b->noexact, b->nostaticfilter,
			   m.xmax - m.xmin, m.ymax - m.ymin, m.zmax - m.zmin );

	tv[ 1 ] = clock( );

	if(b->refine)
	{ // -r
		m.reconstructmesh( );
	}
	else
	{ // -p
		m.incrementaldelaunay( ts[ 0 ] );
	}

	tv[ 2 ] = clock( );

	if(!b->quiet)
	{
		if(b->refine)
		{
			myprintf( "Mesh reconstruction seconds:  %g\n", ( (REAL)( tv[ 2 ] - tv[ 1 ] ) ) / cps );
		}
		else
		{
			myprintf( "Delaunay seconds:  %g\n", ( (REAL)( tv[ 2 ] - tv[ 1 ] ) ) / cps );
			if(b->verbose)
			{
				myprintf( "  Point sorting seconds:  %g\n", ( (REAL)( ts[ 0 ] - tv[ 1 ] ) ) / cps );
			}
		}
	}

	if(b->plc && !b->refine)
	{ // -p
		m.meshsurface( );

		ts[ 0 ] = clock( );

		if(!b->quiet)
		{
			myprintf( "Surface mesh seconds:  %g\n", ( (REAL)( ts[ 0 ] - tv[ 2 ] ) ) / cps );
		}

		if(b->diagnose)
		{ // -d
			m.detectinterfaces( );

			ts[ 1 ] = clock( );

			if(!b->quiet)
			{
				myprintf( "Self-intersection seconds:  %g\n", ( (REAL)( ts[ 1 ] - ts[ 0 ] ) ) / cps );
			}

			// Only output when self-intersecting faces exist.
			if(m.subfaces->items > 0l)
			{
				m.outnodes( out );
				m.outsubfaces( out );
			}

			return;
		}
	}

	tv[ 3 ] = clock( );

	if(( b->metric ) && ( m.bgm != NULL ))
	{ // -m
		m.bgm->initializepools( );
		m.bgm->transfernodes( );
		m.bgm->reconstructmesh( );

		ts[ 0 ] = clock( );

		if(!b->quiet)
		{
			myprintf( "Background mesh reconstruct seconds:  %g\n",
					( (REAL)( ts[ 0 ] - tv[ 3 ] ) ) / cps );
		}

		if(b->metric)
		{ // -m
			m.interpolatemeshsize( );

			ts[ 1 ] = clock( );

			if(!b->quiet)
			{
				myprintf( "Size interpolating seconds:  %g\n", ( (REAL)( ts[ 1 ] - ts[ 0 ] ) ) / cps );
			}
		}
	}

	tv[ 4 ] = clock( );

	if(b->plc && !b->refine)
	{ // -p
		if(b->nobisect)
		{ // -Y
			m.recoverboundary( ts[ 0 ] );
		}
		else
		{
			m.constraineddelaunay( ts[ 0 ] );
		}

		ts[ 1 ] = clock( );

		if(!b->quiet)
		{
			if(b->nobisect)
			{
				myprintf( "Boundary recovery " );
			}
			else
			{
				myprintf( "Constrained Delaunay " );
			}
			myprintf( "seconds:  %g\n", ( (REAL)( ts[ 1 ] - tv[ 4 ] ) ) / cps );
			if(b->verbose)
			{
				myprintf( "  Segment recovery seconds:  %g\n", ( (REAL)( ts[ 0 ] - tv[ 4 ] ) ) / cps );
				myprintf( "  Facet recovery seconds:  %g\n", ( (REAL)( ts[ 1 ] - ts[ 0 ] ) ) / cps );
			}
		}

		m.carveholes( );

		ts[ 2 ] = clock( );

		if(!b->quiet)
		{
			myprintf( "Exterior tets removal seconds:  %g\n", ( (REAL)( ts[ 2 ] - ts[ 1 ] ) ) / cps );
		}

		if(b->nobisect)
		{ // -Y
			if(m.subvertstack->objects > 0l)
			{
				m.suppresssteinerpoints( );

				ts[ 3 ] = clock( );

				if(!b->quiet)
				{
					myprintf( "Steiner suppression seconds:  %g\n",
							( (REAL)( ts[ 3 ] - ts[ 2 ] ) ) / cps );
				}
			}
		}
	}

	tv[ 5 ] = clock( );

	if(b->coarsen)
	{ // -R
		m.meshcoarsening( );
	}

	tv[ 6 ] = clock( );

	if(!b->quiet)
	{
		if(b->coarsen)
		{
			myprintf( "Mesh coarsening seconds:  %g\n", ( (REAL)( tv[ 6 ] - tv[ 5 ] ) ) / cps );
		}
	}

	if(( b->plc && b->nobisect ) || b->coarsen)
	{
		m.recoverdelaunay( );
	}

	tv[ 7 ] = clock( );

	if(!b->quiet)
	{
		if(( b->plc && b->nobisect ) || b->coarsen)
		{
			myprintf( "Delaunay recovery seconds:  %g\n", ( (REAL)( tv[ 7 ] - tv[ 6 ] ) ) / cps );
		}
	}

	if(( b->plc || b->refine ) && b->insertaddpoints)
	{ // -i
		if(( addin != NULL ) && ( addin->numberofpoints > 0 ))
		{
			m.insertconstrainedpoints( addin );
		}
	}

	tv[ 8 ] = clock( );

	if(!b->quiet)
	{
		if(( b->plc || b->refine ) && b->insertaddpoints)
		{ // -i
			if(( addin != NULL ) && ( addin->numberofpoints > 0 ))
			{
				myprintf( "Constrained points seconds:  %g\n", ( (REAL)( tv[ 8 ] - tv[ 7 ] ) ) / cps );
			}
		}
	}

	if(b->quality)
	{
		m.delaunayrefinement( );
	}

	tv[ 9 ] = clock( );

	if(!b->quiet)
	{
		if(b->quality)
		{
			myprintf( "Refinement seconds:  %g\n", ( (REAL)( tv[ 9 ] - tv[ 8 ] ) ) / cps );
		}
	}

	if(( b->plc || b->refine ) && ( b->optlevel > 0 ))
	{
		m.optimizemesh( );
	}

	tv[ 10 ] = clock( );

	if(!b->quiet)
	{
		if(( b->plc || b->refine ) && ( b->optlevel > 0 ))
		{
			myprintf( "Optimization seconds:  %g\n", ( (REAL)( tv[ 10 ] - tv[ 9 ] ) ) / cps );
		}
	}

	if(!b->nojettison && ( ( m.dupverts > 0 ) || ( m.unuverts > 0 )
		|| ( b->refine && ( in->numberofcorners == 10 ) ) ))
	{
		m.jettisonnodes( );
	}

	if(( b->order == 2 ) && !b->convex)
	{
		m.highorder( );
	}

	if(!b->quiet)
	{
		myprintf( "\n" );
	}

	if(out != (tetgenio *)NULL)
	{
		out->firstnumber = in->firstnumber;
		out->mesh_dim = in->mesh_dim;
	}

	if(b->nonodewritten || b->noiterationnum)
	{
		if(!b->quiet)
		{
			myprintf( "NOT writing a .node file.\n" );
		}
	}
	else
	{
		m.outnodes( out );
	}

	if(b->noelewritten)
	{
		if(!b->quiet)
		{
			myprintf( "NOT writing an .ele file.\n" );
		}
		m.indexelements( );
	}
	else
	{
		if(m.tetrahedrons->items > 0l)
		{
			m.outelements( out );
		}
	}

	if(b->nofacewritten)
	{
		if(!b->quiet)
		{
			myprintf( "NOT writing an .face file.\n" );
		}
	}
	else
	{
		if(b->facesout)
		{
			if(m.tetrahedrons->items > 0l)
			{
				m.outfaces( out );  // Output all faces.
			}
		}
		else
		{
			if(b->plc || b->refine)
			{
				if(m.subfaces->items > 0l)
				{
					m.outsubfaces( out ); // Output boundary faces.
				}
			}
			else
			{
				if(m.tetrahedrons->items > 0l)
				{
					m.outhullfaces( out ); // Output convex hull faces.
				}
			}
		}
	}


	if(b->nofacewritten)
	{
		if(!b->quiet)
		{
			myprintf( "NOT writing an .edge file.\n" );
		}
	}
	else
	{
		if(b->edgesout)
		{ // -e
			m.outedges( out ); // output all mesh edges. 
		}
		else
		{
			if(b->plc || b->refine)
			{
				m.outsubsegments( out ); // output subsegments.
			}
		}
	}

	if(( b->plc || b->refine ) && b->metric)
	{ // -m
		m.outmetrics( out );
	}

	if(!out && b->plc &&
		( ( b->object == tetgenbehavior::OFF ) ||
		( b->object == tetgenbehavior::PLY ) ||
		( b->object == tetgenbehavior::STL ) ))
	{
		m.outsmesh( b->outfilename );
	}

	if(!out && b->meditview)
	{
		m.outmesh2medit( b->outfilename );
	}


	if(!out && b->vtkview)
	{
		m.outmesh2vtk( b->outfilename );
	}

	if(b->neighout)
	{
		m.outneighbors( out );
	}

	if(b->voroout)
	{
		m.outvoronoi( out );
	}


	tv[ 11 ] = clock( );

	if(!b->quiet)
	{
		myprintf( "\nOutput seconds:  %g\n", ( (REAL)( tv[ 11 ] - tv[ 10 ] ) ) / cps );
		myprintf( "Total running seconds:  %g\n", ( (REAL)( tv[ 11 ] - tv[ 0 ] ) ) / cps );
	}

	if(b->docheck)
	{
		m.checkmesh( 0 );
		if(b->plc || b->refine)
		{
			m.checkshells( );
			m.checksegments( );
		}
		if(b->docheck > 1)
		{
			m.checkdelaunay( );
		}
	}

	if(!b->quiet)
	{
		m.statistics( );
	}
}

#ifndef TETLIBRARY

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// main()    The command line interface of TetGen.                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

int main( int argc, char *argv[ ] )

#else // with TETLIBRARY

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// tetrahedralize()    The library interface of TetGen.                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void tetrahedralize( char *switches, tetgenio *in, tetgenio *out,
					 tetgenio *addin, tetgenio *bgmin )

#endif // not TETLIBRARY

{
	tetgenbehavior b;

#ifndef TETLIBRARY

	tetgenio in, addin, bgmin;

	if(!b.parse_commandline( argc, argv ))
	{
		terminatetetgen( NULL, 10 );
	}

	// Read input files.
	if(b.refine)
	{ // -r
		if(!in.load_tetmesh( b.infilename, (int)b.object ))
		{
			terminatetetgen( NULL, 10 );
		}
	}
	else
	{ // -p
		if(!in.load_plc( b.infilename, (int)b.object ))
		{
			terminatetetgen( NULL, 10 );
		}
	}
	if(b.insertaddpoints)
	{ // -i
		// Try to read a .a.node file.
		addin.load_node( b.addinfilename );
	}
	if(b.metric)
	{ // -m
		// Try to read a background mesh in files .b.node, .b.ele.
		bgmin.load_tetmesh( b.bgmeshfilename, (int)b.object );
	}

	tetrahedralize( &b, &in, NULL, &addin, &bgmin );

	return 0;

#else // with TETLIBRARY

	if(!b.parse_commandline( switches ))
	{
		terminatetetgen( NULL, 10 );
	}
	tetrahedralize( &b, in, out, addin, bgmin );

#endif // not TETLIBRARY
}

////                                                                       ////
////                                                                       ////
//// main_cxx /////////////////////////////////////////////////////////////////

