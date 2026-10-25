--desc: 
gPlayTable.CreatePlay{
firstState = "shoot", --起始状态拿球“GetBall”

["shoot"]={
     switch =function()
     if   CIsBallKick("Kicker")  then                     
          return "Finish"
        end
     end,
  Kicker = task.KickerTask("shoot01"),                              
},
["Finish"] ={
   switch = function()
      if Cbuf_cnt(true,120) then
         return "Finish"
      end
   end,
   Kicker  = task.RobotHalt("Kicker"),
   Receiver = task.RobotHalt("Receiver"),
},
name = "kick-penalty01"
} 