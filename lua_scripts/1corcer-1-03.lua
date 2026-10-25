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
      if CRole2TargetDist("Kicker")<13 and Cbuf_cnt(true, 30) then                 
         return "Delay_1"
      end
   end,            
  Kicker=task.GotoPos("Kicker",190,0,Kickerdir2Balldir),
  --Kicker =task.GotoPos("Kicker",280,-170,Kickerdir2Balldir),  --GotoPos("Receiver",190,goto_y,Receiverdir2Balldir)       
},

["Delay_1"] = {
   switch = function()
      if Cbuf_cnt(true, 90) then                 
         return "Passball_1"
      end
   end,
   Receiver = task.ReceiverTask("getball"),                                    
   Kicker=task.GotoPos("Kicker",190,0,Kickerdir2Balldir),
  --Kick=task.GotoPos("Receiver",190,0,Receiverdir2Balldir),   --ReceiverTask("goto01")            
},             
 
["Passball_1"] = {
      switch = function()
          if CIsBallKick("Receiver")   then
              return "Delay_2"
          end
      end,

      --Kicker = task.KickerTask("passball02"),--or pass_ball有问题
      Receiver =task.ReceiverTask("passball_corner_light13"),--passball_corner_light10，pass_ball
      Kicker=task.GotoPos("Kicker",190,0,RKickerdir2Balldir), 
},--前面都ok

["Delay_2"] = {
   switch = function()
      if  CBall2RoleDist("Kicker")<11 then --时间注意 or Cbuf_cnt(true, 100)               
         return "shoot"
      end
   end,
   Receiver= task.GotoPos("Receiver",0,away_y,Kickerdir2Balldir),                                    
  Kicker=task.KickerTask("receive"),--拿球(固定原地可能有问题)   receive          
},   
-- Cbuf_cnt(CBall2RoleDist("Receiver")<9, 30) or 
--拿球完成

["shoot"]={
   switch=function()
     if CGetBallX() > 285 and CGetBallY() < 70 and CGetBallY() > -70 then
        return "Finish"
      end
     end,
   Kicker = task.KickerTask("shoot02"),--或者shoot/mydemo1
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

name = "1corcer-1-03" 
}