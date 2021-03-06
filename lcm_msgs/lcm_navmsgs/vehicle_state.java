/* LCM type definition class file
 * This file was automatically generated by lcm-gen
 * DO NOT MODIFY BY HAND!!!!
 */

package lcm_navmsgs;
 
import java.io.*;
import java.util.*;
import lcm.lcm.*;
 
public final class vehicle_state implements lcm.lcm.LCMEncodable
{
    public long timestamp;
    public String name;
    public byte id;
    public double position[];
    public double orientation[];
    public double velocity[];
 
    public vehicle_state()
    {
        position = new double[3];
        orientation = new double[4];
        velocity = new double[3];
    }
 
    public static final long LCM_FINGERPRINT;
    public static final long LCM_FINGERPRINT_BASE = 0x34200f922f5af675L;
 
    static {
        LCM_FINGERPRINT = _hashRecursive(new ArrayList<Class<?>>());
    }
 
    public static long _hashRecursive(ArrayList<Class<?>> classes)
    {
        if (classes.contains(lcm_navmsgs.vehicle_state.class))
            return 0L;
 
        classes.add(lcm_navmsgs.vehicle_state.class);
        long hash = LCM_FINGERPRINT_BASE
            ;
        classes.remove(classes.size() - 1);
        return (hash<<1) + ((hash>>63)&1);
    }
 
    public void encode(DataOutput outs) throws IOException
    {
        outs.writeLong(LCM_FINGERPRINT);
        _encodeRecursive(outs);
    }
 
    public void _encodeRecursive(DataOutput outs) throws IOException
    {
        char[] __strbuf = null;
        outs.writeLong(this.timestamp); 
 
        __strbuf = new char[this.name.length()]; this.name.getChars(0, this.name.length(), __strbuf, 0); outs.writeInt(__strbuf.length+1); for (int _i = 0; _i < __strbuf.length; _i++) outs.write(__strbuf[_i]); outs.writeByte(0); 
 
        outs.writeByte(this.id); 
 
        for (int a = 0; a < 3; a++) {
            outs.writeDouble(this.position[a]); 
        }
 
        for (int a = 0; a < 4; a++) {
            outs.writeDouble(this.orientation[a]); 
        }
 
        for (int a = 0; a < 3; a++) {
            outs.writeDouble(this.velocity[a]); 
        }
 
    }
 
    public vehicle_state(byte[] data) throws IOException
    {
        this(new LCMDataInputStream(data));
    }
 
    public vehicle_state(DataInput ins) throws IOException
    {
        if (ins.readLong() != LCM_FINGERPRINT)
            throw new IOException("LCM Decode error: bad fingerprint");
 
        _decodeRecursive(ins);
    }
 
    public static lcm_navmsgs.vehicle_state _decodeRecursiveFactory(DataInput ins) throws IOException
    {
        lcm_navmsgs.vehicle_state o = new lcm_navmsgs.vehicle_state();
        o._decodeRecursive(ins);
        return o;
    }
 
    public void _decodeRecursive(DataInput ins) throws IOException
    {
        char[] __strbuf = null;
        this.timestamp = ins.readLong();
 
        __strbuf = new char[ins.readInt()-1]; for (int _i = 0; _i < __strbuf.length; _i++) __strbuf[_i] = (char) (ins.readByte()&0xff); ins.readByte(); this.name = new String(__strbuf);
 
        this.id = ins.readByte();
 
        this.position = new double[(int) 3];
        for (int a = 0; a < 3; a++) {
            this.position[a] = ins.readDouble();
        }
 
        this.orientation = new double[(int) 4];
        for (int a = 0; a < 4; a++) {
            this.orientation[a] = ins.readDouble();
        }
 
        this.velocity = new double[(int) 3];
        for (int a = 0; a < 3; a++) {
            this.velocity[a] = ins.readDouble();
        }
 
    }
 
    public lcm_navmsgs.vehicle_state copy()
    {
        lcm_navmsgs.vehicle_state outobj = new lcm_navmsgs.vehicle_state();
        outobj.timestamp = this.timestamp;
 
        outobj.name = this.name;
 
        outobj.id = this.id;
 
        outobj.position = new double[(int) 3];
        System.arraycopy(this.position, 0, outobj.position, 0, 3); 
        outobj.orientation = new double[(int) 4];
        System.arraycopy(this.orientation, 0, outobj.orientation, 0, 4); 
        outobj.velocity = new double[(int) 3];
        System.arraycopy(this.velocity, 0, outobj.velocity, 0, 3); 
        return outobj;
    }
 
}

