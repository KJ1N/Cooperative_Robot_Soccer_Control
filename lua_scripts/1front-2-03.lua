--desc: 
goto_y =function()
   local y
   if CGetBallY() >  0 then
       y=60
   else
       y=-60
   end
   return y
end
away_y =function()
   local y
   if CGetBallY() >  0 then
       y=-150
   else
       y=150
   end
   return y
end

local Kickerdir2Balldir = function()
   return CRole2BallDir("Kicker")                
end

local Receiverdir2Balldir = function()
   return CRole2BallDir("Receiver")
end

local Kickerdir2Roledir = function()
   return COurRole2RoleDir("Kicker","Receiver")
end

gPlayTable.CreatePlay{
firstState = "Beginning", 
["Beginning"] = {
   switch = function()
      if CRole2TargetDist("Receiver")<13 and Cbuf_cnt(true, 30) then                 
         return "Delay_1"
      end
   end,            
  Receiver=task.GotoPos("Receiver",190,0,Kickerdir2Balldir),   --GotoPos("Receiver",190,goto_y,Receiverdir2Balldir)       
},

["Delay_1"] = {
   switch = function()
      if Cbuf_cnt(true, 60) then                 
         return "Passball_1"
      end
   end,
   Kicker = task.KickerTask("getball"),                                    
   Receiver=task.GotoPos("Receiver",190,0,Receiverdir2Balldir),   --ReceiverTask("goto01")            
},             
 
["Passball_1"] = {
      switch = function()
          if CIsBallKick("Kicker")   then--and Cbuf_cnt(true, 10)
              return "Delay_2"
          end
      end,

      --Kicker = task.KickerTask("passball02"),--or pass_ball有问题
      Kicker =task.KickerTask("passball_corner_light15"),--_light1
      Receiver=task.GotoPos("Receiver",190,0,Receiverdir2Balldir), 
},--前面都ok

["Delay_2"] = {
   switch = function()
      if  CBall2RoleDist("Receiver")<11 then --时间注意                
         return "shoot"
      end
   end,
   Kicker = task.GotoPos("Kicker",100,0,Kickerdir2Balldir),                                    
  Receiver=task.ReceiverTask("receive"),--拿球(固定原地可能有问题)   receive          
},   
-- Cbuf_cnt(CBall2RoleDist("Receiver")<9, 30) or 
--拿球完成

["shoot"]={
   switch=function()
     if CGetBallX() > 285 and CGetBallY() < 70 and CGetBallY() > -70 then
        return "Finish"
      end
     end,
   Kicker =task.KickerTask("mydemo1"),--或者shoot/mydemo1
   Receiver=task.GotoPos("Receiver",265,150,Receiverdir2Balldir)
},
["Finish"] ={
   switch = function()
      if Cbuf_cnt(true,120) then
         return "Finish"
      end
   end,
   Kicker  = task.RobotHalt("Kicker"),
   Receiver = task.RobotHalt("Receiver"),
   Goalie = task.Goalie()
},

name = "1front-2-03" 
}