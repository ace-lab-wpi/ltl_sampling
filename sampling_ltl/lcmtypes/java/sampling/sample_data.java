/* LCM type definition class file
 * This file was automatically generated by lcm-gen
 * DO NOT MODIFY BY HAND!!!!
 */

package sampling;
 
import java.io.*;
import java.util.*;
import lcm.lcm.*;
 
public final class sample_data implements lcm.lcm.LCMEncodable
{
    public double state[];
 
    public sample_data()
    {
        state = new double[2];
    }
 
    public static final long LCM_FINGERPRINT;
    public static final long LCM_FINGERPRINT_BASE = 0x5c4b2f699a665926L;
 
    static {
        LCM_FINGERPRINT = _hashRecursive(new ArrayList<Class<?>>());
    }
 
    public static long _hashRecursive(ArrayList<Class<?>> classes)
    {
        if (classes.contains(sampling.sample_data.class))
            return 0L;
 
        classes.add(sampling.sample_data.class);
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
        for (int a = 0; a < 2; a++) {
            outs.writeDouble(this.state[a]); 
        }
 
    }
 
    public sample_data(byte[] data) throws IOException
    {
        this(new LCMDataInputStream(data));
    }
 
    public sample_data(DataInput ins) throws IOException
    {
        if (ins.readLong() != LCM_FINGERPRINT)
            throw new IOException("LCM Decode error: bad fingerprint");
 
        _decodeRecursive(ins);
    }
 
    public static sampling.sample_data _decodeRecursiveFactory(DataInput ins) throws IOException
    {
        sampling.sample_data o = new sampling.sample_data();
        o._decodeRecursive(ins);
        return o;
    }
 
    public void _decodeRecursive(DataInput ins) throws IOException
    {
        this.state = new double[(int) 2];
        for (int a = 0; a < 2; a++) {
            this.state[a] = ins.readDouble();
        }
 
    }
 
    public sampling.sample_data copy()
    {
        sampling.sample_data outobj = new sampling.sample_data();
        outobj.state = new double[(int) 2];
        System.arraycopy(this.state, 0, outobj.state, 0, 2); 
        return outobj;
    }
 
}

