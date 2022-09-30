// Animation by "requestAnimationFrame"

function fadeIn(el) {
    
    function tick() {
        
        // Seperate step and condition can reduce one implement loop
        el.style.opacity = +el.style.opacity + 0.01;
        if (+el.style.opacity < 0.9) {
            
        	reqID = window.requestAnimationFrame(tick);
      	}
        
        else {
        	window.cancelAnimationFrame(reqID);
        }
    }
    
    let reqID;
    el.style.opacity = 0;
    
    tick();
}

document.addEventListener("DOMContentLoaded", function() {
    
    let content = document.querySelector(".content");
    fadeIn(content);
    
    let btn = document.querySelector(".btn");
    btn.addEventListener("click", function() {
        
        // Make sure icon switch immediately after clicking
        let open = document.querySelector(".open");
        let pack = document.querySelector(".pack");
        let status = document.querySelector(".btn");
        
        open.style.display = "contents";
        pack.style.display = "none";
        
        /* Make sure icon switch after text box fully packed up
        Use setTimeout to avoid asynchronous between collapsing and icon switching*/
        setTimeout(function() {
            
            if (status.getAttribute("aria-expanded") === "true") {
                open.style.display = "contents";
                pack.style.display = "none";
            }
            
            else {
                open.style.display = "none";
                pack.style.display = "contents";
            }
            
        }, 1000);
        
    });
});


/* Animation by "setInterval"
function fadeIn(el) {
    
    let tick = function() {
        
        if (+el.style.opacity < 1) {
        	el.style.opacity = +el.style.opacity + 0.01;
      	}
        
        else {
        	clearInterval(reqID);
        }
    
    };
    
    el.style.opacity = 0;
    let reqID = setInterval(tick, 5);
}


document.addEventListener("DOMContentLoaded", function() {
    let content = document.querySelector(".content");
    
    // Prepend "window.onload" (optional)
    fadeIn(content);
});*/