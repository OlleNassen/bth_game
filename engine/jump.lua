function jump()
	if self.isJumping == false then
		 self.entity.velocity.y = 100.0f
		 self.isJumping = true

		 self:resetCharge()
	end

	return false
end

function notJump()
	if self.isJumping == true then
		self.entity.velocity.y = -30.0f
		self.isJumping = false;
	end
end