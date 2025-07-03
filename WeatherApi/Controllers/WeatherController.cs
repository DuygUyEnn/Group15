using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using WeatherApi.Data;
using WeatherApi.Models;
using WeatherApi.Dtos;



namespace WeatherApi.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class WeatherController : ControllerBase
    {
        private readonly WeatherDbContext _context;

        public WeatherController(WeatherDbContext context)
        {
            _context = context;
        }

        // API: GET /api/weather
        [HttpGet]
        public async Task<ActionResult> GetWeatherData(
     [FromQuery] string mode = "month",
     [FromQuery] DateTime? cursorDate = null,
     [FromQuery] int page = 1,
     [FromQuery] int pageSize = 1000)
        {
            if (!new[] { "day", "week", "month" }.Contains(mode.ToLower()))
                return BadRequest("Mode phải là: day, week, hoặc month");

            DateTime end = cursorDate?.ToUniversalTime() ?? DateTime.UtcNow;
            DateTime start;

            switch (mode.ToLower())
            {
                case "day":
                    start = new DateTime(end.Year, end.Month, end.Day, 0, 0, 0, DateTimeKind.Utc);
                    end = start.AddDays(1).AddTicks(-1);
                    break;
                case "week":
                    start = end.AddDays(-7);
                    end = end.AddTicks(-1);
                    break;
                case "month":
                    start = end.AddMonths(-1);
                    end = end.AddTicks(-1);
                    break;
                default:
                    return BadRequest("Mode không hợp lệ.");
            }

            // Tính ngày trước/sau
            DateTime prevDate, nextDate;
            switch (mode.ToLower())
            {
                case "day":
                    prevDate = end.AddDays(-1);
                    nextDate = end.AddDays(1);
                    break;
                case "week":
                    prevDate = end.AddDays(-7);
                    nextDate = end.AddDays(7);
                    break;
                case "month":
                    prevDate = end.AddMonths(-1);
                    nextDate = end.AddMonths(1);
                    break;
                default:
                    prevDate = nextDate = DateTime.Today;
                    break;
            }

            var query = _context.messages
                .Where(m => m.Time >= start && m.Time <= end)
                .OrderBy(m => m.Time);

            var totalRecords = await query.CountAsync();

            var pagedData = await query
                .Skip((page - 1) * pageSize)
                .Take(pageSize)
                .ToListAsync();

            return Ok(new
            {
                mode,
                cursorDate = end.ToString("yyyy-MM-dd"),
                data = pagedData,
                page,
                pageSize,
                totalRecords,
                prevCursorDate = prevDate.ToString("yyyy-MM-dd"),
                nextCursorDate = nextDate.ToString("yyyy-MM-dd")
            });
        }


    }
}
