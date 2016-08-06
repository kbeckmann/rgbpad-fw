id  = 0
sda = 1
scl = 2

-- initialize i2c, set pin1 as sda, set pin2 as scl
    i2c.setup(id, sda, scl, i2c.SLOW)

-- user defined function: read from reg_addr content of dev_addr
function read_reg(dev_addr, reg_addr, reg_length)
    i2c.start(id)
    i2c.address(id, dev_addr, i2c.TRANSMITTER)
    i2c.write(id, reg_addr)
    i2c.stop(id)
    i2c.start(id)
    i2c.address(id, dev_addr, i2c.RECEIVER)
    c = i2c.read(id, reg_length)
    i2c.stop(id)
    return c
end

-- read from device 0x10, register 0x01, 2 bytes
--reg = read_reg(0x10, 0x01, 2)
--print(reg:byte(0,2))
--print(reg)



function send_data(dev_addr, reg_addr, data)
    i2c.start(id)
    i2c.address(id, dev_addr, i2c.TRANSMITTER)
    i2c.write(id, reg_addr)
    i2c.stop(id)
    i2c.start(id)
    i2c.address(id, dev_addr, i2c.TRANSMITTER)
    i2c.write(id, data)
    i2c.stop(id)
end

function saw(x, max)
    max2 = max*2
    y = x % max2
    if y > max then
        return max2 - y
    else
        return y
    end
end


i=2000
function rainbow()
    i = i + 40
    x = i % 256
    r = saw(i,      255 *  4)  /  4
    g = saw(i + 37, 255 *  7)  /  7
    b = saw(i + 63, 255 * 13)  / 13
    a = 1
    abgr = string.char(a, 0, 0, x)
    send_data(0x10, 0x02, abgr:rep(16))
    abgr = string.char(a, 0, x, 0)
    send_data(0x11, 0x02, abgr:rep(16))
    abgr = string.char(a, x, 0, 0)
    send_data(0x12, 0x02, abgr:rep(16))
    --apa102.write(clk, data, abgr)
    tmr.alarm(1, 10, tmr.ALARM_SINGLE, rainbow)
end

rainbow()

