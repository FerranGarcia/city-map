function this = push(this, newMapNode)
% returns MapNode with id value given in textId or numId
% if not found returns NaN
    if (nargin==2)
        if (isa(newMapNode,'MapNode'))       
            this.nodes(end+1) = newMapNode;
        else
            % otherwise display error message
            error('Wrong input arguments for push newMapNode')        
        end
    else
        % otherwise display error message
        error('Wrong number of input arguments for push newMapNode')
    end
end