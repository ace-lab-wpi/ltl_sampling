"""LCM type definitions
This file automatically generated by lcm.
DO NOT MODIFY BY HAND!!!!
"""

try:
    import cStringIO.StringIO as BytesIO
except ImportError:
    from io import BytesIO
import struct

class sample_draw(object):
    __slots__ = ["if_draw"]

    def __init__(self):
        self.if_draw = False

    def encode(self):
        buf = BytesIO()
        buf.write(sample_draw._get_packed_fingerprint())
        self._encode_one(buf)
        return buf.getvalue()

    def _encode_one(self, buf):
        buf.write(struct.pack(">b", self.if_draw))

    def decode(data):
        if hasattr(data, 'read'):
            buf = data
        else:
            buf = BytesIO(data)
        if buf.read(8) != sample_draw._get_packed_fingerprint():
            raise ValueError("Decode error")
        return sample_draw._decode_one(buf)
    decode = staticmethod(decode)

    def _decode_one(buf):
        self = sample_draw()
        self.if_draw = bool(struct.unpack('b', buf.read(1))[0])
        return self
    _decode_one = staticmethod(_decode_one)

    _hash = None
    def _get_hash_recursive(parents):
        if sample_draw in parents: return 0
        tmphash = (0x353c2e8252453c2c) & 0xffffffffffffffff
        tmphash  = (((tmphash<<1)&0xffffffffffffffff)  + (tmphash>>63)) & 0xffffffffffffffff
        return tmphash
    _get_hash_recursive = staticmethod(_get_hash_recursive)
    _packed_fingerprint = None

    def _get_packed_fingerprint():
        if sample_draw._packed_fingerprint is None:
            sample_draw._packed_fingerprint = struct.pack(">Q", sample_draw._get_hash_recursive([]))
        return sample_draw._packed_fingerprint
    _get_packed_fingerprint = staticmethod(_get_packed_fingerprint)

