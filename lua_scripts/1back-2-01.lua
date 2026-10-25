--desc: 
goto_y =function()
   local y
   if CGetBallY() >  0 then
       y=-60
   else
       y=60
   end
   return y
end
away_y =function()
   local y
   if CGetBallY() >  0 then
       y=180
   else
       y=-180
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
  Receiver=task.GotoPos("Receiver",190,goto_y,Kickerdir2Balldir),          
},

["Delay_1"] = {
   switch = function()
      if Cbuf_cnt(true, 60) then                 
         return "Passball_1"
      end
   end,
   Kicker = task.KickerTask("getball"),                                    
   Receiver=task.GotoPos("Receiver",190,goto_y,Receiverdir2Balldir),               
},             
 
["Passball_1"] = {
      switch = function()
          if CIsBallKick("Kicker")  and Cbuf_cnt(true, 10) then
              return "Delay_2"
          end
      end,
      Kicker =task.KickerTask("pass_ball"),
      Receiver=task.GotoPos("Receiver",190,goto_y,Receiverdir2Balldir), 
},--前面都ok

["Delay_2"] = {
   switch = function()
      if  CBall2RoleDist("Receiver")<9 or Cbuf_cnt(true, 100)then --时间注意                
         return "shoot"
      end
   end,
   Kicker = task.GotoPos("Kicker",0,away_y,Kickerdir2Balldir),                                    
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
   Receiver = task.ReceiverTask("mydemo1"),--或者shoot/mydemo1
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

name = "1back-2-01" 
}
