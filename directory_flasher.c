/*
 * File : directory_flasher.c
 *
 * Author : Jonathan Sanabria
 *
 * Description : 
 *		An application that requires 
 *
 *		An application that empowers non-coders to build directories by visually representing a tree like structure with GTK entries . The program starts with one empty entry and grows to have one entry beneath it ( a child ), and one to its right on the same level vertically.
 *			ATTENTION :
 *			 This pattern continues each time a user
 *			 fills an empty entry AND presses enter.
 *		If the initial entry is left empty upon closing the application, then no new directories will be made. This is because the directories are automatically made during the closing of the program by reading the tree structure. Any directories/ files on the same vertical level are all children to the first directory which is above and to the left of it. ( any directory above BUT to the right is not considered a "parent" to the child in question. 
 *
 *
 */



#include <gtk/gtk.h>
#include <assert.h>

/* GtkWidget is the storage type for widgets */
GtkWidget *window;
GtkWidget *fixed1; // cur 
gint idx_fix = 0 ;//
static int bytesallocated_by_me;

/* stock (x,y) position for widgets within the
 * given fixed container
*/
gint x = 90;
gint y = 1;
// STOCK WIDTH AND HEIGHT OF WINDOW
gint width = 30 ;
gint height = 10 ;

static int child_ar_count ;
struct child *child_array[1000];
//DEL static int ARRAYSIZE = 1000 ;
//void *xy_array[ 1000 ] ; //[ARRAYSIZE] // holds up to ARRAYSIZE ints*. first 4 bytes xy mean ////// second 4 tag xy range ///// consider smaller range in center of matrix goes towards the front of the array, then those broken up into
//
// DEFINE A3
//
// SORT_ARRAY CODE
// END DEL

static struct head *start ;
static int treenodes ; 
char *listofptrs[1000] ;

static void add_child( struct child **ptr );
static void on_activate_entry_fixed( GtkWidget *widget, struct child *cur);

static GtkWidget *get_and_add_new_entry_to( struct child *cur );
static GtkWidget *get_and_add_new_fixed_to( struct child *cur );

struct child *init_child( int isE , int isF ,GtkWidget *no , struct child *par , struct child *si , struct child *su , int Y , int X ) ;
void init_head( struct head **stock_head );
/*     Head
 *
 * The head structure defines the window
 * which will hold all entries as a child
 */
struct head{
	struct child *begin ;
};

/* 	Child
 *
 * Each child struct carries its own information
 *
 */
struct child{
	GtkWidget *node ;
	GtkWidget *curentry ;
	char prevstring[90] ;
	struct child *parent ;
	struct child *sibling ;
	struct child *sub ;
	int *x, *y, *isEntry, *isFixed ;
	
};

//###############################################################################
//###############################################################################
//#############                 HELPER FUNCTIONS                #################
//###############################################################################
//###############################################################################
/**
 * Initialize the head
 *
 * @param struct head **stock_head
 * @return void
 */ 
void init_head( struct head **stock_head ){
	assert(! start );
	*stock_head = (struct head *)calloc(1, sizeof ( start )); 
	bytesallocated_by_me +=sizeof( start ) ;
	assert( start );
	assert(  treenodes == 0 );
	assert(  start == *stock_head );
	printf( "\t            heap    bytes\nstart->        %p    %d \n",start, sizeof( start )) ;
	printf( "headpointer->  %p     %d \n",*stock_head);
	//assert(0);
//DEL	make_replacement( "window" , 0  , 0  , &(start->begin) ); 
	GtkWidget *entry;
	char tmp[100] ;

	add_child( &(start->begin)  ); 				   
	start->begin = init_child( 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 ); 
	get_and_add_new_fixed_to( 0 );

	//START ENTRY
	sprintf( tmp , "%s", "New Folder" );
	assert( treenodes == 1 );
	//brand new
	entry = get_and_add_new_entry_to( start->begin );
	gtk_entry_set_placeholder_text (GTK_ENTRY ((GtkEntry *)entry), tmp );
							

}



/**
 * Initialize a child
 * Assigns a parent to a child.
 *
 * @param int isE 		
 * @param int isF 		
 * @param GtkWidget *no 	 
 * @param struct child *par 	 
 * @param struct child *si 	 
 * @param struct child *su 	 
 * @param int Y 		 
 * @param int Y 		 
 * @return int
 */ 
struct child *init_child( int isE , int isF ,GtkWidget *no , struct child *par ,struct child *si , struct child *su , int Y , int X )  {
	// ASSIGNS CHILD TO PAR ---- par gets a child initiated in it.
	// MAKES NEW CUR 
	assert( 0 <= treenodes++ );
	printf("init_child->TREENODES_>-->%d\n",treenodes );
	struct child *chi ;
       	chi = calloc( 1, sizeof( struct child ) );// malloc_1
	bytesallocated_by_me += sizeof(chi);// b.c. struct child == 72 
	printf( "\t            heap    stack\nchi            %p    %d \n",chi, sizeof( struct child ));// chi )) ;
		chi->x =  ( int *)calloc( 1, sizeof( int *) ) ;
		bytesallocated_by_me += sizeof(chi->x);
	   	assert(sizeof( chi->x) == sizeof(int *) );
		printf( "chi->x         %p    %d \n",chi->x, sizeof( chi->x )) ;

		chi->y =  ( int *)calloc( 1, sizeof( int *)) ;
		bytesallocated_by_me += sizeof(chi->y);
		assert( sizeof(chi->y)==sizeof(int *) );
		printf( "chi->y         %p    %d \n",chi->y, sizeof( chi->y )) ;

		chi->isEntry =  ( int *)calloc( 1, sizeof( int *) );
		bytesallocated_by_me += sizeof(chi->isEntry );
		assert( sizeof( chi->isEntry) ==sizeof(int *));
		printf( "chi->isEntry   %p    %d \n",chi->isEntry , sizeof( chi->isEntry )) ;

		chi->isFixed =  ( int *)calloc( 1, sizeof( int *) );
		bytesallocated_by_me += sizeof(chi->isFixed );
		assert( sizeof(chi->isFixed) ==sizeof(int *));
		printf( "chi->isFixed   %p    %d \n",chi->isFixed, sizeof( chi->isFixed )) ;
		//assert( 0 );
		chi->node = gtk_fixed_new ();
		chi->curentry = 0 ;  //assert( chi->curentry) ;
		chi->parent = par ;  //assert( chi->parent );
		chi->sibling = si ;  //assert( chi->sibling );
		chi->sub = su ;      //assert( chi->sub );
		*(chi->x) = Y ;        //assert( chi->x );
		*(chi->y) = X ;        //assert( chi->y );
		*(chi->isEntry) = isE ;//assert( chi->isEntry );
		*(chi->isFixed) = isF ;//assert( chi->isFixed );
	add_child( &(chi) );
	printf("init_child->end child added->TREENODES_>-->%d\n",treenodes );
	return chi;
	
}







/**
 * Add a child to the stack based array of pointers which
 * are later free'd.
 *
 * @param struct child **ptr 	Pointer to the current child to input.
 * @return void
 */
static void add_child( struct child **ptr ){
	child_array[ child_ar_count++ ] = *ptr ;
	child_array[ child_ar_count ] = 0 ;
	printf( "childadded *ptr == %p     ptr == %p \n",*ptr , ptr );
}


/**
 * empty( struct child *cur )
 *
 * @param struct child *cur 	The current child to inspect
 * @return int
 *    		1 if cur has an empty entry box
 *    		0 otherwise
 */   
static int empty( struct child *cur ){
	return strlen( gtk_entry_get_text( GTK_ENTRY (cur->curentry))) ==0 ; 
}



/**
 * This is the function that actually makes the directory in the system.
 * The -p flag makes any higherlevel directories which might be needed 
 * to make the final directory in the path. 
 *
 * @param char ar[1000] 	The current path.
 * @return int
 */
static int mkdir( char ar[1000] ){

	char final_path[1000] ; final_path[0] = 0 ;
	strncat( final_path , "mkdir -p ",999);
	strncat( final_path , ar,999 );
	printf( "%s\n",final_path );system( final_path );

	return 0 ;

}
//###############################################################################
//###############################################################################
//#############             END HELPER FUNCTIONS                #################
//###############################################################################
//###############################################################################





/**
 *  
 *  Recursively makes the path to each and every leaf directory.
 *  calls mkdir when :
 *			empty ( sib ) && empty( sub )   --- is leaf
 *
 *			!empty ( sib ) && empty( sub )  --- is a lonesome directory
 *
 * @param struct child *cur 	
 * @param char last_path[1000] 	The last known path.
 * @return int
 */
static int make_all_leaves(  struct child *cur , char last_path[1000] ){
	if( !empty( cur )
	&& ( empty(cur->sibling) && empty( cur->sub) )){

		char *t =(char *)gtk_entry_get_text (GTK_ENTRY (cur->curentry));
		if( strlen( last_path ) != 0 ){
			strcat( last_path, "/") ;
		}
		strcat( last_path, t ) ;
		mkdir( last_path ) ;
	} else if( !empty( cur )
	&&  !empty(cur->sibling) && empty( cur->sub) ){
		char last_path2[1000] ; last_path2[0]=0 ;
		char *t =(char *)gtk_entry_get_text (GTK_ENTRY (cur->curentry));
		strncpy( last_path2, last_path , 999) ;
		if( strlen( last_path2 ) != 0 ){
			strcat( last_path2, "/") ;
		}
		strcat( last_path2, t ) ;
		mkdir( last_path2 ) ;
		make_all_leaves( cur->sibling , last_path );	
	} else if( !empty( cur ) 
	&&  empty(cur->sibling) && !empty( cur->sub) ){
		char *t =(char *)gtk_entry_get_text (GTK_ENTRY (cur->curentry));
		if( strlen( last_path ) != 0 ){
			strcat( last_path, "/") ;
		}
		strcat( last_path, t ) ;
		make_all_leaves( cur->sub , last_path );	
	
	} else if( !empty( cur ) 
	&&  !empty(cur->sibling) && !empty( cur->sub) ){	
		char last_path2[1000] ; last_path2[0]=0 ;
		strncpy( last_path2, last_path , 999) ;
		make_all_leaves( cur->sibling , last_path );	
		char *t =(char *)gtk_entry_get_text (GTK_ENTRY (cur->curentry));
		if( strlen( last_path2 ) != 0 ){
			strcat( last_path2, "/") ;
		}
		strcat( last_path2, t ) ;
		make_all_leaves( cur->sub , last_path2 );	

	}
	return 0 ;

}

/**
 * Initializes the sub and sibling for the current node.
 * before this functions the cur would have been empty
 *
 * @param struct child *cur
 * @return GtkWidget *             return the fixed of the current child
 */ 
static GtkWidget *get_and_add_new_fixed_to( struct child *cur ){
		
	printf( "struct child *cur;");
	if( cur ){
		printf( "get_and_add_new_fixed_to if(cur)->>>> SHOULD BE ^ \n");
		printf( "cur-        >   %p\n",cur);
		//assert(0);
		//makes new sub and sibling fixeds
		cur->sub     = init_child( 0 , 1 , 0 , cur , 0 , 0 , 0 , 30 ); 
		cur->sibling = init_child( 0 , 1 , 0 , cur , 0 , 0 ,  width*4 , 0 ); 
		gtk_fixed_put (GTK_FIXED ((GtkFixed *)cur->node), cur->sub->node , 0 , 30 );
		gtk_fixed_put (GTK_FIXED ((GtkFixed *)cur->node), cur->sibling->node , width*4 , 0 );
		gtk_widget_show ( cur->sub->node );
		gtk_widget_show ( cur->sibling->node );
		
		printf( "cur->sub        %p\n",cur->sub);
		get_and_add_new_entry_to( cur->sub );

		printf( "cur->sibling    %p\n",cur->sibling);
		get_and_add_new_entry_to( cur->sibling );
		//assert(0);
		gtk_widget_show (cur->node);
		return cur->node ;
	}  
	//assert(0);
	assert( treenodes == 1 ); // in first init_child( 0 ) b/c  it returns for start->begin 
	assert( start->begin );//             ^same 
	assert( (start->begin->node) != 0 );//^same 
	gtk_container_add (GTK_CONTAINER ( window ), start->begin->node );
	gtk_widget_show ( window );
	//assert(0);
	gtk_widget_show (start->begin->node);
	return start->begin->node ;
}

/**
 * Adds new entry to the current node.
 *
 * @param struct child *cur
 * @return GtkWidget *             return the entry of the current child
 */ 
static GtkWidget *get_and_add_new_entry_to( struct child *cur ){
		#ifdef TERMINALPRINT
		printf( "in get_and_add_new_entry_to : SHOULD BE ^ \n");
		printf( "headpointer->   %p\n",cur);
		#endif
			cur->curentry = gtk_entry_new ();
			gtk_entry_set_width_chars (GTK_ENTRY (cur->curentry),10);
			gtk_entry_set_max_width_chars(GTK_ENTRY (cur->curentry),width);
			g_signal_connect (cur->curentry,"activate",G_CALLBACK (on_activate_entry_fixed), cur );
			gtk_entry_set_max_length (GTK_ENTRY (cur->curentry), 29);
			gtk_fixed_put (GTK_FIXED ((GtkFixed *)cur->node), cur->curentry, 0 , 0 );
			gtk_widget_show (cur->curentry);
		#ifdef TERMINALPRINT
		printf( "before returning cur->curentry    %p\n",cur->curentry);
		#endif
		return cur->curentry ;
}


//###############################################################################
//###############################################################################
//#############             GUI READJUST FUNCTIONS              #################
//###############################################################################
//###############################################################################
struct child *from_sub_move_all_vertical_sibs( struct child *cur ){
	// FROM A SUB
	while ( cur->parent &&  (cur->parent->sub == cur )){
//				are you a sub ??  yes
//				do you have a par yes
//				then need to scoot par->sib to the right by one
		gtk_fixed_move (GTK_FIXED ((GtkFixed *)cur->parent->node), cur->parent->sibling->node, ( *(cur->parent->sibling->x) += width*4 ) , 0 );
		assert(cur->parent);
		cur = cur->parent ;
	}
	return cur ;

}
struct child *from_sib_move_to_root_sub( struct child *cur ){
	// FROM A SIB
	while ( cur->parent &&  (cur->parent->sub != cur )){
//				are you a sib ??  yes
//				do you have a par yes
//			then need to scoot par->sib to the left by one
		if ( (cur->parent && ( cur->parent->sub == cur ) )){
			if( cur->parent->sibling ){
				gtk_fixed_move (GTK_FIXED ((GtkFixed *)cur->parent->node), cur->parent->sibling->node, ( *(cur->parent->sibling->x) += width*4 ) , 0 );
		
			}
		}
		assert(cur->parent);
		cur = cur->parent ;	
	}
	return cur ;

}

//###############################################################################
//###############################################################################
//#############             CALLBACK    FUNCTIONS               #################
//###############################################################################
//###############################################################################
static void on_activate_entry_fixed( GtkWidget *widget, struct child *cur){
	printf("IN on_activate_entry_fixed \n \n ");
		assert( GTK_ENTRY(widget) );
		const gchar *entry_text;
		entry_text = gtk_entry_get_text (GTK_ENTRY (widget));
		// change children to editable
		assert( strlen( entry_text ) >= 0 );
		if( strlen( entry_text ) ){
			// #############################
			//  HAS TEXT ###################
			printf ("Entry contents: %s\n", entry_text);
			if( treenodes > 1 && cur->parent->prevstring ){ 
				// ############################
				// START HAS CHILD ############
				// make new fixed cur->sub
				// and cur->sibling
				printf( "on_activate treenodes >  1 cur->   %p\n",cur);
				if( strcmp( cur->prevstring , entry_text ) == 0 ){
				//	assert(0);	
					return;
				}else if( ! *(cur->prevstring) ) { 
					//assert(0);	
					get_and_add_new_fixed_to( cur );// makes new full fixed sub and sibling at once
					sprintf(cur->prevstring,"%s", (char *)entry_text) ;
					gtk_entry_set_placeholder_text (GTK_ENTRY ((GtkEntry *)cur->curentry), "on_activate" );
					// fill them with a new entry
					if ( cur->parent
					    &&  (cur->parent->sub == cur )){
					// FROM SUB
					cur = from_sub_move_all_vertical_sibs( cur );
					cur = from_sib_move_to_root_sub( cur );
					from_sub_move_all_vertical_sibs( cur );
					}else if ( cur->parent
						&&  (cur->parent->sub != cur )){
					cur = from_sib_move_to_root_sub( cur );
					from_sub_move_all_vertical_sibs( cur );
					}
				}
				printf( "cur->parent  %p\n",cur->parent );	
				// if it has a parent and is a sub then its parents sibling must move over,
				// if it is a sibling and has siblings then it must move its sibling to the right
				// t	then it must scoot any other parent siblings
				//assert(0);	
			}else if( treenodes == 1 ){
				//dont ask if par
				// make new fixed cur->sub and cur->sibling
				printf( "on_activate treenodes == 1 cur->   %p\n",cur);
				get_and_add_new_fixed_to( cur );// makes new full fixed sub and sibling at once
				sprintf(cur->prevstring,"%s", (char *)entry_text) ;
			}
				
		}
}

//###############################################################################
//###############################################################################
//#############             DESCTRUCTOR FUNCTIONS               #################
//###############################################################################
//###############################################################################
void free_child( struct child *cur ){
	fprintf(stderr," 0 " );
	if( cur && child_ar_count != 0 ){
		fprintf(stderr,"about to free cur== %p    and cur->*\n",cur );
		if( child_ar_count >= 1 ){
			free( cur->x ); fprintf(stderr, " 2 " );  // 
			bytesallocated_by_me -= sizeof(start->begin->x);
			free( cur->y ); fprintf(stderr, " 3 " );  // 
			bytesallocated_by_me -= sizeof(start->begin->y);
			free( cur->isEntry ); fprintf(stderr, " 4 " );  //
			bytesallocated_by_me -= sizeof(start->begin->isEntry );
			free( cur->isFixed ); fprintf(stderr, " 5 " );  //
			bytesallocated_by_me -= sizeof(start->begin->isFixed );
			free( cur ); fprintf(stderr, " 8 " );  //
			bytesallocated_by_me -=sizeof( start->begin) ;
		}
	}
}
void free_tree( struct head *tree ){
	for(  ; 0 <= --child_ar_count ;  ){ 
		printf("\nCOUNT ==> %d\n", child_ar_count );
		free_child( child_array[  child_ar_count ] );
	}
}
static void free_then_quit(GtkWidget *widget ){
	char tmp_ar[1000];
	tmp_ar[0] = 0 ;
	make_all_leaves(  start->begin , tmp_ar );
//	assert( 0 ) ;
	free_tree( start );
	free( start ); fprintf(stderr," 9 " );  
	bytesallocated_by_me -=sizeof( start ) ;
	printf( "INFREEEEEEEEEEEEEEE  leftover bytesallocated_by_me == %d\n", bytesallocated_by_me );
	gtk_main_quit();
}



//###############################################################################
//###############################################################################
//#############             MAIN         MAIN                   #################
//###############################################################################
//###############################################################################
int main( int   argc,
          char *argv[] )
{
	child_ar_count = 0 ;
	bytesallocated_by_me = 0 ;
	assert( sizeof( struct child ) > sizeof( struct child *) );
	printf("struct child == %d ,struct head * == %d\n", sizeof( struct child ), sizeof( struct head *) );
	/* Initialise GTK */
	gtk_init (&argc, &argv);

	/* Create a new window */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size ( GTK_WINDOW (window), width, height);
	gtk_window_set_title (GTK_WINDOW (window), ""); // window header "New Folder"

	/* Here we connect the "destroy" event to a signal handler */ 
	g_signal_connect (window, "destroy",
	    			G_CALLBACK (free_then_quit), NULL);

	/* Sets the border width of the window. */
	gtk_container_set_border_width (GTK_CONTAINER (window), 0);

	init_head( &start ) ;

	/* Display the window */
	gtk_widget_show (window);

	/* Enter the event loop */
	gtk_main ();

	return 0;
}








