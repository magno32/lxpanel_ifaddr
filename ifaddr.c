#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib/gi18n.h>

#include <lxpanel/plugin.h>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <sys/types.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <errno.h>

// internal to the plugin source, not used by the 'priv' variable
static int iInstanceCount = 0;

/* the plugin's id – an instance of this struct 
   is what will be assigned to 'priv' */
typedef struct 
{
  gint iMyId;
} IfAddrPlugin;

static int 
ifaddr_constructor(Plugin * p, char ** fp)
{

 /* get the ip... */
struct ifaddrs *ifaddr, *ifa;
    int s;
    char host[NI_MAXHOST];
    int interfaces;
    char MycIdBuf[256] = {'\0'};

    if (getifaddrs(&ifaddr) == -1) 
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    interfaces=0;
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
    {
        if (ifa->ifa_addr == NULL)
            continue;  
        s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
        if (ifa->ifa_addr->sa_family==AF_INET)
                                             interfaces=interfaces+1;
        if(ifa->ifa_addr->sa_family==AF_INET)
        {
            if (s != 0)
            {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
        if((strcmp(ifa->ifa_name,"lo")!=0)&&(ifa->ifa_addr->sa_family==AF_INET))  
            {          
            strcat(MycIdBuf,ifa->ifa_name);
            strcat(MycIdBuf,": ");
            strcat(MycIdBuf,host);
            strcat(MycIdBuf,"\n");
            } 
	    /* break; */
        }
    }


    freeifaddrs(ifaddr);
/* done getting ip... */

 /* fp is a pointer to the configuration file stream
     since we don't use it, we'll make sure it doesn't
     give us an error at compilation time */
 (void)fp;

 // allocate our private structure instance
 IfAddrPlugin *pTest = g_new0(IfAddrPlugin, 1);

 // put it where it belongs
 p->priv = pTest;

 // update the instance count
 pTest->iMyId = ++iInstanceCount;

 // make a label out of the ID
 char cIdBuf[256] = {'\0'};

 /*sprintf(cIdBuf, "%s: %s %d",ifName, ifAddr, interfaces);*/
 strcpy(cIdBuf,MycIdBuf);


 GtkWidget *pLabel = gtk_label_new(cIdBuf);

 // need to create a widget to show
 p->pwid = gtk_event_box_new();

 // set border width
 gtk_container_set_border_width(GTK_CONTAINER(p->pwid), 1);

 // add the label to the container
 gtk_container_add(GTK_CONTAINER(p->pwid), GTK_WIDGET(pLabel));

 // set the size we want
 gtk_widget_set_size_request(p->pwid, 158, 25);

 // our widget doesn't have a window...
 gtk_widget_set_has_window(p->pwid, FALSE);

 // show our widget
 gtk_widget_show_all(p->pwid);

 // success!!!
 return 1;
}
static void 
ifaddr_destructor(Plugin * p)
{
  // decrement our local instance count
  --iInstanceCount;

  // find our private structure instance
  IfAddrPlugin *pTest = (IfAddrPlugin *)p->priv;

  // free it  -- no need to worry about freeing 'pwid', the panel does it
  g_free(pTest);
}
static void ifaddr_configure(Plugin * p, GtkWindow * parent)
{
  // doing nothing here, so make sure neither of the parameters
  // emits a warning at compilation
  (void)p;
  (void)parent;
}

static void ifaddr_save_configuration(Plugin * p, FILE * fp)
{
  // doing nothing here, so make sure neither of the parameters
  // emits a warning at compilation
  (void)p;
  (void)fp;
}

/* Plugin descriptor. */
PluginClass ifaddr_plugin_class = {

   // this is a #define taking care of the size/version variables
   PLUGINCLASS_VERSIONING,

   // type of this plugin
   type : "ifaddr",
   name : N_("Interface Address List"),
   version: "1.0",
   description : N_("Show the IP Address of network interfaces."),

   // we can have many running at the same time
   one_per_system : FALSE,

   // can't expand this plugin
   expand_available : FALSE,

   // assigning our functions to provided pointers.
   constructor : ifaddr_constructor,
   destructor  : ifaddr_destructor,
   config : ifaddr_configure,
   save : ifaddr_save_configuration
};

