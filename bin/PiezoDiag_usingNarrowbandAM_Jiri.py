import bench
dsi3pas = bench.App('DSI3PAS')

sample_area = 194

def SetFrequency(freqinHz):
  if ((freqinHz > 20000) and (freqinHz < 99000)):
    dec = 40000000/freqinHz
  else:
    dec = 600
  dsi3pas.set('DSI3Slave.SREGX042[tx_per_h]',dec)
  dsi3pas.set('DSI3Slave.SREGX040[tx_per_l]',dec)
  dsi3pas.run('DSI3Slave.WriteModeRegs',0)

def Measure(par):
  dsi3pas.run('Measurement.Measure',par)

def getSample(slot, index):
  sample = dsi3pas.run('Measurement.getSample',str(slot), str(index))
  return sample

def getSample1_1():
  global sample_area
  return getSample(1,sample_area)

def getSample1_2():
  global sample_area
  return getSample(1,sample_area+1)

def getSample1_3():
  global sample_area
  return getSample(1,sample_area+2)

def getSample1_4():
  global sample_area
  return getSample(1,sample_area+3)

def getSample1_5():
  global sample_area
  return getSample(1,sample_area+4)

def getSample1_6():
  global sample_area
  return getSample(1,sample_area+5)

def getSample1_7():
  global sample_area
  return getSample(1,sample_area+6)

def getSample1_8():
  global sample_area
  return getSample(1,sample_area+7)

def getSample2_1():
  global sample_area
  return getSample(2,sample_area)

def getSample2_2():
  global sample_area
  return getSample(2,sample_area+1)

def getSample2_3():
  global sample_area
  return getSample(2,sample_area+2)

def getSample2_4():
  global sample_area
  return getSample(2,sample_area+3)

def getSample2_5():
  global sample_area
  return getSample(2,sample_area+4)

def getSample2_6():
  global sample_area
  return getSample(2,sample_area+5)

def getSample2_7():
  global sample_area
  return getSample(2,sample_area+6)

def getSample2_8():
  global sample_area
  return getSample(2,sample_area+7)