#ifndef MESSAGE_H
#define	MESSAGE_H

#include "protocol.c"

enum MESSAGES {
    
     MESSAGE_NOTHING = 0x00,
    /// Comienzo de un mensaje sin encriptar
    MESSAGE_BEGIN = 0x01,
    /// Comienzo de un mensaje encriptado
    MESSAGE_RSA_BEGIN = 0x02,
    /// Ping. Comprobar que el cliente / servidor se encuentran conectados
    MESSAGE_PING = 0x02,
    /// Ping. Comprobar que el cliente / servidor se encuentran conectados
    MESSAGE_PONG = 0x03,
    /// Creacion de Cuentas
    /**
     * Comprueba si este e-mail existe
     *
     * @param mail	String	Correo electronico ha comprobar
     * @return exitst	1|0	Existente o no
     */
    MESSAGE_MAIL_EXITST = 0x09,
    /**
     * Crea una cuenta nueva<br> <b>email</b>	String	Correo electronico<br>
     * <b>pass</b>	String	Contraseña del usuario<br> <b>name</b>	String	Nombre
     * de usuario<br> <b>surname</b>	String	Apellidos del usuario<br> return
     * <b>result</b>	ok|error	Resultado<br> <b>message</b> String	Descripcion
     * del error
     */
    MESSAGE_CREATE_ACCOUNT = 0x10,
    /**
     * Inicia la sesion en el servidor email	String	Correo electronico hash
     * String	Contrase&ntilde,a <b>return</b><br> <b>result</b>	succeed|error
     * Resultado<br> <b>id</b>	String	Id del usuario logeado<br> <b>name</b>
     * String	Nombre del usuario<br> <b>surname</b>	String	Apellidos<br>
     * <b>avatar</b>	String	URL del avatar<br> <b>status</b>	String Estado
     * personalizado<br> <b>message</b> String	Descripcion del error<br>
     */
    MESSAGE_LOGIN = 0x11,
    /**
     * Fuerza el envio de los datos offline
     *
     */
    MESSAGE_OFFLINE_DATA = 0x12,
    /**
     * Retorna la lista de amigos<br> <b>return</b><br> <b>count</b>	int
     * Cantidad de amigos, Base 0<br> <b>id[]</b>	String	Id del usuario<br>
     * <b>name[]</b>	String	Nombre del usuario<br> <b>surname[]</b>	String
     * Apellidos<br> <b>avatar[]</b>	String	URL del avatar<br> <b>status[]</b>
     * String Estado personalizado<br> <b>online[]</b>	0|1 Esta online = 1<br>
     * <b>allowed[]</b>	0|1|2 0 Peticion 1 Permitido 2 Denegado<br>
     * <b>message</b>	Descripcion del error<br>
     */
    MESSAGE_FRIEND_LIST = 0x13,
    /**
     * Indica al cliente que un usuario se a conectado <br> <b>Atencion!</b>
     * Solo el servidor envia este mensaje<br> <b>return</b><br> <b>id</b>	int
     * Id del usuario
     */
    MESSAGE_FRIEND_ONLINE = 0x14,
    /**
     * Indica al cliente que un usuario se a desconectado <br> <b>Atencion!</b>
     * Solo el servidor envia este mensaje<br> <b>return</b><br> <b>id</b>	int
     * Id del usuario
     */
    MESSAGE_FRIEND_OFFLINE = 0x15,
    /**
     * Indica al cliente que un usuario quiere ser tu amigo <br>
     * <b>Atencion!</b> Solo el servidor envia este mensaje<br>
     * <b>return</b><br> <b>id</b>	int	Id del usuario
     */
    MESSAGE_FRIEND_REQUEST = 0x16,
    MESSAGE_FRIEND_INFO = 0x17,
    /**
     * Enviar un mensaje a un usuario <br> <b>Atencion!</b> Solo el servidor
     * envia este mensaje<br> <b>Param</b><br> <b>id</b>	int	Id del usuario ha
     * enviar el mensaje<br> <b>temp_id</b>	int	Id temporal del mensaje que el
     * cliente ha creado en la DB interna. Este numero corresponde con la id del
     * mensaje insertado en la base de datos del servidor.<br> <b>message</b>
     * String Mensaje ha enviar<br> <b>Return</b><br> <b>result</b>	String
     * Resultado: delivered = entregado|succeed = recivido por el
     * servidor|error<br> <b>message</b>	String Descripcion del error
     *
     */
    MESSAGE_SEND_CHAT = 0x20,
    /**
     * Nuevo chat recivido<br> <b>Atencion!</b> Solo el servidor envia este
     * mensaje<br> <b>Return</b><br> <b>message</b>	String Mensaje recivido<br>
     * <b>id</b>	int Id del usuario que lo ha enviado<br> <b>date</b>	long	Fecha
     * en formato UNIX<br> <b>mid</b>	int	Id del mensaje<br>
     *
     */
    MESSAGE_RECV_CHAT = 0x21,
    /**
     * Chat leido<br> <b>Atencion!</b> Este mensaje solo puede ser enviado si el
     * usuario lo desea, en caso contrario podria ser un atento contra su
     * privacidad<br> <b>Param</b><br> <b>mid</b>	int	Id del mensaje<br>
     *
     */
    MESSAGE_READ_CHAT = 0x22,
    MESSAGE_READ_RECV = 0x25,
    /**
     * Usuario esta escribiendo<br> No implementado
     *
     */
    MESSAGE_WRITTING_CHAT = 0x23,
    /**
     * Usuario ha dejado de escribir<br> No implementado
     *
     */
    MESSAGE_NO_WRITTING_CHAT = 0x24,
    /**
     * Cambiar el mensaje de estado<br> <b>Param</b><br> <b>status</b>	String
     * Nuevo estado<br> <b>Return</b><br> <b>result</b>	String	succeed|error<br>
     * <b>message</b>	Descripcion del error<br>
     *
     */
    MESSAGE_STATUS = 0x30,
    /**
     * Un usuario ha cambiado su estado<br> <b>Atencion!</b> Solo el servidor
     * envia este mensaje<br> <b>Return</b><br> <b>id</b>	int Id del usuario que
     * lo ha cambiado<br> <b>status</b>	String	Nuevo estado<br>
     */
    MESSAGE_STATUS_CHANGED = 0x31,
    /**
     * Cambiar la direccion URL del avatar<br> <b>Param</b><br> <b>avatar</b>
     * String	Nueva URL del avatar<br> <b>Return</b><br> <b>result</b>	String
     * succeed|error<br> <b>message</b>	Descripcion del error<br>
     *
     */
    MESSAGE_AVATAR = 0x3a,
    /**
     * Un usuario ha cambiado su avatar<br> <b>Atencion!</b> Solo el servidor
     * envia este mensaje<br> <b>Return</b><br> <b>id</b>	int Id del usuario que
     * lo ha cambiado<br> <b>avatar</b>	String	Nuevo avatar<br>
     */
    MESSAGE_AVATAR_CHANGED = 0x3b,
    /**
     * Buscar a un amigo por nombre, apellido o e-mail<br> <b>Param</b><br>
     * <b>query</b>	String	nombre, apellido o e-mail ha buscar<br>
     * <b>Return</b><br> <b>count</b>	int Cantidad de resultados, Base 0<br>
     * <b>id[]</b>	String	Id del usuario<br> <b>name[]</b>	String	Nombre del
     * usuario<br> <b>surname[]</b>	String	Apellidos<br> <b>avatar[]</b>	String
     * URL del avatar<br> <b>status[]</b>	String Estado personalizado<br>
     * <b>online[]</b>	0|1 Esta online = 1<br> <b>message</b>	Descripcion del
     * error<br>
     *
     */
    MESSAGE_FIND_FRIEND = 0x40,
    /**
     * Solicitar la amistad a un usuario. Hay que esperar que el usuario
     * responda a la peticion.<br> <b>Param</b><br> <b>id</b>	int	ID del usuario
     * que vas a agregar<br> <b>Return</b><br> <b>result</b>	String
     * succeed|error<br> <b>message</b>	Descripcion del error<br>
     *
     */
    MESSAGE_ADD_FRIEND = 0x41,
    /**
     * Aceptar denegar a un usuario.<br> <b>Param</b><br> <b>id</b>	int	ID del
     * usuario que vas a editar<br>> <b>result</b>	0 = Denegado | 1 =
     * Aceptar<br> <b>Return</b><br> <b>result</b>	String	succeed|error<br>
     * <b>message</b>	Descripcion del error<br>
     *
     */
    MESSAGE_ALLOW_FRIEND = 0x42,
    /**
     * Realizar una llamada<br> Param<br> <b>id</b>	int	ID del usuario a
     * llamar<br> <b>Result</b><br> <b>result</b>	String	succeed|error<br>
     * <b>message</b>	Descripcion del error<br>
     */
    MESSAGE_SEND_CALL = 0x4a,
    /**
     * Llamada entrante<br> <b>Atencion!</b> Solo el servidor envia este
     * mensaje<br> <b>Result</b><br> <b>id</b>	int	Id del usuario que esta
     * realizando la llamada
     */
    MESSAGE_INCOMING_CALL = 0x4b,
    /**
     * Responder a la llamada entrante<br> Param<br> <b>result</b>	int	0 =
     * Denegar|1 = Aceptar<br> <b>Result</b><br> <b>result</b>	String
     * succeed|error<br> <b>message</b>	Descripcion del error<br>
     */
    MESSAGE_RESPOND_CALL = 0x4c,
    /**
     * Enviar datos de voz<br> Para porder enviar estos datos antes deve de
     * haberse establecido la comunicación en ambas partes<br> Param<br>
     * <b>data</b>	byte[]	Array con los datos de la grabacion<br>
     * <b>Result</b><br> <b>result</b>	String	succeed|error<br> <b>message</b>
     * Descripcion del error<br>
     */
    MESSAGE_SEND_VOICE = 0x4d,
    /**
     * Recibir datos de voz<br> <b>Atencion!</b> Solo el servidor envia este
     * mensaje<br> Param<br> <b>Result</b><br> <b>id</b>	int	Id del usuario que
     * esta realizando la llamada<br> <b>data</b>	byte[]	Array con los datos de
     * la grabacion<br>
     */
    MESSAGE_RECV_VOICE = 0x4e,
    MESSAGE_CALL = 0x4f,
    MESSAGE_ADMIN = 0x50,
    MESSAGE_RSA = 0x60,
    /**
     * Crear un nuevo grupo<br> Input<br> name	String	Nombre del grupo<br> desc
     * String	Descripcion del grupo<br> mode	0|1	0 = solo el creador puede
     * añadir/eliminar usuarios. 1 = Cualquiera puede entrar<br> Result<br>
     * result	String	succeed|error<br> message	String	Descripcion del error<br>
     */
    MESSAGE_GROUP_CREATE = 0x70,
    /**
     * Listar los grupos en los que participo
     */
    MESSAGE_GROUP_LIST = 0x71,
    /**
     * Enviar Mensaje al grupo Input<br> gid	int	ID del Grupo<br> message	String
     * Mensaje<br>
     */
    MESSAGE_GROUP_SEND = 0x72,
    /**
     * Recivir Mensaje del grupo <b>Atencion!</b> Solo el servidor envia este
     * mensaje<br> Output<br> gid	int	ID del Grupo<br> message	String
     * Mensaje<br> uid	int	ID del usuario<br> fullname	String	nombre del
     * usuario<br> date	int	fecha del mensaje<br>
     */
    MESSAGE_GROUP_RECV = 0x73,
    MESSAGE_GROUP_ADD = 0x74,
    MESSAGE_GROUP_KICK = 0x75,
    MESSAGE_USER_IP_EQUAL = -0x10,
    MESSAGE_USER_JOIN = -0x20,
    MESSAGE_PUSH = 0x7a,
    MESSAGE_PUSH_REGISTRATION = 0x7b,
    MESSAGE_ERROR = -0x30,
    MESSAGE_MAX = -0x49,
    //Admin Commands
    ADMIN_GET_INFO = 0x01,
    ADMIN_GET_TRAFIC = 0x02,
    ADMIN_GET_USERS = 0x03,
    ADMIN_RESTART_SCRIPTS = 0x10,

};

enum VALUE_TYPE {
    VALUE_TYPE_NONE = 0,
    VALUE_TYPE_STRING = 1,
    VALUE_TYPE_BOOLEAN = 2,
    VALUE_TYPE_SHORT = 3,
    VALUE_TYPE_INT = 4,
    VALUE_TYPE_LONG = 5,
    VALUE_TYPE_BLOB = 6
};

typedef struct {
    int type;
    char * key;
    void * data;
    long num_data;
} IMPValue;

typedef struct {
    char message_type;
    IMPValue ** values;
    int values_count;
} IMPMessage;

//#define MESSAGE_DEBUG 

static IMPMessage * impmessage_allocate(char message_type) {
    IMPMessage * m = g_new0(IMPMessage, 1);
    m->message_type = message_type;
    return m;
}

static int impmessage_add_value(IMPMessage * message, IMPValue * value) {
    message->values = realloc(message->values, (message->values_count + 1) * sizeof (IMPValue*));
    message->values[message->values_count++] = value;
    return message->values_count;
}

static IMPValue * impmessage_get_value(IMPMessage * message, char * key) {
    if (!message) return NULL;
    int i = 0;
    for (i = 0; i < message->values_count; i++) {
        if (strcmp(message->values[i]->key, key) == 0) {
            return message->values[i];
        }
    }
    return NULL;
}

static boolean impmessage_has_key(IMPMessage * message, char * key) {
    return ((impmessage_get_value(message, key) != NULL) ? true : false);
}

static IMPValue * impmessage_add_string(IMPMessage * message, const char * key, const char * value) {
    IMPValue * val = g_new0(IMPValue, 1);
    val->key = key;
    val->type = VALUE_TYPE_STRING;
    val->data = (void *) value;
    impmessage_add_value(message, val);
#ifdef MESSAGE_DEBUG
    purple_debug_info("imparty", "< add string : %s >\n", value);
#endif
    return val;
}

static IMPValue * impmessage_add_short(IMPMessage * message, const char * key, short value) {
    IMPValue * val = g_new0(IMPValue, 1);
    val->key = key;
    val->type = VALUE_TYPE_SHORT;
    val->num_data = value;
    impmessage_add_value(message, val);
    return val;
}

static IMPValue * impmessage_add_integer(IMPMessage * message, const char * key, int value) {
    IMPValue * val = g_new0(IMPValue, 1);
    val->key = key,
    val->type = VALUE_TYPE_INT,
    val->num_data = value,
    impmessage_add_value(message, val);
    return val;
}

static IMPValue * impmessage_add_long(IMPMessage * message, const char * key, long value) {
    IMPValue * val = g_new0(IMPValue, 1);
    val->key = key,
    val->type = VALUE_TYPE_LONG,
    val->num_data = value,
    impmessage_add_value(message, val);
    return val;
}
static IMPValue * impmessage_add_boolean(IMPMessage * message, const char * key, int value) {
    IMPValue * val = g_new0(IMPValue, 1);
    val->key = key,
    val->type = VALUE_TYPE_BOOLEAN,
    val->num_data = value,
    impmessage_add_value(message, val);
    return val;
}

static char * impmessage_get_string(IMPMessage * message, const char * key) {
    IMPValue * value = impmessage_get_value(message, key);
    if (value) {
        return (char *) value->data;
    }
    return NULL;
}

static short impmessage_get_short(IMPMessage * message, const char * key) {
    IMPValue * value = impmessage_get_value(message, key);
    if (value != NULL) {
        return (short) value->num_data;
    }
    return 0;
}

static short impmessage_get_integer(IMPMessage * message, const char * key) {
    IMPValue * value = impmessage_get_value(message, key);
    if (value != NULL) {
        return (int) value->num_data;
    }
    return 0;
}

static long impmessage_get_long(IMPMessage * message, const char * key) {
    IMPValue * value = impmessage_get_value(message, key);
    if (value != NULL) {
        return (long) value->num_data;
    }
    return 0;
}
static boolean impmessage_get_boolean(IMPMessage * message, const char * key) {
    IMPValue * value = impmessage_get_value(message, key);
    if (value != NULL) {
        return (boolean) value->num_data;
    }
    return 0;
}

/**
 * Warning!!!!
 * @param message
 * @param proto
 * @return 
 */
static boolean impmessage_load_protocol(IMPMessage * message, IMProtocol * proto) {
    if (proto == NULL) {
        purple_debug_info("imparty", "proto is null\n");
        return false;
    }
    if (message == NULL) {
        purple_debug_info("imparty", "message is null\n");
        return false;
    }
    char * _str;
    improtocol_reset_reader(proto);
    char msgType = 0;

    if (improtocol_eop(proto) == false) {
        msgType = improtocol_get_byte(proto);
        if (msgType > 0) {
            
            char valueType = 0;
            while (true) {
                if(improtocol_eop(proto))
                    return false;
                valueType = improtocol_get_byte(proto);
                if(valueType == 0) break;
                char * key;
                if ((key = improtocol_get_string(proto)) == false) return false;
                switch (valueType) {
                    case VALUE_TYPE_SHORT:
                        if (improtocol_eop_leng(proto, 2)) return false;
                        impmessage_add_short(message, key, improtocol_get_short(proto));
                        break;
                    case VALUE_TYPE_INT:
                        if (improtocol_eop_leng(proto, 4)) return false;
                        impmessage_add_integer(message, key, improtocol_get_integer(proto));
                        break;
                    case VALUE_TYPE_LONG:
                        if (improtocol_eop_leng(proto, 8)) return false;
                        impmessage_add_long(message, key, improtocol_get_long(proto));
                        break;
                    case VALUE_TYPE_STRING:
                        _str = improtocol_get_string(proto);
                        if (_str == false) {
                            return false;
                        }
                        impmessage_add_string(message, key, _str);
                        break;
                    case VALUE_TYPE_BOOLEAN:
                        if (improtocol_eop_leng(proto, 1)) return false;
                        impmessage_add_boolean(message, key, improtocol_get_byte(proto));
                        break;
                }
            }
            if(improtocol_eop(proto))
                return false;
            
            message->message_type = msgType;
            improtocol_clear(proto);
            return true;
        }
        improtocol_clear(proto);
        return false;
    }
    return false;
}

static IMProtocol * impmessage_save_protocol(IMPMessage * message) {
    IMProtocol * proto;
    proto = improtocol_allocate();
    if (message->message_type == 0) return proto;
    improtocol_add_byte(proto, message->message_type);
    int i = 0;
    for (i = 0; i < message->values_count; i++) {
        improtocol_add_byte(proto, message->values[i]->type);
        improtocol_add_string(proto, message->values[i]->key);
        if (message->values[i]->type == VALUE_TYPE_STRING) {

            improtocol_add_string(proto, message->values[i]->data);
        } else if (message->values[i]->type == VALUE_TYPE_SHORT) {
            improtocol_add_short(proto, message->values[i]->num_data);
        } else if (message->values[i]->type == VALUE_TYPE_INT) {
            improtocol_add_integer(proto, message->values[i]->num_data);
        } else if (message->values[i]->type == VALUE_TYPE_LONG) {
            improtocol_add_long(proto, message->values[i]->num_data);
        }
    }
    improtocol_add_byte(proto,0); //No more keys
    improtocol_add_byte(proto,0); //No more messages
    return proto;
}

#endif
