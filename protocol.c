#ifndef PROTOCOL_H
#define	PROTOCOL_H

#define true 1
#define false 0

#define MAX_PACKET_SIZE			1024 * 32

typedef unsigned char PACKET_RAW;

typedef short boolean;

typedef struct {
    PACKET_RAW * m_raw;

    unsigned short length;
    unsigned short read_pos;
} IMProtocol;


//Packet control
//EOP, clear...

static IMProtocol * improtocol_allocate()
{
    IMProtocol * proto = g_new0(IMProtocol, 1);
    proto->read_pos = 0;
    proto->length = 0;
    proto->m_raw = g_new0(PACKET_RAW, MAX_PACKET_SIZE);
    return proto;
}
static boolean improtocol_eop_leng(IMProtocol * packet, int leng) {
    return (packet->read_pos + leng > packet->length);
}

static boolean improtocol_eop(IMProtocol * packet) {
    return improtocol_eop_leng(packet, 1);
}
//Adders

/**
 * Add byte to the end
 * @param packet
 * @param data
 * @return 
 */
static int improtocol_add_byte(IMProtocol * packet, const PACKET_RAW data) {
    packet->m_raw[packet->length++] = data;
    return packet->length;
}

static int improtocol_add_byte_raw(IMProtocol * packet, const PACKET_RAW * data, int leng) {
    int i = 0;
    for (i = 0; i < leng; i++) {
        improtocol_add_byte(packet, data[i]);
    }
}

static void improtocol_add_short(IMProtocol * packet, short number) {
    improtocol_add_byte(packet, (PACKET_RAW) number & 0xFF);
    improtocol_add_byte(packet, (PACKET_RAW) (number >> 8) & 0xFF);
}

static void improtocol_add_integer(IMProtocol * packet, int number) {
    improtocol_add_byte(packet, (PACKET_RAW) number);
    improtocol_add_byte(packet, (PACKET_RAW) (number >> 8) & 0xFF);
    improtocol_add_byte(packet, (PACKET_RAW) (number >> 16) & 0xFF);
    improtocol_add_byte(packet, (PACKET_RAW) (number >> 24) & 0xFF);
}

static void improtocol_add_long(IMProtocol * packet, long number) {
    improtocol_add_byte(packet, (PACKET_RAW) number & 0xFF);
    improtocol_add_byte(packet, (PACKET_RAW) (number >> 8) & 0xFF);
    improtocol_add_byte(packet, (PACKET_RAW) (number >> 16) & 0xFF);
    improtocol_add_byte(packet, (PACKET_RAW) (number >> 24) & 0xFF);
    improtocol_add_byte(packet, (PACKET_RAW) (number >> 32) & 0xFF);
    improtocol_add_byte(packet, (PACKET_RAW) (number >> 40) & 0xFF);
    improtocol_add_byte(packet, (PACKET_RAW) (number >> 48) & 0xFF);
    improtocol_add_byte(packet, (PACKET_RAW) (number >> 56) & 0xFF);
}

static void improtocol_add_string(IMProtocol * packet, char * string) {
    int length = strlen(string);
    
    improtocol_add_integer(packet, length);
    improtocol_add_byte_raw(packet, string, length);
}
//Getters

static PACKET_RAW improtocol_get_byte(IMProtocol * packet) {
    if(!improtocol_eop(packet))
    {
        return packet->m_raw[packet->read_pos++];
    }
    return 0;
}

static int improtocol_get_byte_raw(IMProtocol * packet, PACKET_RAW * raw, int leng) {
    int i = 0;
    for (i = 0; i < leng; i++) {
        raw[i] = improtocol_get_byte(packet);
    }
    return leng;
}

static short improtocol_get_short(IMProtocol * packet) {
    short _short = 0;
    _short ^= ((unsigned short) improtocol_get_byte(packet) & 0xFF);
    _short ^= ((unsigned short) improtocol_get_byte(packet) & 0xFF) << 8;
    return _short;
}

static int improtocol_get_integer(IMProtocol * packet) {
    int _integer = 0;
    _integer = ((unsigned int) improtocol_get_byte(packet) & 0xFF);
    _integer ^= ((unsigned int) improtocol_get_byte(packet) & 0xFF) << 8;
    _integer ^= ((unsigned int) improtocol_get_byte(packet) & 0xFF) << 16;
    _integer ^= ((unsigned int) improtocol_get_byte(packet) & 0xFF) << 24;
    return _integer;
}

static long improtocol_get_long(IMProtocol * packet) {
    long _long;
    _long = improtocol_get_byte(packet) & 0xFF;
    _long ^= ((unsigned long) improtocol_get_byte(packet) & 0xFF) << 8;
    _long ^= ((unsigned long) improtocol_get_byte(packet) & 0xFF) << 16;
    _long ^= ((unsigned long) improtocol_get_byte(packet) & 0xFF) << 24;
    _long ^= ((unsigned long) improtocol_get_byte(packet) & 0xFF) << 32;
    _long ^= ((unsigned long) improtocol_get_byte(packet) & 0xFF) << 40;
    _long ^= ((unsigned long) improtocol_get_byte(packet) & 0xFF) << 48;
    _long ^= ((unsigned long) improtocol_get_byte(packet) & 0xFF) << 56;
    return _long;
}

static boolean improtocol_get_string_raw(IMProtocol * packet, char * raw) {
    if (improtocol_eop_leng(packet, 4)==true) return false;
    int leng = improtocol_get_integer(packet);
    if (improtocol_eop_leng(packet, leng)==true) return false;
    improtocol_get_byte_raw(packet, raw, leng);
    return true;
}

static char *  improtocol_get_string(IMProtocol * packet) {
    
    if (improtocol_eop_leng(packet, 4))
    {
        return false;
    }
    int leng = improtocol_get_integer(packet);
    if(leng == 0) return "";
    if (improtocol_eop_leng(packet, leng))
    {
        return false;
    }
    char * _str = g_new0(char *, leng + 1);
    improtocol_get_byte_raw(packet, _str, leng);
    _str[leng] = '\0';
    return _str;
}

static void improtocol_reset_reader(IMProtocol * packet)
{
    packet->read_pos=0;
}
static void improtocol_clear(IMProtocol * packet) {
    
    int j = 0,i=0;
    for (i = 0; i < packet->length; i++) {
        if (i >= packet->read_pos) {
            packet->m_raw[j++] = packet->m_raw[i];
        }
    }
    packet->length = j;
    packet->read_pos = 0;
}
//Packet debug only
static void improtocol_inspect(IMProtocol * packet)
{
    purple_debug_info("imparty", "Raw leng: %d, raw pointer: %s\n",packet->length,packet->m_raw);
    
    int i = 0;
    for(i = 0;i < packet->length; i++)
    {
        purple_debug_info("imparty", "%d\t%d\n",i,packet->m_raw[i]);
    }
}
#endif